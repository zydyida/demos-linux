/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * linuxapi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * linuxapi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with linuxapi. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __signal_utils_h
#define __signal_utils_h

/*
 * This is a collection of helper function to help with dealing
 * with the Linux signal API
 */

/* THIS IS A C FILE, NO C++ here */

#include <firstinclude.h>
#include <us_helper.h>	// for ARRAY_SIZEOF()
#include <err_utils.h>	// for CHECK_ASSERT(), CHECK_NOT_SIGT(), CHECK_NOT_M1()
#include <stdio.h>	// for printf(3)
#include <string.h>	// for strcmp(3), strsignal(3)
#include <signal.h>	// for siginfo_t, sighandler_t, sigaction(2), signal(2)

/*
 * List of signals can be found in [man 7 signal]
 * You can find them in [/usr/include/asm/signal.h]
 * [kill -l] in bash (builtin) will list all signals.
 * [kill -l] or [kill --list] from [/bin/kill] will list them too.
 * [kill -L] or [kill --table] from [/bin/kill] will show them in a nice table.
 */

typedef struct _sig_val_and_name {
	int val;
	const char* name;
} sig_val_and_name;

/*
 * List of all signals and their names. The reason for this is that the standard
 * C library does not provide a translation from signal name to signal value.
 * (it does provide the reverse translation via strsignal(3)).
 * Note: you cant use stringification macros here because they will substitute
 * the macro. Use only macros that don't substitute.
 */
#define entry(x) { x, # x }
static sig_val_and_name sig_tbl[]={
	entry(SIGHUP),
	entry(SIGINT),
	entry(SIGQUIT),
	entry(SIGILL),
	entry(SIGTRAP),
	entry(SIGABRT),
	entry(SIGIOT),
	entry(SIGBUS),
	entry(SIGFPE),
	entry(SIGKILL),
	entry(SIGUSR1),
	entry(SIGSEGV),
	entry(SIGUSR2),
	entry(SIGPIPE),
	entry(SIGALRM),
	entry(SIGTERM),
	entry(SIGSTKFLT),
	entry(SIGCHLD),
	entry(SIGCONT),
	entry(SIGSTOP),
	entry(SIGTSTP),
	entry(SIGTTIN),
	entry(SIGTTOU),
	entry(SIGURG),
	entry(SIGXCPU),
	entry(SIGXFSZ),
	entry(SIGVTALRM),
	entry(SIGPROF),
	entry(SIGWINCH),
	entry(SIGIO),
	entry(SIGPOLL),
	entry(SIGPWR),
	entry(SIGSYS),
	entry(SIGUNUSED),
	entry(SIGRTMIN),
	entry(SIGRTMAX),
};
#undef entry

/*
 * Translate signal name to signal value
 */
static inline int signal_get_by_name(const char* name) {
	unsigned int i;
	for(i=0; i<ARRAY_SIZEOF(sig_tbl); i++) {
		if(strcmp(name, sig_tbl[i].name)==0) {
			return sig_tbl[i].val;
		}
	}
	CHECK_ASSERT("bad signal name"==NULL);
	return -1;
}

/*
 * Print out a table of all signal values, names and descriptions
 */
static inline void print_signal_table() {
	unsigned int i;
	printf("number of signal values is %zd\n", ARRAY_SIZEOF(sig_tbl));
	for(i=0; i<ARRAY_SIZEOF(sig_tbl); i++) {
		int val=sig_tbl[i].val;
		const char* name=sig_tbl[i].name;
		printf("i=%d, sig=%d, in_code=%s, strsignal(%d)=%s\n", i, val, name, val, strsignal(val));
	}
}

/*
 * a function to print the siginfo_t structure
 * see 'man sigaction' for details on the internals of
 * this structure...
 * This function DOES NOT print all the fields of the siginfo
 * structure as there are just too many of them...
 */
static inline void print_siginfo(FILE* out, siginfo_t *si) {
	fprintf(out, "sighandler: si is %p\n", si);
	fprintf(out, "sighandler: si_signo is: %d\n", si->si_signo);
	fprintf(out, "sighandler: si_errno is: %d\n", si->si_errno);
	fprintf(out, "sighandler: si_pid is: %d\n", si->si_pid);
	fprintf(out, "sighandler: si_uid is: %d\n", si->si_uid);
	fprintf(out, "sighandler: si_addr is: %p\n", si->si_addr);
}

/*
 * easy registration of signals via signal(2)
 */
static inline void register_handler_signal(int signum, sighandler_t handler) {
	CHECK_NOT_SIGT(signal(signum, handler), SIG_ERR);
}

/*
 * easy registration of signals via sigaction(2)
 */
typedef void (*my_sig_handler)(int, siginfo_t *, void *);
static inline void register_handler_sigaction(int sig, my_sig_handler handler) {
	struct sigaction sa;
	sa.sa_flags=SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction=handler;
	CHECK_NOT_M1(sigaction(sig, &sa, NULL));
}

#endif	/* !__signal_utils_h */

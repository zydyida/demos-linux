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

#include <firstinclude.h>
#include <sys/inotify.h>// for inotify_init(2), inotify_add_watch(2)
#include <stdio.h>	// for printf(3)
#include <unistd.h>	// for read(2), close(2)
#include <assert.h>	// for assert(3)
#include <errno.h>	// for errno(3)
#include <sys/types.h>	// for getpid(2)
#include <stdlib.h>	// for EXIT_FAILURE, EXIT_SUCCESS
#include <signal.h>	// for siginfo_t
#include <string.h>	// for strcmp(3)
#include <us_helper.h>	// for CHECK_NOT_M1()
#include <fcntl.h>	// for DN_* constants

/*
 * Demonstrate the use of the (obsolete) dnotify feature to obtain directory
 * change notifications. (Modern programs should use inotify instead of
 * dnotify. The inotify API is available in Linux 2.6.13 and later.)
 * Usage is as shown in usageError() below. An example is the following:
 * dnotify dir1:a xyz/dir2:acdM
 * See also demo_inotify.c.
*/

/* Print (optional) message and usage info, then exit */
static void usageError(const char *progName, const char *msg) {
	if (msg != NULL)
		fprintf(stderr, "%s", msg);
	fprintf(stderr, "Usage: %s directory:[events]...\n", progName);
	fprintf(stderr, "    Events are:\n"
		"        a - access; A - attrib; c - create; d - delete\n"
		"        m - modify; r - rename; M - multishot\n"
		"        (default is all events)\n");
	exit(EXIT_FAILURE);
}

static void handler(int sig, siginfo_t *si, void *ucontext) {
	printf("Got event on descriptor %d\n", si->si_fd);
}

int main(int argc, char** argv, char** envp) {
	struct sigaction sa;
	int fd, events, fnum;
	const int NOTIFY_SIG = SIGRTMIN;
	char *p;

    if (argc < 2 || strcmp(argv[1], "--help") == 0)
        usageError(argv[0], NULL);

    /* Establish handler for notification signal */

    sa.sa_sigaction = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;           /* So handler gets siginfo_t arg. */
	CHECK_NOT_M1(sigaction(NOTIFY_SIG, &sa, NULL));

    for (fnum = 1; fnum < argc; fnum++) {
        p = strchr(argv[fnum], ':');    /* Look for optional ':' */

        if (p == NULL) {                /* Default is all events + multishot */
            events = DN_ACCESS | DN_ATTRIB | DN_CREATE | DN_DELETE |
                     DN_MODIFY | DN_RENAME | DN_MULTISHOT;
        } else {                        /* ':' present, parse event chars */
            *p = '\0';                  /* Terminates directory component */
            events = 0;
            for (p++; *p != '\0'; p++) {
                switch (*p) {
                case 'a': events |= DN_ACCESS;          break;
                case 'A': events |= DN_ATTRIB;          break;
                case 'c': events |= DN_CREATE;          break;
                case 'd': events |= DN_DELETE;          break;
                case 'm': events |= DN_MODIFY;          break;
                case 'r': events |= DN_RENAME;          break;
                case 'M': events |= DN_MULTISHOT;       break;
                default:  usageError(argv[0], "Bad event character\n");
                }
            }
        }

        /* Obtain a file descriptor for the directory to be monitored */

        fd = CHECK_NOT_M1(open(argv[fnum], O_RDONLY));
        printf("opened '%s' as file descriptor %d\n", argv[fnum], fd);

        /* Use alternate signal instead of SIGIO for dnotify events */

        CHECK_NOT_M1(fcntl(fd, F_SETSIG, NOTIFY_SIG));

        /* Enable directory change notifications */

        CHECK_NOT_M1(fcntl(fd, F_NOTIFY, events));
        printf("events: %o\n", (unsigned int) events);
    }

	while(true) {
		pause();
	}
	return EXIT_SUCCESS;
}

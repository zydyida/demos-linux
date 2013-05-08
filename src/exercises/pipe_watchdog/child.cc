/*
 * This file is part of the linuxapi project.
 * Copyright (C) 2011-2013 Mark Veltzer <mark.veltzer@gmail.com>
 *
 * The linuxapi package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * The linuxapi package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with the GNU C Library; if not, write to the Free
 * Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA.
 */

#include <firstinclude.h>
#include <stdio.h>	// for printf(3), fprintf(3)
#include <sys/types.h>	// for kill(2), open(2)
#include <sys/stat.h>	// for open(2)
#include <fcntl.h>	// for open(2)
#include <signal.h>	// for kill(2), sigaction(2)
#include <unistd.h>	// for alarm(2), read(2), write(2)
#include <stdlib.h>	// for EXIT_SUCCESS, EXIT_SUCCESS, exit(3), atoi(3)
#include <us_helper.h>	// for CHECK_NOT_M1()

const int BUFSIZE=1024;

int sigme;

void sigalrmHandler(int gotsig) {
	kill(getppid(), sigme);
	alarm(2);
}

void do1() {
	char buffer[BUFSIZE];
	int npfd;
	int rcount;
	while((npfd=open("np", O_RDONLY))==-1) {
		if(errno!=EINTR) {
			CHECK_NOT_M1(-1);
		}
	}
	fprintf(stderr, "np opend now read\n");
	while((rcount=read(npfd, buffer, sizeof(buffer))) > 0 || errno==EINTR) {
		if(errno!=EINTR) {
			buffer[rcount]='\0';
			ssize_t ret=write(1, buffer, strlen(buffer));
			CHECK_ASSERT(ret!=(ssize_t)strlen(buffer));
		}
	}
}

void do2() {
	char buffer[BUFSIZE];
	int rcount;
	while((rcount=read(0, buffer, sizeof(buffer))) > 0 || errno==EINTR) {
		if(errno!=EINTR) {
			buffer[rcount]='\0';
			printf("Servicing request %s\n", buffer);
		}
	}
}

int main(int argc, char** argv, char** envp) {
	if(argc < 2) {
		fprintf(stderr, "%s: Usage: %s 1 or 2\n", argv[0], argv[0]);
		exit(EXIT_FAILURE);
	}
	int me=atoi(argv[1]);
	struct sigaction sigalrm;
	sigset_t emptyset;
	sigemptyset(&emptyset);
	sigalrm.sa_handler=sigalrmHandler;
	sigalrm.sa_mask=emptyset;
	sigalrm.sa_flags=0;
	CHECK_NOT_M1(sigaction(SIGALRM, &sigalrm, NULL));
	alarm(2);
	if(me < 1 || me > 2) {
		fprintf(stderr, "I said 1 or 2\n");
		exit(EXIT_FAILURE);
	}
	switch(me) {
	case 1:
		sigme=SIGUSR1;
		do1();
		break;
	case 2:
		sigme=SIGUSR2;
		do2();
		break;
	}
	return EXIT_SUCCESS;
}
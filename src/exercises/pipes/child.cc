/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include<firstinclude.h>
#include<errno.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<signal.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h> // for EXIT_SUCCESS, EXIT_SUCCESS, exit(3)

const int BUFSIZE=1024;

int sigme;

void sigalrmHandler(int gotsig) {
	kill(getppid(),sigme);
	alarm(2);
}

void do1() {
	char buffer[BUFSIZE];
	int npfd;
	int rcount;
	while((npfd=open("np",O_RDONLY))==-1) {
		if(errno!=EINTR) {
			perror("Open named pipe \"np\" failed");
			exit(errno);
		}
	}
	fprintf(stderr, "np opend now read\n");
	while((rcount=read(npfd, buffer, sizeof(buffer))) > 0 || errno==EINTR) {
		if(errno!=EINTR) {
			buffer[rcount]='\0';
			ssize_t ret=write(1, buffer, strlen(buffer));
			if(ret!=(ssize_t)strlen(buffer)) {
				perror("did not write fully");
			}
		}
	}
}

void do2() {
	char buffer[BUFSIZE];
	int rcount;
	while((rcount=read(0, buffer, sizeof(buffer))) > 0 || errno==EINTR) {
		if(errno!=EINTR) {
			buffer[rcount]='\0';
			printf("Servicing request %s\n",buffer);
		}
	}
}

int main(int argc,char** argv,char** envp) {
	int me;
	struct sigaction sigalrm;
	sigset_t emptyset;
	sigemptyset(&emptyset);
	sigalrm.sa_handler=sigalrmHandler;
	sigalrm.sa_mask=emptyset;
	sigalrm.sa_flags=0;
	if(sigaction(SIGALRM, & sigalrm, NULL)==-1) {
		perror("sigaction SIGUSR1 failed");
		exit(errno);
	}
	alarm(2);
	if(argc < 2) {
		fprintf(stderr, "Usage: %s 1 or 2\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	me=atoi(argv[1]);
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
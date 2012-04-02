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
#include<string.h>
#include<stdio.h>
#include<assert.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * EXTRA_CMDS=pkg-config --cflags --libs ACE
 */
int main(int argc,char** argv,char** envp) {
	struct sockaddr_in srvr;
	memset(&srvr,0,sizeof(srvr));
	srvr.sin_family=AF_INET;
	srvr.sin_addr.s_addr=inet_addr("127.0.0.1");
	srvr.sin_port=htons(50000);
	int fd=socket(AF_INET,SOCK_STREAM,0);
	assert(fd>=0);
	assert(connect(fd,(struct sockaddr*)&srvr,sizeof(srvr))==0);
	char buf[64];
	memset(buf,0,sizeof(buf));
	int res=write(fd,"uptime\n",7);
	res++;
	int bc=read(fd,buf,sizeof(buf));
	res=write(1,buf,bc);
	close(fd);
	return EXIT_SUCCESS;
}

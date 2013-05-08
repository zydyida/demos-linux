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
#include <stdlib.h>	// for EXIT_SUCCESS
#include <stdio.h>	// for printf(3)
#include <unistd.h>	// for usleep(3)
#include <us_helper.h>	// for CHECK_NOT_M1()

/*
 * This is an uncontrolled program that we want to arrange logging for...
 */
int main(int argc, char** argv, char** envp) {
	unsigned int counter=0;
	while(true) {
		printf("%d: This is yet another log message\n", counter);
		usleep(100);
		counter++;
	}
	return EXIT_SUCCESS;
}

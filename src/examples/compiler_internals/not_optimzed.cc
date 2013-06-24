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
#include <stdio.h>	// for printf(3)
#include <stdlib.h>	// for EXIT_SUCCESS
#include <multi_processing.h>	// for my_system()

/*
 * This is an example of a loop that gcc cannot optimize away...
 * Consider the question why ? Doesn't gcc simulate your code
 * in order to optimize it away ? The answer is that it does
 * not simulate your code but rather understand the underlying
 * construct to deduce what you are doing and use a predefined
 * set of formulas to precalculate the result...
 *
 * You can see that indeed it is not optimized when you disassemble
 * the code.
 *
 * Compare the second loop to the first.
 */

int main(int argc, char** argv, char** envp) {
	int sum=0;
	for(int i=0; i<100; i++) {
		sum+=i;
	}
	printf("sum is %d\n", sum);
	for(int i=0; i<100; i++) {
		sum+=i*i;
	}
	printf("sum is %d\n", sum);
	// disassemble myself...
	my_system("objdump --disassemble --source %s", argv[0]);
	return EXIT_SUCCESS;
}

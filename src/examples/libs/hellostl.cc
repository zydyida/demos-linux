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
#include<vector> // for std::vector
#include<iostream> // for std::cout, std::endl
#include<stdlib.h> // for EXIT_SUCCESS

/*
 * This program demonstrates the use of STL (Standard Template Library).
 */
int main(int argc,char** argv,char** envp) {
	std::vector<int> v(3);

	v[0] = 5;
	v[1] = 8;
	v[2] = 9;
	std::cout << v[0] << "-" << v[1] << "-" << v[2] << std::endl;
	return EXIT_SUCCESS;
}

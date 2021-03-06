/*
 * This file is part of the linuxapi package.
 * Copyright (C) 2011-2020 Mark Veltzer <mark.veltzer@gmail.com>
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
#define DEBUG
#ifdef DEBUG
#define CWDEBUG
#include <libcwd/sys.h>
#include <libcwd/debug.h>
#include <stdlib.h>	// for EXIT_SUCCESS

/*
 * This is a demo program showing how to use libcwd for C++ debug support...
 *
 * EXTRA_LINK_FLAGS=-lcwd
 */

// Define our own debug channel:
namespace libcwd {
namespace channels {
namespace dc {
channel_ct hello("HELLO");
};
};
};
#endif

// using namespace libcwd::channels::dc;

int main(int argc, char** argv, char** envp) {
	Debug(check_configuration());
	Debug(libcw_do.on());
	// List all debug channels (nor very usefull unless you allow to turn
	// channels on and off from the commandline; this is supported in libcw).
	Debug(list_channels_on(libcw_do));
	Debug(dc::hello.on());
	Dout(dc::hello, "==============================");
	Debug(list_channels_on(libcw_do));
	Dout(dc::hello, "Hello World!");
	return EXIT_SUCCESS;
}

/* HP 39g+ / 39gs / 40gs series keyboard input module

Copyright (C) 2005 The HP-GCC Development Team
Copyright (C) 2016 Arnie97

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "s3c2410.h"
#include "hp39kbd.h"
#include "main.h"

void __keybdelay()
{
	// REPLACE BY SOMETHING MORE SOPHISTICATED LATER
	int k;

	for(k=0;k<100;++k) ;
}

void keyb_getmatrix(keymatrix *m)
{
    // // by Claudio, with portions (well, most of it) of code by Al

    int lo = 0, hi = 0;

    int col;
    unsigned int control;

    for (col = 7; col >= 4; --col) {

        control = 1 << ((col + 8) * 2); //set the correct col pin to output, others inputs
        control = control | 0xAAA9; //fix up the lower bits
        *GPGCON = control; // write the new control value.

        // DELAY 100us APPROX.
        __keybdelay();

        hi = (hi << 8) | ((~( *GPGDAT)) & 0xfe);

    }

    for (; col >= 0; --col) {

        control = 1 << ((col + 8) * 2); //set the correct col pin to output, others inputs
        control = control | 0xAAA9; //fix up the lower bits
        *GPGCON = control; // write the new control value.

        // DELAY 100us APPROX.
        __keybdelay();

        lo = (lo << 8) | ((~( *GPGDAT)) & 0xfe);

    }

    *GPGCON = 0x5555AAA9; //restore default

    hi |= ((*GPFDAT) & 0x70) << 24;
    hi |= (*GPFDAT) << 31;

    m->words[0] = lo;
    m->words[1] = hi;

    return;
}

int get_1st_key(int wait) {
	keymatrix m;
	if(wait) {
		// wait for a non-shift key to be pressed
		// while( ((m.words[0]&0x8fffffff)|m.words[1] )==0 ) keyb_getmatrix(&m);
		while (!any_key_pressed);
	}

	keyb_getmatrix(&m);
	for (int i = 0; i < 64; ++i)
	{
		if (m.full & (((ULONGLONG)1)<<i))
		{
			return i;
		}
	}
	return get_1st_key(wait);
}

int
get_key(void)
{
	// wait until a key is pressed
	while (!any_key_pressed);
	// deal with the modifier keys
	if (comma_pressed) {
		return event_handler(3, 4);
	} else if (alpha_pressed) {
		return event_handler(4, 4);
	} else if (shift_pressed) {
		return event_handler(5, 4);
	} else if (on_pressed) {
		return event_handler(6, 4);
	}
	 else if (!any_normal_key_pressed) {
		return 0;
	}

	for (unsigned c = 0; c < 8; c++) {
		// set the current column pin to output, others inputs
		*GPGCON = (1 << 16 << c * 2) | 0xAAA9;
		delay(100);

		// skip this column if no key in it is pressed
		if (!any_key_pressed) {
			continue;
		}
		for (unsigned r = 0; r < 8; r++) {
			// check whether a row is active
			if (!(*GPGDAT & (1 << r + 1))) {
				// restore all column pins to output
				*GPGCON = 0x5555AAA9;
				delay(100);

				// return at the first detected key press
				return event_handler(r, c);
			}
		}
	}

	// the key was released before it could be captured, hence try again
	return get_key();
}

/* A simple user interface for this project

Copyright (C) 2016 Arnie97
Copyright (C) 2019 jejer

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

#include <hpstdio.h>
#include <hpconio.h>
#include <hpgraphics.h>
#include "hp39kbd.h"
#include "display.h"
#include "main.h"

int
event_handler(unsigned row, unsigned col)
{
	return row*10+col;
	// [APLET]
	if (row == 0 && col == 7) {
		// exit immediately
		return 5;
	} else {
		// wait until the key is released
		set_indicator(indicator_wait, TRUE);
		while (any_key_pressed);
		set_indicator(indicator_wait, FALSE);
	}

	// [UP]: 0, [DOWN]: 2, [PLOT]: 4, [HOME]: 6
	if (col == 6 && !(row & 1)) {
		return (row >> 1) + 1;
	} else if (row <= 6 && col <= 4) {
		int ch = row * 5 - col + 'D';  // letter keys
		if (ch == 'D') {
			return 0;  // [DEL]
		} else if (ch < 'D') {
			ch++;  // skip the [DEL] key after [D]
		} else if (ch >= 'T') {
			ch--;  // skip the [ALPHA] key before [T]
			if (ch >= 'X') {
				ch--;  // skip the [SHIFT] key before [X]
				if (row == 6 && col == 0) {
					return 6;  // [ENTER]
				} else if (ch > 'Z') {
					return 0;
				}
			}
		}
		return ch;
	} else if (row == 6 && col == 5) {
		return 7;  // [SYMB]
	}

	// unhandled keys
	return 0;
}


int
main(void)
{
	keymatrix matrix;
	hpg_set_mode_gray16(1);

	hpg_set_font(hpg_stdscreen, hpg_get_bigfont());
	hpg_set_color(hpg_stdscreen, HPG_COLOR_GRAY_6);

	hpg_clear();
	hpg_draw_text("Hello, world!", 0, 0);

	hpg_set_font(hpg_stdscreen, hpg_get_minifont());
	hpg_set_color(hpg_stdscreen, HPG_COLOR_BLACK);
	hpg_draw_text("Hello, world!", 0, 5);

	hpg_flip(); //updates the screen with the new image (needed in double-buffered mode)
	while (!any_key_pressed);

	hpg_set_mode_mono(0);
	clear_screen();
	printf(
		"test"
	);
	delay(50000);
	for (int i = 0; i < 6; i++) {
		set_indicator((i + 1) % 6, TRUE);
		set_indicator(i, FALSE);
		delay(400000);
	}

	gotoxy(0, 8);
	printf(
		"Config:  [PLOT]   Exit:   [APLET]"
		"Refresh: [HOME]   About:  [SYMB]"
	);

	int x = 0;
	for (;;) {
		delay(400000);
		int key = get_1st_key(1);
		clear_screen();
		gotoxy(0, 0);
		printf("%d\n", key);
		keyb_getmatrix(&matrix);
		gotoxy(0, 1);
		printf("%x\n", matrix.words[0]);
		gotoxy(0, 2);
		printf("%x\n", matrix.words[1]);
		gotoxy(0, 3);
		printf("%d", x);
		x++;
		matrix.full = 0;
	}
}

int
show_credits(void)
{
	clear_screen();
	printf(
		"Copyright (C) 2005 HP-GCC Team\n"
		"Copyright (C) 2016 Arnie97\n"
		"Copyright (C) 2019 jejer\n"
	);
	gotoxy(0, 9);
	printf(
		"Exit:   [APLET]"
	);
	for (;;) {
		int k = get_key();
		if (k == 5)  // [APLET]
			return 0;  // exit program
	}
}

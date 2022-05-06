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
#include <hpstring.h>
#include <hpconio.h>
#include <hpgraphics.h>
#include "hp39kbd.h"
#include "display.h"
#include "main.h"

#define MODE_DEC 0
#define MODE_HEX 1
#define MODE_BIN 2
#define MODE_UNSIGN 3
#define MODE_SIGN 4

const int MIN_SIGN = -2147483648;
const int MAX_SIGN = 2147483647;
const unsigned int MAX_UNSIGN = 4294967295;

unsigned int answer = 0;
char string_bin[34];
char string_hex[9];
char string_dec[12];
char string_debug[34];
int mode = MODE_DEC;
int mode_sign = MODE_UNSIGN;
// char* string_bin = "00000000000000000000000000000000"
// char* string_hex = "00000000"
// char* string_dec = "-0000000000"

void update_strings() {
	string_bin[0] = 'B';
	string_bin[1] = '\0';
	string_hex[0] = '\0';
	string_dec[0] = '\0';
	if (mode_sign == MODE_UNSIGN) {
		sprintf(string_dec, "%u", answer);
	} else {
		sprintf(string_dec, "%d", answer);
	}
	sprintf(string_hex, "%x", answer);
	for (unsigned int z = 0x80000000; z != 0; z >>= 1) {
		strcat(string_bin, ((answer & z) == z) ? "1" : "0");
	}
}

void update_screen() {
	update_strings();
	clear_screen();
	gotoxy(0, 0);
	printf(string_bin);
	gotoxy(0, 1);
	printf("HEX %s", string_hex);
	gotoxy(0, 2);
	printf("DEC %s", string_dec);
	gotoxy(0, 8);
	printf(string_debug);
	gotoxy(0, 9);
	if (mode == MODE_DEC) {
		printf("[DEC]  HEX  BIN");
	} else if (mode == MODE_HEX) {
		printf("DEC  [HEX]  BIN");
	} else if (mode == MODE_BIN) {
		printf("DEC  HEX  [BIN]");
	}
	if (mode_sign == MODE_UNSIGN) {
		printf("  [US]");
	} else {
		printf("  [S]");
	}
}

int
event_handler(unsigned row, unsigned col)
{
	// return row*10+col;
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

int mapKeyToNum(int key) {
	if (key == 31) return 0;
	if (key == 30) return 1;
	if (key == 22) return 2;
	if (key == 14) return 3;
	if (key == 29) return 4;
	if (key == 21) return 5;
	if (key == 13) return 6;
	if (key == 28) return 7;
	if (key == 20) return 8;
	if (key == 12) return 9;
	if (key == 33) return 10;
	if (key == 25) return 11;
	if (key == 17) return 12;
	if (key == 9) return 13;
	if (key == 34) return 14;
	if (key == 26) return 15;
	return -1;
}

int getLeft() {
	int left = 0;
	switch (mode) {
		case MODE_DEC:
			if (mode_sign == MODE_UNSIGN) {
				if (answer > MAX_UNSIGN/10) {
					left = -1;
				} else {
					left = MAX_UNSIGN/10 - answer;
					if (left == 0) left = MAX_UNSIGN - answer*10;
				}
			} else {
				if (answer > MAX_SIGN/10) {
					left = -1;
				} else {
					left = MAX_SIGN/10 - answer;
					if (left == 0) left = MAX_SIGN - answer*10;
				}
			}
			break;
		case MODE_HEX:
			if ((answer & 0xF0000000) > 0) {
				left = -1;
			} else {
				left = 0xF;
			}
			break;
		case MODE_BIN:
			if ((answer & 0x80000000) > 0) {
				left = -1;
			} else {
				left = 1;
			}
			break;
	}
	return left;
}

void handle(int key) {
	if (key == 63) {	// ON
		answer = 0;
		return;
	}
	if (key == 41) {	// F1
		mode = MODE_DEC;
		return;
	}
	if (key == 42) {		// F2
		mode = MODE_HEX;
		return;
	}
	if (key == 43) {	// F3
		mode = MODE_BIN;
		return;
	}
	if (key == 44) {	// F4
		mode_sign = mode_sign == MODE_UNSIGN ? MODE_SIGN : MODE_UNSIGN;
		return;
	}
	if (key == 1) {	// DEL
		switch (mode) {
		case MODE_BIN:
			answer >>= 1;
			return;
		case MODE_HEX:
			answer >>= 4;
			return;
		case MODE_DEC:
			answer /= 10;
			return;
		}
	}

	unsigned int left = getLeft();
	int input = mapKeyToNum(key);
	// int input = 7;
	sprintf(string_debug, "left=%d key=%d input=%d", left, key, input);
	if (input == -1 || left == -1) {
		return;
	}
	if (input == 0 && answer == 0) {
		return;
	}
	if (input > left) {
		return;
	}
	// if (mode == MODE_DEC) { //dec
	// 	if (input > 10) return;
	// 	answer *= 10;
	// 	answer += input;
	// } else ()
	switch (mode) {
		case MODE_DEC:
			if (input < 10) {
				answer *= 10;
				answer += input;
			}
			break;
		case MODE_HEX:
			answer <<= 4;
			answer |= input;
			break;
		case MODE_BIN:
			answer <<= 1;
			answer |= input;
			break;
	}
}



int
main(void)
{
	string_debug[0] = '\0';
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
	// delay(50000);
	// for (int i = 0; i < 6; i++) {
	// 	set_indicator((i + 1) % 6, TRUE);
	// 	set_indicator(i, FALSE);
	// 	delay(400000);
	// }

	gotoxy(0, 8);
	printf(
		"Config:  [PLOT]   Exit:   [APLET]"
		"Refresh: [HOME]   About:  [SYMB]"
	);

	int x = 0;
	for (;;) {
		delay(400000);
		int key = get_1st_key(1);
		handle(key);
		update_screen();
		// clear_screen();
		// gotoxy(0, 0);
		// printf("%d\n", key);
		// keyb_getmatrix(&matrix);
		// gotoxy(0, 1);
		// printf("%x\n", matrix.words[0]);
		// gotoxy(0, 2);
		// printf("%x\n", matrix.words[1]);
		// gotoxy(0, 3);
		// printf("%d", x);
		// x++;
		// matrix.full = 0;
		if (key == 55 || key == 57) {
			// clear_screen();
			break;
		}
	}
	// show_credits();
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

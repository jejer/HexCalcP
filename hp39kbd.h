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

#ifndef _HP39KBD_H
#define _HP39KBD_H

#include "s3c2410.h"

// LONG TYPE DEFINITION, ADDED BY CLAUDIO 01/14/05
#ifndef _LONGLONG_DEF
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
#define _LONGLONG_DEF
#endif

/*
KEYBOARD BIT MAP
----------------
This is the bit number in the 64-bit keymatrix.
Bit set means key is pressed.

    F1]+  F2]+  F3]+  F4]+  F5]+  F6]+  
    |41|  |42|  |43|  |44|  |45|  |46|
    +--+  +--+  +--+  +--+  +--+  +--+  

    SY]+  PL]+  NU]+        UP]+        
    |47|  |53|  |54|        |49|      
    +--+  +--+  +--+  LF]+  +--+  RT]+  
                      |50|  DN]+  |52|
    HO]+  AP]+  VI]+  +--+  |51|  +--+  
    |55|  |57|  |58|        +--+    
    +--+  +--+  +--+                    

    A]--+  B]--+  C]--+  D]--+  DEL]+
    | 33|  | 25|  | 17|  | 09|  | 01|
    +---+  +---+  +---+  +---+  +---+

    E]--+  F]--+  G]--+  H]--+  I]--+
    | 34|  | 26|  | 18|  | 10|  | 02|
    +---+  +---+  +---+  +---+  +---+

    J]--+  K]--+  L]--+  M]--+  /]--+
    | 35|  | 27|  | 19|  | 11|  | 03|
    +---+  +---+  +---+  +---+  +---+

    O]-+  7]--+  8]--+  9]--+  *]--+
    | 60|  | 28|  | 20|  | 12|  | 04|
    +---+  +---+  +---+  +---+  +---+

    AL]-+  4]--+  5]--+  6]--+  -]--+
    | 61|  | 29|  | 21|  | 13|  | 05|
    +---+  +---+  +---+  +---+  +---+

    SF]-+  1]--+  2]--+  3]--+  +]--+
    | 62|  | 30|  | 22|  | 14|  | 06|
    +---+  +---+  +---+  +---+  +---+

    ON]-+  0]--+  .]--+    ]-+  EN]-+
    | 63|  | 31|  | 23|  | 15|  | 07|
    +---+  +---+  +---+  +---+  +---+


id = col * 0xf + row
F1:          row=0, col=5, id=75,  matrixid=KB_F1
F2:          row=1, col=5, id=76,  matrixid=KB_F2
F3:          row=2, col=5, id=77,  matrixid=KB_F3
F4:          row=3, col=5, id=78,  matrixid=KB_F4
F5:          row=4, col=5, id=79,  matrixid=KB_F5
F6:          row=5, col=5, id=80,  matrixid=KB_F6
SYMB:        row=6, col=5, id=81,  matrixid=KB_SY
PLOT:        row=4, col=6, id=94,  matrixid=KB_PL
NUM:         row=5, col=6, id=95,  matrixid=KB_NU
HOME:        row=6, col=6, id=96,  matrixid=KB_HO
APLET:       row=0, col=7, id=105, matrixid=KB_AP
VIEWS:       row=1, col=7, id=106, matrixid=KB_VI
UP:          row=0, col=6, id=90,  matrixid=KB_UP
LEFT:        row=1, col=6, id=91,  matrixid=KB_DN
DOWN:        row=2, col=6, id=92,  matrixid=KB_LF
RIGHT:       row=3, col=6, id=93,  matrixid=KB_RT
VARS (A):    row=0, col=4, id=60,  matrixid=KB_A
MATH (B):    row=0, col=3, id=45,  matrixid=KB_B
d/dx (C):    row=0, col=2, id=30,  matrixid=KB_C
T,X,θ (D):   row=0, col=1, id=15,  matrixid=KB_D
DEL:         row=0, col=1, id=0,   matrixid=KB_DEL
SIN (E):     row=1, col=4, id=61,  matrixid=KB_E
COS (F):     row=1, col=3, id=46,  matrixid=KB_F
TAN (G):     row=1, col=2, id=31,  matrixid=KB_G
LN (H):      row=1, col=1, id=16,  matrixid=KB_H
LOG (I):     row=1, col=0, id=1,   matrixid=KB_I
Sqrt(X) (J): row=2, col=4, id=62,  matrixid=KB_J
Y^X (K):     row=2, col=3, id=47,  matrixid=KB_K
( (L):       row=2, col=2, id=32,  matrixid=KB_L
) (M):       row=2, col=1, id=17,  matrixid=KB_M
/ (N):       row=2, col=0, id=2,   matrixid=KB_DIV
COMMA(,)(O): row=3, col=4, id=63,  matrixid=KB_O
7 (P):       row=3, col=3, id=48,  matrixid=KB_7
8 (Q):       row=3, col=2, id=33,  matrixid=KB_8
9 (R):       row=3, col=1, id=18,  matrixid=KB_9
* (S):       row=3, col=0, id=3,   matrixid=KB_MUL
ALPHA:       row=4, col=4, id=64,  matrixid=KB_AL
4:           row=4, col=3, id=49,  matrixid=KB_4
5:           row=4, col=2, id=34,  matrixid=KB_5
6:           row=4, col=1, id=19,  matrixid=KB_6
-:           row=4, col=0, id=4,   matrixid=KB_SUB
SHIFT:       row=5, col=4, id=65,  matrixid=KB_SF
1:           row=5, col=3, id=50,  matrixid=KB_1
2:           row=5, col=2, id=35,  matrixid=KB_2
3:           row=5, col=1, id=20,  matrixid=KB_3
+:           row=5, col=0, id=5,   matrixid=KB_ADD
ON:          row=6, col=4, id=66,  matrixid=63 KB_ON
0:           row=6, col=3, id=51,  matrixid=KB_0
.:           row=6, col=2, id=36,  matrixid=KB_DOT
(-):         row=6, col=1, id=21,  matrixid=KB_MIN
ENTER:       row=6, col=0, id=6,   matrixid=KB_ENT
*/

typedef union {
	ULONGLONG full;
	unsigned int words[2];
	unsigned char bytes[8];
} keymatrix;
void keyb_getmatrix(keymatrix *m);
int get_1st_key(int wait);


#define modifier_key_pressed(bit) ((*GPFDAT >> (bit)) & 1)
#define on_pressed    modifier_key_pressed(0)
#define comma_pressed modifier_key_pressed(4)
#define alpha_pressed modifier_key_pressed(5)
#define shift_pressed modifier_key_pressed(6)
#define hp49_alpha_pressed comma_pressed
#define hp49_ls_pressed alpha_pressed
#define hp49_rs_pressed shift_pressed

#define any_normal_key_pressed ((*GPGDAT & 0xFE) != 0xFE)
#define any_key_pressed ( \
	any_normal_key_pressed || on_pressed || \
	comma_pressed || alpha_pressed || shift_pressed \
)

int get_key(void);

#endif

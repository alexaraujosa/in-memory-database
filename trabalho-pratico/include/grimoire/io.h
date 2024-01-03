// NOTE: Keypresses cannot be captured, only character sequences. Modifier keys are not sequences.

#ifndef TP_GRIMOIRE_IO_H
#define TP_GRIMOIRE_IO_H

#include <stdint.h>
#include <ctype.h>

#include "grimoire/grimoire_public.h"

// TODO: Add HOME, etc.
#define GM_KEY_NUL 0
#define GM_KEY_ENTER 10
#define GM_KEY_ESCAPE 27
#define GM_KEY_SPACE 32
#define GM_KEY_BACKSPACE 127
#define GM_KEY_DELETE 177

#define GM_KEY_F1  0xFF + 1 
#define GM_KEY_F2  0xFF + 2 
#define GM_KEY_F3  0xFF + 3 
#define GM_KEY_F4  0xFF + 4 
#define GM_KEY_F5  0xFF + 5 
#define GM_KEY_F6  0xFF + 6 
#define GM_KEY_F7  0xFF + 7 
#define GM_KEY_F8  0xFF + 8 
#define GM_KEY_F9  0xFF + 9 
#define GM_KEY_F10 0xFF + 10
#define GM_KEY_F11 0xFF + 11
#define GM_KEY_F12 0xFF + 12

#define GM_KEY_ARROW_UP    0xFF + 13
#define GM_KEY_ARROW_DOWN  0xFF + 14
#define GM_KEY_ARROW_RIGHT 0xFF + 15
#define GM_KEY_ARROW_LEFT  0xFF + 16

// WARN: Non-exaustive list
#define GM_KEY_CTRL_A 1
#define GM_KEY_CTRL_B 2
#define GM_KEY_CTRL_C 3
#define GM_KEY_CTRL_D 4
#define GM_KEY_CTRL_E 5
#define GM_KEY_CTRL_F 6
#define GM_KEY_CTRL_G 7
#define GM_KEY_CTRL_H 8
#define GM_KEY_CTRL_I 9
#define GM_KEY_CTRL_J 10
#define GM_KEY_CTRL_K 11
#define GM_KEY_CTRL_L 12
#define GM_KEY_CTRL_M 13
#define GM_KEY_CTRL_N 14
#define GM_KEY_CTRL_O 15
#define GM_KEY_CTRL_P 16
#define GM_KEY_CTRL_Q 17
#define GM_KEY_CTRL_R 18
#define GM_KEY_CTRL_S 19
#define GM_KEY_CTRL_T 20
#define GM_KEY_CTRL_U 21
#define GM_KEY_CTRL_V 22
#define GM_KEY_CTRL_W 23
#define GM_KEY_CTRL_X 24
#define GM_KEY_CTRL_Y 25
#define GM_KEY_CTRL_Z 26

#define GM_KEY_IS_ALPHANUMERIC(key) (isalnum(key) || (key) == ' ')
#define GM_KEY_IS_PRINTABLE(key) (isprint(key) || (key) == ' ')
// #define GM_KEY_IS_FUNCTION(key) (key > 255 && key < 12)
#define GM_KEY_IS_FUNCTION(key) ((int)(c) >= GM_KEY_F1 && (int)(c) <= GM_KEY_F12)
#define GM_KEY_IS_ARROW(key) ((int)(c) >= GM_KEY_ARROW_UP && (int)(c) <= GM_KEY_ARROW_LEFT)
#define GM_IS_CTRL(key) ((int)(c) >= GM_KEY_CTRL_A && (int)(c) <= GM_KEY_CTRL_Z)

int kbhit();
int getch();
uint16_t get_sequence();

void gm_hide_cursor();
void gm_show_cursor();

#endif
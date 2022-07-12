/*
* Cards
* Copyright (C) Zach Wilder 2022
* 
* This file is a part of Cards
*
* Cards is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* Cards is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with Cards.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef DRAW_H
#define DRAW_H

/*********
 * Globals
 *********/
extern int g_screenW;
extern int g_screenH;
/******************
 * draw.c functions
 ******************/
void scr_init(void);
void scr_restore(void);
void scr_reset(void);
void scr_clear(void);
void scr_pt_char(int x, int y, char c);
void scr_pt_clr_char(int x, int y, uint8_t fg, uint8_t bg, char c);
void scr_pt(int x, int y, char *fstr,...);
void scr_pt_clr(int x, int y, uint8_t fg, uint8_t bg, char *fstr,...);
void scr_set_clr(uint8_t fg, uint8_t bg);
void scr_set_curs(int x, int y);
void scr_set_style(int style);

/*******
 * Flags
 *******/
typedef enum {
    ST_NONE         = 1 << 0,
    ST_ULINE        = 1 << 1,
    ST_ITALIC       = 1 << 2,
    ST_BLINK        = 1 << 3,
    ST_BOLD         = 1 << 4,
    ST_STRIKE       = 1 << 5,
    ST_DIM          = 1 << 6
} StyleFlags;

#endif

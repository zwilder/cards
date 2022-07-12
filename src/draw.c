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
#include <cards.h>

int g_screenW = 0;
int g_screenH = 0;

void scr_init(void) {
    struct winsize ws;
    ioctl(0,TIOCGWINSZ,&ws);
    g_screenW = ws.ws_col;
    g_screenH = ws.ws_row;
    printf("\x1b[?1049h"); //Alternate buffer
    printf("\x1b[?25l"); //Hides cursor (l = low,0)
    scr_reset();
    scr_clear();
}

void scr_restore(void) {
    scr_reset();
    scr_clear();
    printf("\x1b[?1049l");
    printf("\x1b[?25h"); //Show cursor (h = high,1)
}

void scr_reset(void) {
    printf("\x1b[0m"); //Reset to default
    fflush(stdout);
}

void scr_clear(void) {
    printf("\x1b[H\x1b[J"); //?
    fflush(stdout);
}

void scr_pt_char(int x, int y, char c) {
    printf("\x1b[%d;%dH%c", y+1,x+1,c); //coordinates start at 1,1
    fflush(stdout);
}

void scr_pt_clr_char(int x, int y, uint8_t fg, uint8_t bg, char c) {
    scr_set_clr(fg,bg);
    scr_pt_char(x,y,c);
    scr_reset();
}

void scr_pt(int x, int y, char *fstr,...) {
    va_list args;
    va_start(args,fstr);
    scr_set_curs(x,y);
    vprintf(fstr, args);
    fflush(stdout);
    va_end(args);
}

void scr_pt_clr(int x, int y, uint8_t fg, uint8_t bg, char *fstr,...) {
    va_list args;
    va_start(args, fstr);
    scr_set_curs(x,y);
    scr_set_clr(fg,bg);
    vprintf(fstr, args);
    scr_reset();
    fflush(stdout);
    va_end(args);
}

void scr_set_clr(uint8_t fg, uint8_t bg) {
    printf("\x1b[38;5;%dm\x1b[48;5;%dm",fg,bg);
    fflush(stdout);
}

void scr_set_curs(int x, int y) {
    printf("\x1b[%d;%dH",y+1,x+1);
}

void scr_set_style(int style) {
    if((style & ST_BOLD) == ST_BOLD) {
        printf("\x1b[1m");
    }
    if((style & ST_DIM) == ST_DIM) {
        printf("\x1b[2m");
    }
    if((style & ST_ITALIC) == ST_ITALIC) {
        printf("\x1b[3m");
    }
    if((style & ST_ULINE) == ST_ULINE) {
        printf("\x1b[4m");
    }
    if((style & ST_BLINK) == ST_BLINK) {
        printf("\x1b[5m");
    }
    if((style & ST_STRIKE) == ST_STRIKE) {
        printf("\x1b[9m");
    }
    if((style & ST_NONE) == ST_NONE) {
        printf("\x1b[0m");
    }
}

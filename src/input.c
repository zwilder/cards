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

struct termios g_oldkbflags;

void kb_init(void) {
    struct termios newkbflags;
    if(tcgetattr(0, &g_oldkbflags)>=0) {
        tcgetattr(0, &newkbflags);
        newkbflags.c_lflag &= ~ICANON;
        newkbflags.c_lflag &= ~ECHO;
        newkbflags.c_cc[VMIN] = 1;
        newkbflags.c_cc[VTIME] = 0;
        tcsetattr(0, TCSANOW, &newkbflags);
    }
}

void kb_restore(void) {
    tcsetattr(0,TCSADRAIN,&g_oldkbflags);
}

char kb_get_char(void) {
    char c;
    read(STDIN_FILENO,&c,1);
    return c;
}

char* kb_get_str(void) {
    char* input = malloc(1000 * sizeof(char));
    kb_restore();
    printf("\x1b[?25h\x1b[3 q");
    scanf("%[^\n]s",input);
    printf("\x1b[?25l\x1b[0 q");
    kb_init();
    return input;
}

char* kb_get_str_at(int x, int y) {
    scr_set_curs(x,y);
    return kb_get_str();
}

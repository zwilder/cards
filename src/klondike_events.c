/*
* Cards
* Copyright (C) Zach Wilder 2022-2024
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

bool klondike_events(void) {
    int running = true;
    int input = kb_get_char();
    switch(input) {
        case 'q': running = false; break;
        case 'a': klondike_toggle_btn(g_klondike->btns[0]); break;
        case 'b': klondike_toggle_btn(g_klondike->btns[1]); break;
        case 'c': klondike_toggle_btn(g_klondike->btns[2]); break;
        case 'd': klondike_toggle_btn(g_klondike->btns[3]); break;
        case 'e': klondike_toggle_btn(g_klondike->btns[4]); break;
        case 'f': klondike_toggle_btn(g_klondike->btns[5]); break;
        case 'g': klondike_toggle_btn(g_klondike->btns[6]); break;
        case 'h': klondike_toggle_btn(g_klondike->btns[7]); break;
        case 'i': klondike_toggle_btn(g_klondike->btns[8]); break;
        case 'j': klondike_toggle_btn(g_klondike->btns[9]); break;
        case 'k': klondike_toggle_btn(g_klondike->btns[10]); break;
        case 'l': klondike_toggle_btn(g_klondike->btns[11]); break;
        case 'm': klondike_toggle_btn(g_klondike->btns[12]); break;
        default: break;
    }
    return running;
}

void klondike_deactivate_btns(void) {
    int i = 0;
    for(i = 0; i < g_klondike->num_btns; i++) {
        g_klondike->btns[i]->selected = false;
    }
}

int klondike_count_selected_btns(void) {
    int i = 0;
    int result = 0;
    for(i = 0; i < g_klondike->num_btns; i++) {
        if(g_klondike->btns[i]->selected) result++;
    }
    return result;
}

void klondike_toggle_btn(Button *btn) {
    if(!btn->active) return;
    btn->selected = !(btn->selected);
}

void klondike_msg(char *msg,...) {
    if(g_klondike->msg) {
        free(g_klondike->msg);
        g_klondike->msg = NULL;
    }
    if(!msg) return;
    va_list args;
    va_start(args,msg);
    int i = strlen(msg) + 1;
    g_klondike->msg = malloc(sizeof(char) * i);
    //g_klondike->msg = strncpy(g_klondike->msg, msg, i);
    vsnprintf(g_klondike->msg,i,msg,args);
    va_end(args);
}

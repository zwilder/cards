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

void cribbage_update(void) {
    int i;
    for(i = 0; i < 6; ++i) {
        g_cribbage->btns[i]->active = false;
    }
    if(check_flag(g_cribbage->flags, CRIB_CGO | CRIB_PGO)) {
        remove_flag(&g_cribbage->flags, CRIB_CGO);
        remove_flag(&g_cribbage->flags, CRIB_PGO);
        cribbage_change_state(CRIB_STSHOW);
    }
}

void new_cribbage_round(void) {
    add_cards(&g_cribbage->ctable, &g_cribbage->deck);
    add_cards(&g_cribbage->ptable, &g_cribbage->deck);
    add_cards(&g_cribbage->crib, &g_cribbage->deck);
    add_cards(&g_cribbage->computer->hand, &g_cribbage->deck);
    add_cards(&g_cribbage->player->hand, &g_cribbage->deck);
    draw_cards(&g_cribbage->deck, &g_cribbage->computer->hand, 6);
    draw_cards(&g_cribbage->deck, &g_cribbage->player->hand, 6);
    cribbage_change_state(CRIB_STDISCARD);
}

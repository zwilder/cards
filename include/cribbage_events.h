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
#ifndef CRIBBAGE_EVENTS_H
#define CRIBBAGE_EVENTS_H

bool cribbage_events(void);
void crib_sel_btn(void);
void crib_tog_btn(int btn);
void crib_reset_btns(void);
int crib_count_sel_btns(void);
Deck* crib_get_card_btn(int btn);
void cribbage_computer_events(void);
void cribbage_discard_events(void);
void cribbage_player_turn(void);

#endif

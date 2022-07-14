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
#ifndef CRIBBAGE_DRAW_H
#define CRIBBAGE_DRAW_H

void cribbage_draw(void);
void cribbage_draw_cards(int fg,int bg,int xo,int yo);
void cribbage_draw_buttons(int xo,int yo);
void cribbage_draw_score(int fg, int bg, int xo, int yo);
void cribbage_draw_peg(int x, int y, int fg, int bg, int sc);
void cribbage_draw_board(int fg, int bg, int xo, int yo);

#endif

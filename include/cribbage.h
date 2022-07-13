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
#ifndef CRIBBAGE_H
#define CRIBBAGE_H

typedef struct {
    Deck *hand;
    uint8_t score; //Total player score
    uint8_t pegA; //Score location of peg
    uint8_t pegB;
    uint8_t strategy; //0 none, 1 easy, 2 standard, 3 pro
} CribPlayer;

typedef struct {
    uint8_t qty;
    uint8_t pts;
    char *msg;
} CribScore;
/**********************
 * cribbage.c functions
 **********************/
CribPlayer* create_cribbage_player(void);
void destroy_cribbage_player(CribPlayer **player); 
void cribbage_init(void);
void cribbage_cleanup(void);
void cribbage_loop(void);
bool cribbage_events(void);
void cribbage_draw(void);
void draw_score(int fg, int bg, int xo, int yo);
void draw_peg(int x, int y, int fg, int bg, int sc);
void draw_board(int fg, int bg, int xo, int yo);
void cribbage_update(void);

CribScore* create_cribscore(int qty, int pts, char *msg,...);
void destroy_cribscore(CribScore *score);

int get_cribbage_card_value(int card);
int score_cribbage_hand(Deck *hand, Deck *flop);
CribScore* count_runs(Deck *hand, Deck *flop);
CribScore* count_flush(Deck *hand, Deck *flop);
CribScore* count_15s(Deck *hand, Deck *flop);
CribScore* count_pairs(Deck *hand, Deck *flop);
CribScore* count_nobs(Deck *hand, Deck *flop);


#endif

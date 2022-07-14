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

typedef struct {
    char ch;
    bool active;
    bool selected;
    int num;
    int x;
    int y;
} Button;

typedef struct {
    CribPlayer *computer;
    CribPlayer *player;

    Deck *deck; // Standard 52 card deck
    Deck *ctable; // Cards in front of the cpu
    Deck *ptable; // Cards in front of the player
    Deck *crib; // Cards in the crib

    int flags;
    Button **btns;

} Cribbage;
    
typedef enum {
    CRIB_STNONE     = 1 << 0,
    CRIB_STCUT      = 1 << 1,
    CRIB_STDISCARD  = 1 << 2,
    CRIB_STSHOW     = 1 << 3,
    CRIB_PCRIB      = 1 << 4,
    CRIB_CCRIB      = 1 << 5,
    CRIB_PTURN      = 1 << 6,
    CRIB_CTURN      = 1 << 7,
    CRIB_EASY       = 1 << 8,
    CRIB_STD        = 1 << 9,
    CRIB_PRO        = 1 <<10
} CribFlags;
    
/**********************
 * cribbage.c functions
 **********************/
void cribbage_init(void);
void cribbage_cleanup(void);
void cribbage_loop(void);
void cribbage_change_state(int st);
bool cribbage_events(void);
void cribbage_draw(void);
void cribbage_draw_cards(int fg,int bg,int xo,int yo);
void cribbage_draw_buttons(int xo,int yo);
void cribbage_draw_score(int fg, int bg, int xo, int yo);
void cribbage_draw_peg(int x, int y, int fg, int bg, int sc);
void cribbage_draw_board(int fg, int bg, int xo, int yo);
void cribbage_update(void);
void cribbage_discard_events(void);
void new_cribbage_round(void);

Button* create_button(char ch, int num); 
void toggle_button(int btn);
int count_selected_buttons(void);
Deck* get_card_at_button(int btn);

CribPlayer* create_cribbage_player(void);
void destroy_cribbage_player(CribPlayer **player); 

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

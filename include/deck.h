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
#ifndef DECK_H
#define DECK_H

#define CARD_WIDTH 6
#define CARD_HEIGHT 6

typedef enum {
    CD_NONE     = 1 << 0,
    CD_A        = 1 << 1,
    CD_2        = 1 << 2,
    CD_3        = 1 << 3,
    CD_4        = 1 << 4,
    CD_5        = 1 << 5,
    CD_6        = 1 << 6,
    CD_7        = 1 << 7,
    CD_8        = 1 << 8,
    CD_9        = 1 << 9,
    CD_10       = 1 << 10,
    CD_J        = 1 << 11,
    CD_Q        = 1 << 12,
    CD_K        = 1 << 13,
    CD_H        = 1 << 14,
    CD_S        = 1 << 15,
    CD_C        = 1 << 16,
    CD_D        = 1 << 17,
    CD_UP       = 1 << 18
} DeckFlags;

struct Deck {
    int card;
    struct Deck *next;
};
typedef struct Deck Deck;

/*************************
 * Deck printing functions
 *************************/
char* get_suite(int card);
char get_suite_ch(int card);
int get_value(int card);
char* get_value_str(int card);
int get_card(int value, char suite);
void pt_card_at(int x, int y, int card);
void pt_card_clr_at(int x, int y, int card);
void pt_card_back_at(int x, int y);
void pt_card_back_clr_at(int x, int y, int color);
void pt_deck_stack_at(int x, int y);
void pt_deck_stack_clr_at(int x, int y, int color);
void pt_deck_stackv_at(int x, int y);
void pt_deck_stackv_clr_at(int x, int y, int color);
void pt_card_spc_at(int x, int y, char c, char *suite);
void pt_card_spc_clr_at(int x, int y, char c, char *suite, uint8_t fg, uint8_t bg); 
void pt_card_active(int x, int y);

/****************
 * Deck functions
 ****************/
Deck* create_card(int card);
Deck* find_card(Deck **headref, int card);
Deck* get_last_card(Deck *headref);
Deck* get_card_at(Deck **headref, int n);
Deck* remove_card(Deck **headref, Deck *card);
void move_card_to_back(Deck **todeck, Deck **fromdeck, Deck *card);
void move_card_to(Deck **todeck, Deck **fromdeck, Deck *card);
void shuffle_deck(Deck **headref, int rounds);
void push_card(Deck **headref, Deck *card);
void push_card_back(Deck **headref, Deck *card);
void destroy_deck(Deck **headref);
int count_deck(Deck *cards);
void merge_sort_deck(Deck **headref, bool bysuite);
Deck* sorted_merge(Deck *a, Deck *b, bool bysuite);
void ft_bk_splt(Deck *source, Deck **frontref, Deck **backref);

Deck* create_std_deck(void);
void draw_card(Deck **from, Deck **to);
void draw_cards(Deck **from, Deck **to, int n);
void draw_card_back(Deck **from, Deck **to);
void draw_cards_back(Deck **from, Deck **to, int n);
void add_cards(Deck **from, Deck **to); 

/*******************
 * Card Interactions
 *******************/
bool card_hearts(int card);
bool card_diamonds(int card);
bool card_spades(int card);
bool card_clubs(int card);
bool card_black(int card);
bool card_red(int card);
bool card_alt_color(int a, int b);
bool card_same_suit(int a, int b);
bool card_in_asc_sequence(int a, int b);
bool card_in_dec_sequence(int a, int b);
void turn_deck_facedown(Deck *deck);
bool card_faceup(int card);
bool card_facedown(int card);

#endif

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

#ifndef KLONDIKE_H
#define KLONDIKE_H

typedef struct {
    Deck *deck; // Standard 52 card deck
    Deck *waste; // "Waste" pile, drawn off the deck
    Deck **tableau; // Tableaus, 7 of them
    Deck **foundation; // Foundations, 4 of them (0H,1D,2C,3S)
    Deck *fromref; 
    Deck *toref; 
    int flags;
    int num_btns;
    Button **btns;
    uint8_t deckcolor;
    char *msg;
} Klondike;

extern Klondike* g_klondike;
/**********************
 * klondike.c functions
 **********************/
void klondike_init(void);
void klondike_deal(void);
void klondike_cleanup(void);
void klondike_loop(void);

/**********************
 * klondike_events.c
 *********************/
bool klondike_events(void);
void klondike_deactivate_btns(void);
void klondike_toggle_btn(Button *btn);
int klondike_count_selected_btns(void);
void klondike_msg(char *msg,...);

/**********************
 * klondike_update.c
 **********************/
void klondike_update(void);
Deck* klondike_btn_to_deck(Button *btn);
bool klondike_deck_is_tableau(Deck *deck);
bool klondike_btn_is_tableau(Button *btn);
bool klondike_deck_is_foundation(Deck *deck);
bool klondike_btn_is_foundation(Button *btn);
void klondike_automove(void);

/**********************
 * klondike_draw.c
 **********************/
void klondike_draw(void);
void klondike_draw_space(int x, int y);
void klondike_draw_btn(Button *btn, int x, int y);

#endif //KLONDIKE_H

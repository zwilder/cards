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

void klondike_update(void) {
    // Count the buttons, if there are exactly two then we need to move cards
    // we need to keep track of the tableau it responds to
    Deck *card = NULL;
    Button *btn = g_klondike->btns[0];
    int btncount = klondike_count_selected_btns();
    int i = 0;
    if ((1 == btncount) && !g_klondike->fromref) {
        //Only one deck selected, store that as our "from" reference
        while(!btn->selected) {
            i++;
            btn = g_klondike->btns[i];
        }
        g_klondike->fromref = klondike_btn_to_deck(btn);
    } else if (2 == btncount) {
        //Exactly two buttons active, move card from "fromref" to "toref"
        //Figure out what toref is
        while((!btn->selected) || klondike_btn_to_deck(btn) == g_klondike->fromref) {
            i++;
            btn = g_klondike->btns[i];
        }
        // Cant move cards to the waste or stock, so check to make sure toref is
        // neither of those
        if(btn->ch == 'a' || btn->ch == 'm') {
            // Second button pressed is an illegal move, ignore it
            g_klondike->toref = NULL;
        } else {
            g_klondike->toref = klondike_btn_to_deck(btn);
        }
        //check validity of move (eventually)
        //move card
        if(g_klondike->fromref && g_klondike->toref) {
            // Get last card of fromref
            card = get_last_card(g_klondike->fromref);
            // Remove that card from fromref
            // Push that card to the back of toref
            if(card) {
                move_card_to_back(&g_klondike->toref, &g_klondike->fromref, card);
            }
        }
        //Flip over card that was under the card moved, if it isn't already up
        i = count_deck(g_klondike->fromref) - 1;
        card = get_card_at(&g_klondike->fromref, i);
        if(card) {
            engage_flag(&(card->card), CD_UP);
        }
        //deactivate buttons
        klondike_deactivate_btns();
        // NULL references
        g_klondike->fromref = NULL;
        g_klondike->toref = NULL;
    }
    //if btncount is 1, and fromref is the deck, then we need to draw three
    //cards from the stock and put it on the waste
    if(g_klondike->fromref == g_klondike->deck) {
        klondike_deactivate_btns();
        g_klondike->fromref = NULL;
        draw_cards_back(&g_klondike->deck, &g_klondike->waste, 3);
        card = g_klondike->waste;
        while(card) {
            engage_flag(&(card->card), CD_UP);
            card = card->next;
        }
    }
}

Deck* klondike_btn_to_deck(Button *btn) {
    Deck *result = NULL;
    switch(btn->num) {
        case 0: result = g_klondike->waste; break; // a
        case 1: result = g_klondike->tableau[0]; break; // b
        case 2: result = g_klondike->tableau[1]; break; // c
        case 3: result = g_klondike->tableau[2]; break; // d
        case 4: result = g_klondike->tableau[3]; break; // e
        case 5: result = g_klondike->tableau[4]; break; // f
        case 6: result = g_klondike->tableau[5]; break; // g
        case 7: result = g_klondike->tableau[6]; break; // h
        case 8: result = g_klondike->foundation[0]; break; // i
        case 9: result = g_klondike->foundation[1]; break; // j
        case 10: result = g_klondike->foundation[2]; break; // k
        case 11: result = g_klondike->foundation[3]; break; // l
        case 12: result = g_klondike->deck; break; // m, move 3 cards from stock to waste
        default: break;
    }
    return result;
}

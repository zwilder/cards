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
    Deck *card = NULL, *tocard = NULL;
    Button *btn = g_klondike->btns[0];
    int btncount = klondike_count_selected_btns();
    int i = 0;
    klondike_automove();
    if ((1 == btncount) && !g_klondike->fromref) {
        //Only one deck selected, store that as our "from" reference
        while(!btn->selected) {
            i++;
            btn = g_klondike->btns[i];
        }
        if(klondike_btn_is_foundation(btn)) {
            // Can't move cards from a foundation
            klondike_deactivate_btns();
            g_klondike->fromref = NULL;
            //klondike_msg("Can't move cards from a foundation.");
        } else {
            g_klondike->fromref = klondike_btn_to_deck(btn);
        }
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
            //klondike_msg("Can't move cards to the waste/stock.");
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
                tocard = get_last_card(g_klondike->toref);
                if(klondike_deck_is_tableau(g_klondike->toref)) {
                    // Moving card to tableau, make sure card is:
                    // - Alternate color
                    // - Descending sequence
                    if(card_alt_color(tocard->card,card->card) &&
                            card_in_dec_sequence(card->card,tocard->card)) {
                        //Success, valid move
                        move_card_to_back(&g_klondike->toref, &g_klondike->fromref, card);
                        klondike_msg(NULL);
                    } else {
                        //Invalid move
                        klondike_msg("Invalid move.");
                    }
                }
                if(klondike_deck_is_foundation(g_klondike->toref)) {
                    //Trying to move card to a foundation, so is:
                    //- The suite of the foundation the same as the card?
                    //- The card in asc sequence with the last card on the
                    //  foundation?
                    if (!tocard) {
                        klondike_msg("Nothing in foundation.");
                    } else if(card_same_suit(card->card,tocard->card) &&
                            card_in_asc_sequence(card->card,tocard->card)) {
                        // Valid move
                    } else {
                        // Invalid move
                        klondike_msg("Invalid move.");
                    }
                }
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
        if(g_klondike->deck) {
            draw_cards_back(&g_klondike->deck, &g_klondike->waste, 3);
            card = g_klondike->waste;
            while(card) {
                engage_flag(&(card->card), CD_UP);
                card = card->next;
            }
        } else {
            // No cards in the deck, add the waste back to the deck
            // This **should** work, but adds the cards back in the wrong order
            add_cards_back(&g_klondike->waste, &g_klondike->deck);
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

bool klondike_deck_is_tableau(Deck *deck) {
    int i = 0;
    for(i = 0; i < 7; i++) {
        if(deck == g_klondike->tableau[i]) {
            return true;
        }
    }
    return false;
}

bool klondike_btn_is_tableau(Button *btn) {
    return (klondike_deck_is_tableau(klondike_btn_to_deck(btn)));
}

bool klondike_deck_is_foundation(Deck *deck) {
    int i = 0;
    for(i = 0; i < 4; i++) {
        if(deck == g_klondike->foundation[i]) {
            return true;
        }
    }
    return false;
}

bool klondike_btn_is_foundation(Button *btn) {
    return (klondike_deck_is_foundation(klondike_btn_to_deck(btn)));
}

void klondike_automove(void) {
    // check each tableau's last card, and if it can be moved to a foundation,
    // move it
    // Foundations, 4 of them (0H,1D,2C,3S)
    int i = 0;
    int cflags = 0;
    Deck *card = NULL;
    // Check tableaus
    for(i = 0; i < 7; i++) {
        card = get_last_card(g_klondike->tableau[i]); 
        if(!card) continue;
        cflags = card->card;
        if(check_flag(cflags, CD_H) && check_flag(cflags, CD_A)) {
            //move_card_to_back(&g_klondike->tableau[i], &g_klondike->foundation[0],card);
            //g_klondike->foundation[0] = remove_card(&g_klondike->tableau[i],card);
            draw_card(&g_klondike->tableau[i],&g_klondike->foundation[0]);
        }
        if(check_flag(cflags, CD_D) && check_flag(cflags, CD_A)) {
            //move_card_to_back(&g_klondike->tableau[i], &g_klondike->foundation[1],card);
            //g_klondike->foundation[1] = remove_card(&g_klondike->tableau[i],card);
            draw_card(&g_klondike->tableau[i],&g_klondike->foundation[1]);
        }
        if(check_flag(cflags, CD_C) && check_flag(cflags, CD_A)) {
            //move_card_to_back(&g_klondike->tableau[i], &g_klondike->foundation[2],card);
            //g_klondike->foundation[2] = remove_card(&g_klondike->tableau[i],card);
            draw_card(&g_klondike->tableau[i],&g_klondike->foundation[2]);
        }
        if(check_flag(cflags, CD_S) && check_flag(cflags, CD_A)) {
            //move_card_to_back(&g_klondike->tableau[i], &g_klondike->foundation[3],card);
            //g_klondike->foundation[3] = remove_card(&g_klondike->tableau[i],card);
            draw_card(&g_klondike->tableau[i],&g_klondike->foundation[3]);
        }
    }
    // Check waste
    /*
    card = get_last_card(g_klondike->waste);
    if(card) {
        cflags = card->card;
        if(check_flag(cflags, CD_H) && check_flag(cflags, CD_A)) {
            klondike_msg("Ace of Hearts on waste");
            move_card_to_back(&g_klondike->waste, &g_klondike->foundation[0],card);
        }
        if(check_flag(cflags, CD_D) && check_flag(cflags, CD_A)) {
            klondike_msg("Ace of Diamonds on waste");
            move_card_to_back(&g_klondike->waste, &g_klondike->foundation[1],card);
        }
        if(check_flag(cflags, CD_C) && check_flag(cflags, CD_A)) {
            klondike_msg("Ace of Clubs on waste");
            move_card_to_back(&g_klondike->waste, &g_klondike->foundation[2],card);
        }
        if(check_flag(cflags, CD_S) && check_flag(cflags, CD_A)) {
            klondike_msg("Ace of Spades on waste");
            move_card_to_back(&g_klondike->waste, &g_klondike->foundation[3],card);
        }
    }
    */
}

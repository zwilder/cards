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

bool cribbage_events(void) {
    bool running = true;
    if(check_flag(g_cribbage->flags, CRIB_CTURN)) {
        cribbage_computer_events();
        return running;
    }
    int input = kb_get_char();
    switch(input) {
        case 'q': running = false; break;
        case '1':
        case 'a': crib_tog_btn(5); break;
        case '2':
        case 's': crib_tog_btn(4); break;
        case '3':
        case 'd': crib_tog_btn(3); break;
        case '4':
        case 'f': crib_tog_btn(2); break;
        case '5':
        case 'g': crib_tog_btn(1); break;
        case '6':
        case 'h': crib_tog_btn(0); break;
        case 'n': new_cribbage_round(); break;
        case 'S': merge_sort_deck(&g_cribbage->player->hand,true); break; 
        case 'A': merge_sort_deck(&g_cribbage->player->hand,false); break; 
        case 10: crib_sel_btn(); break;
        default: break;
    }
    return running;
}

void crib_sel_btn(void) {
    int fl = g_cribbage->flags;
    if(check_flag(fl, CRIB_STDISCARD)) {
        cribbage_discard_events();
    }else if(check_flag(fl, CRIB_PTURN)) {
        cribbage_player_turn();
    }
    crib_reset_btns();
}

void crib_tog_btn(int btn) {
    if(!g_cribbage->btns[btn]->active) return;
    g_cribbage->btns[btn]->selected = !(g_cribbage->btns[btn]->selected);
}

void crib_reset_btns(void) {
    int i;
    for(i = 0; i < 6; i++) {
        g_cribbage->btns[i]->selected = false;
    }
}

int crib_count_sel_btns(void) {
    int i;
    int count = 0;
    for(i = 0; i < 6; i++) {
        if(g_cribbage->btns[i]->selected) count++;
    }
    return count;
}

Deck* crib_get_card_btn(int btn) {
    Deck *card = g_cribbage->player->hand;
    int i;
    int count = count_deck(card);
    for(i = 0; i < count; i++) {
        if(i == btn) break;
        card = card->next;
    }
    return card;
}

void cribbage_computer_events(void) {
    int i,count;
    Deck *hand = g_cribbage->computer->hand;
    Deck *card = NULL;
    if(check_flag(g_cribbage->flags,CRIB_CTURN)) {
        // Chose a card to lay down on the table
        count = count_deck(hand);
        if(count == 0) {
            engage_flag(&g_cribbage->flags,CRIB_CGO);
            return;
        }
        i = mt_rand(0,count-1);
        card = get_card_at(&hand, i);
        if(!card) return;
        // Put card on table
        move_card_to_back(&g_cribbage->ctable, &hand, card);
        //push_card_back(&g_cribbage->ctable, remove_card(&hand, card));
        // Update count and score points
        // Change to player turn
        cribbage_change_state(CRIB_PTURN);
    }
}

void cribbage_discard_events(void) {
    int i, cA, cB;
    int count = crib_count_sel_btns();
    Deck *cardA = NULL;
    Deck *cardB = NULL;
    if(count == 2) {
        // Move the two cards to the crib
        for(i = 0; i < 6; i++) {
            if(g_cribbage->btns[i]->selected) {
                if(!cardA) {
                    cardA = crib_get_card_btn(i);
                } else {
                    cardB = crib_get_card_btn(i);
                }
            }
        }
        cardA = remove_card(&g_cribbage->player->hand, cardA);
        cardB = remove_card(&g_cribbage->player->hand, cardB);
        push_card(&g_cribbage->crib, cardA);
        push_card(&g_cribbage->crib, cardB);
        // Have the computer move two cards (random for now)
        // Should be it's own state when the computer has a strategy
        cA = mt_rand(0,5);
        cardA = get_card_at(&g_cribbage->computer->hand,cA);
        cB = mt_rand(0,4);
        cardB = get_card_at(&g_cribbage->computer->hand,cB);
        push_card(&g_cribbage->crib, 
                remove_card(&g_cribbage->computer->hand, cardA));
        push_card(&g_cribbage->crib, 
                remove_card(&g_cribbage->computer->hand, cardB));

        engage_flag(&g_cribbage->flags, CRIB_PCRIB); //TEMP, should be set by cut
        if(check_flag(g_cribbage->flags, CRIB_PCRIB)) {
            cribbage_change_state(CRIB_CTURN);
        } else {
            cribbage_change_state(CRIB_PTURN);
        }
    } else {
        //Message to "Only select two, please."
        //scr_pt(g_screenW/2 - 40, g_screenH + 12, "> Only select two, please.");
        //snprintf(g_cribbage->msg, 80, "> Only select two, please.");
    }
}

void cribbage_player_turn(void) {
    int i;
    int count = crib_count_sel_btns();
    Deck *hand = g_cribbage->player->hand;
    Deck *card = NULL;
    if(count == 1) {
        // Get card
        for(i = 0; i < 6; i++) {
            if(g_cribbage->btns[i]->selected) {
                card = crib_get_card_btn(i);
                break;
            }
        }
        if(!card) return;
        // Put on table
        move_card_to_back(&g_cribbage->ptable, &hand, card);
        //push_card_back(&g_cribbage->ptable, remove_card(&hand, card));
        // Update score, pegs, and count
        // Computer turn
        cribbage_change_state(CRIB_CTURN);
    }else if(count_deck(hand) == 0) {
        engage_flag(&g_cribbage->flags,CRIB_PGO);
        return;
    }
}

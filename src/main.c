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

void move_card_temp(Deck **deckto,Deck **deckfrom, Deck* card) {
    Deck *tmp = *deckfrom;
    while(tmp && tmp != card) {
        tmp = tmp->next;
    }
    if(!tmp) return;
    tmp = remove_card(deckfrom, card);
    push_card_back(deckto, tmp);
}

int main(int argc, char **argv) {
    scr_init();
    kb_init();
    init_genrand(time(NULL));

    Deck *deck = create_std_deck();
    Deck *hand = NULL;
    Deck *flop = NULL;
    Deck *tmp = NULL;
    bool running = true;
    char input = 0;
    int x = g_screenW / 2;
    int y = g_screenH / 2;
    y -= 3;
    pt_card_back_clr_at(x - 18,y,43);
    pt_card_spc_clr_at(x - 12,y,'C',"\u2660",250,0);
    pt_card_spc_clr_at(x - 6,y,'a',"\u2665",203,0);
    pt_card_spc_clr_at(x,y,'r',"\u2666",250,0);
    pt_card_spc_clr_at(x + 6,y,'d',"\u2663",203,0);
    pt_card_spc_clr_at(x + 12,y,'s',"\u2660",250,0);
    pt_card_back_clr_at(x + 18,y,43);
    scr_pt(0,g_screenH - 1,"Deck Size: %d. Press q to exit, r to shuffle deck, d to draw card.", count_deck(deck));
    while(running) {
        // Handle events
        input = kb_get_char();
        // Update
        switch (input) {
            case 'q':
                running = false;
                break;
            case 'r':
            case 'd':
                add_cards(&hand, &deck);
                add_cards(&flop, &deck);
                shuffle_deck(&deck, 500);
                draw_cards(&deck, &hand, 4);
                draw_card(&deck, &flop);
                break;
            case 's':
                merge_sort_deck(&hand,false);
                break;
            case 'a':
                merge_sort_deck(&hand,true);
                break;
            case 'p':
                move_card_to_back(&flop, &hand, hand); 
                break;
            case 'c':
                cribbage_init();
                break;
            case 'k':
                klondike_init();
                break;
            default:
                break;
        }
        // Draw
        if(!running) break;
        x = 7;
        y = 0;
        scr_clear();
        if(hand) {
            tmp = hand;
            while(tmp) {
                pt_card_clr_at(x,y,tmp->card);
                x += 6;
                if(x >= 78) {
                    x = 7;
                    y += 6;
                }
                tmp = tmp->next;
            }
        }
        x = count_deck(deck);
        if(x > 1) {
            pt_deck_stack_clr_at(0,0,43);
        } else if (x == 1) {
            pt_card_back_clr_at(0,0,43);
        }
        /*
        if(hand && flop) {
            score_cribbage_hand(hand, flop);
        }
        */
        if(flop) {
            pt_card_clr_at(0,6,flop->card);
        }
        scr_pt(0,g_screenH - 1,"Deck Size: %d. Press q to exit, r to shuffle deck, d to draw card.", count_deck(deck));
    }

    scr_clear();

    destroy_deck(&flop);
    destroy_deck(&deck);
    destroy_deck(&hand);
    scr_restore();
    kb_restore();
    return 0;
}

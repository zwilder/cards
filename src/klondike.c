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

/* Reviving this project attempt after a year and a half - some good stuff in
 * here but NO DANG COMMENTS?! Past Zach is a jerk. Unfortunately, this means
 * I'm going to have to figure out why I did stuff the way I did to do new
 * stuff. I remember I stopped working on Cribbage because I was having weird
 * issues with trying to get the computer to take a turn after I took a turn - I
 * think that was mostly due to the keyboard handling which I later solved with
 * the Goblin Caves project. I'll look into that again. 
 *
 * Inspired by politaire.com - a fantastic solitaire card game collection - I
 * decided it would be fun to try to implement a couple versions of solitaire,
 * starting with Klondike of course.
 * */

/* Drawing reference
 * ♠ u2660, ♤ u2664
 * ♥ u2665, ♡ u2661
 * ♦ u2666, ♢ u2662
 * ♣ u2663, ♧ u2667
 *
 * deck at 1,1
 * waste at 2,6
 * tableau[0] at 13,1
 * tableau[1] at 20,1 
 * tableau[2] at 27,1
 * tableau[3] at 34,1
 * tableau[4] at 41,1
 * tableau[5] at 48,1
 * tableau[6] at 55,1
 * foundation[0] at 68,1
 * foundation[1] at 75,1
 * foundation[2] at 68,8
 * foundation[3] at 75,8
 * message at 23,0
 *
 * [m] at 2,0
 * [b] at 13,0
 * [c] at 20,0
 * [d] at 27,0
 *         1         2         3         4         5         6         7         8
  12345678901234567890123456789012345678901234567890123456789012345678901234567890
 1  [ m ]      [ b ]  [ c ]  [ d ]  [ e ]  [ f ]  [ g ]  [ h ]        [ i ]  [ j ]
 2 ╔╔╔═══╗     ♦═10╗  ╔═══╗  ╔═══╗  ╔═══╗  ╔═══╗  ╔═══╗  ╔═══╗        ╔   ╗  ╔   ╗  
 3 ║║║╔╗/║     ♠═9═╗  ♥═8═╗  ╔═══╗  ╔═══╗  ╔═══╗  ╔═══╗  ╔═══╗
 4 ║║║╚║╗║     ║   ║  ║   ║  ♦═8═╗  ╔═══╗  ╔═══╗  ╔═══╗  ╔═══╗          ♤      ♡
 5 ║║║/╚╝║     ║ 9 ║  ║ 8 ║  ║   ║  ♣═7═╗  ╔═══╗  ╔═══╗  ╔═══╗
 6 ╚╚╚═══╝     ║   ║  ║   ║  ║ 8 ║  ║   ║  ♠═10╗  ╔═══╗  ╔═══╗        ╚   ╝  ╚   ╝
 7  ♠═3═╗      ╚═9═♦  ╚═8═♥  ║   ║  ║ 7 ║  ║   ║  ♠═4═╗  ╔═══╗
 8  ♥═Q═╗                    ╚═8═♦  ║   ║  ║ 10║  ║   ║  ♥═5═╗        [ k ]  [ l ]         
 9  ♥═2═╗                           ╚═7═♣  ║   ║  ║ 4 ║  ║   ║        ╔   ╗  ╔   ╗
10  ║   ║                                  ╚═10♠  ║   ║  ║ 5 ║ 
 1  ║ 2 ║                                         ╚═4═♠  ║   ║          ♢      ♧
 2  ║   ║                                                ╚═5═♥
 3  ╚═2═♥                                                             ╚   ╝  ╚   ╝
 4  [ a ]
 5
 6
 7
 8
 9
20
 1
 2         
 3
 4 > How many cards would you like to move? 3         
 *   	0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F
U+255x	═	║	╒	╓	╔	╕	╖	╗	╘	╙	╚	╛	╜	╝	╞	╟
*/

/* Elements of Klondike
 * - Stock - deck of cards to draw from, three at a time. Adds to the...
 * - Waste - The "top" card of the waste pile can be moved to a tableau or
 *   foundation pile
 * - Tableaus - 7 of them, on the board. The bottom card can be moved to a tableau
 *   or foundation pile
 * - Foundations - 4 of them, at the top. Aces go here, then the next card in
 *   suite. 
 * UI Elements needed in addition
 * - A message/prompt line at the bottom. 
 * - 13 buttons
 */
#include <cards.h>

Klondike *g_klondike = NULL;

void klondike_init(void) {
    int i = 0;
    if(g_klondike) klondike_cleanup(); 
    g_klondike = malloc(sizeof(Klondike));
    g_klondike->deck = create_std_deck();
    g_klondike->waste = NULL;
    g_klondike->tableau = malloc (7 * sizeof(Deck));
    for(i = 0; i < 7; i++) {
        g_klondike->tableau[i] = NULL;
    }
    g_klondike->foundation = malloc (4 * sizeof(Deck));
    for(i = 0; i < 4; i++) {
        g_klondike->foundation[i] = NULL;
    }
    turn_deck_facedown(g_klondike->deck);

    g_klondike->num_btns = 13;
    g_klondike->btns = malloc(13 * sizeof(Button));
    g_klondike->btns[0] = create_button('a',0);
    g_klondike->btns[1] = create_button('b',1);
    g_klondike->btns[2] = create_button('c',2);
    g_klondike->btns[3] = create_button('d',3);
    g_klondike->btns[4] = create_button('e',4);
    g_klondike->btns[5] = create_button('f',5);
    g_klondike->btns[6] = create_button('g',6);
    g_klondike->btns[7] = create_button('h',7);
    g_klondike->btns[8] = create_button('i',8);
    g_klondike->btns[9] = create_button('j',9);
    g_klondike->btns[10] = create_button('k',10);
    g_klondike->btns[11] = create_button('l',11);
    g_klondike->btns[12] = create_button('m',12);
    for(i = 0; i < g_klondike->num_btns; i++) {
        g_klondike->btns[i]->active = true;
    }
    g_klondike->msg = NULL;

    g_klondike->deckcolor = 31; //Eventually there will be an option for this.
    klondike_deal();
    klondike_draw();
    klondike_loop();
}

void klondike_deal(void) {
    // Assign cards to tableaus
    // 0 - 1
    // 1 - 2
    // 2 - 3
    // 3 - 4
    // 4 - 5
    // 5 - 6
    // 6 - 7
    int i = 0, j = 0;
    Deck *tmp;
    for(i = 0; i < 7; i++) {
        j = i + 1;
        // Draw cards from the deck to put on the tableau
        draw_cards(&(g_klondike->deck), &(g_klondike->tableau[i]), j);

        // Turn the top card (last card) faceup
        tmp = g_klondike->tableau[i];
        while(tmp->next) {
            tmp = tmp->next;
        }
        engage_flag(&(tmp->card), CD_UP);
    }
}

void klondike_cleanup(void) {
    if(!g_klondike) return;
    int i;
    for(i = 0; i < g_klondike->num_btns; i++) {
        free(g_klondike->btns[i]);
        g_klondike->btns[i] = NULL;
    }
    free(g_klondike->btns);
    for(i = 0; i < 7; i++) {
        destroy_deck(&(g_klondike->tableau[i]));
        g_klondike->tableau[i] = NULL;
    }
    for(i = 0; i < 4; i++) {
        destroy_deck(&(g_klondike->foundation[i]));
        g_klondike->foundation[i] = NULL;
    }
    destroy_deck(&g_klondike->deck);
    destroy_deck(&g_klondike->waste);
    if(g_klondike->msg) {
        free(g_klondike->msg);
    }
    free(g_klondike);
    g_klondike = NULL;
}

void klondike_loop(void) {
    bool running = true;
    while(running) {
        running = klondike_events();
        klondike_update();
        // We could, possibly, have update return a bool and then only call draw
        // if the game actually updated? Or maybe have g_klondike carry a set of
        // flags that can be engaged/disengaged throughout the loop - one of
        // which will be a draw flag?
        klondike_draw();
    }
}

bool klondike_events(void) {
    int running = true;
    int input = kb_get_char();
    switch(input) {
        case 'q': running = false; break;
        case 'a': klondike_toggle_btn(g_klondike->btns[0]); break;
        case 'b': klondike_toggle_btn(g_klondike->btns[1]); break;
        case 'c': klondike_toggle_btn(g_klondike->btns[2]); break;
        case 'd': klondike_toggle_btn(g_klondike->btns[3]); break;
        case 'e': klondike_toggle_btn(g_klondike->btns[4]); break;
        case 'f': klondike_toggle_btn(g_klondike->btns[5]); break;
        case 'g': klondike_toggle_btn(g_klondike->btns[6]); break;
        case 'h': klondike_toggle_btn(g_klondike->btns[7]); break;
        case 'i': klondike_toggle_btn(g_klondike->btns[8]); break;
        case 'j': klondike_toggle_btn(g_klondike->btns[9]); break;
        case 'k': klondike_toggle_btn(g_klondike->btns[10]); break;
        case 'l': klondike_toggle_btn(g_klondike->btns[11]); break;
        case 'm': klondike_toggle_btn(g_klondike->btns[12]); break;
        default: break;
    }
    return running;
}

void klondike_deactivate_btns(void) {
    int i = 0;
    for(i = 0; i < g_klondike->num_btns; i++) {
        g_klondike->btns[i]->selected = false;
    }
}

int klondike_count_selected_btns(void) {
    int i = 0;
    int result = 0;
    for(i = 0; i < g_klondike->num_btns; i++) {
        if(g_klondike->btns[i]->selected) result++;
    }
    return result;
}

void klondike_toggle_btn(Button *btn) {
    if(!btn->active) return;
    btn->selected = !(btn->selected);
}

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

void klondike_draw(void) {
    // I need to revisit the drawing functions in this program, I updated and
    // improved a lot of things with Goblin Caves in 2023 since I wrote this in
    // 2022. 
    int xo = g_screenW / 2 - 25; //80x24
    int yo = g_screenH / 2 - 12;
    xo = 0; // The offsets are funky and I dont know why I did them that way. 
    yo = 0;
    //uint8_t fg = 172;
    //uint8_t bg = 238;
    int i = 0, x = 0, y = 0;
    Deck *tmp = NULL;
    Button *btn = NULL;

    // Clear the screen
    scr_clear();

    // Draw the stock/button
    btn = g_klondike->btns[12];
    i = count_deck(g_klondike->deck);
    if(i > 2) {
        pt_deck_stack_clr_at(1+xo,1+yo,g_klondike->deckcolor);
    } else if (i > 0) {
        pt_card_back_clr_at(1+xo,1+yo,g_klondike->deckcolor);
    }
    if(g_klondike->deck) {
        klondike_draw_btn(btn, 1+xo, 0+yo);
    }

    // Draw the tableaus/buttons
    for(i = 0; i < 7; i++) {
        tmp = g_klondike->tableau[i];
        x = 13 + (7 * i);
        y = 1;
        btn = g_klondike->btns[i+1];
        klondike_draw_btn(btn,x+xo,y+yo-1);
        if(count_deck(tmp) > 0) {
            while(tmp) {
                if(check_flag(tmp->card, CD_UP)) {
                    // Draw card face up
                    pt_card_clr_at(x+xo,y+yo,tmp->card);
                } else {
                    // Draw card face down
                    pt_card_back_clr_at(x+xo, y+yo, g_klondike->deckcolor);
                }
                y++; 
                tmp = tmp->next;
            }
        } else {
            // draw space
            klondike_draw_space(x+xo,y+yo);
        }
    }

    // Draw waste pile, and button
    if(g_klondike->waste) {
        tmp = g_klondike->waste;
        x = 2;
        y = 6;
        i = count_deck(g_klondike->waste);
        tmp = get_card_at(&g_klondike->waste, i-3); //3rd to Last card
        if(tmp){
            pt_card_clr_at(x+xo,y+yo,tmp->card);
            y++;
        }
        tmp = get_card_at(&g_klondike->waste, i-2); //2nd to Last card
        if(tmp){
            pt_card_clr_at(x+xo,y+yo,tmp->card);
            y++;
        }
        tmp = get_card_at(&g_klondike->waste, i-1); //Last card
        if(tmp){
            pt_card_clr_at(x+xo,y+yo,tmp->card);
        }
        klondike_draw_btn(g_klondike->btns[0],2+xo,13+yo);
    }

    // Draw the foundations/buttons
    scr_pt(69+xo,3+yo,"\u2664");
    scr_pt(76+xo,3+yo,"\u2661");
    scr_pt(69+xo,10+yo,"\u2662");
    scr_pt(76+xo,10+yo,"\u2667");
    klondike_draw_space(67+xo,1+yo);
    klondike_draw_space(74+xo,1+yo);
    klondike_draw_space(67+xo,8+yo);
    klondike_draw_space(74+xo,8+yo);
    for(i = 0; i < 4; i++) {
       tmp = g_klondike->foundation[i];
       switch(i) {
           case 0: x = 67; y = 1; break;
           case 1: x = 74; y = 1; break;
           case 2: x = 67; y = 8; break;
           case 3: x = 74; y = 8; break;
           default: break;
       }
       if (tmp) {
           // Draw the last card of the foundation pile 
           while(tmp->next) {
               tmp = tmp->next;
           }
           pt_card_clr_at(x+xo, y+yo, g_klondike->deckcolor);
       }
    } 
    klondike_draw_btn(g_klondike->btns[8],67+xo,0+yo);
    klondike_draw_btn(g_klondike->btns[9],74+xo,0+yo);
    klondike_draw_btn(g_klondike->btns[10],67+xo,7+yo);
    klondike_draw_btn(g_klondike->btns[11],74+xo,7+yo);

    // Draw the messages at the bottom of the screen
    scr_pt(0,g_screenH - 1,"Stock: %d. Waste: %d. Press q to exit.", count_deck(g_klondike->deck), count_deck(g_klondike->waste));
    scr_pt(0,g_screenH - 2,"b:%d. c:%d. d:%d. e:%d. f:%d. g:%d. h:%d. Active buttons: %d",
            count_deck(g_klondike->tableau[0]),
            count_deck(g_klondike->tableau[1]),
            count_deck(g_klondike->tableau[2]),
            count_deck(g_klondike->tableau[3]),
            count_deck(g_klondike->tableau[4]),
            count_deck(g_klondike->tableau[5]),
            count_deck(g_klondike->tableau[6]),
            klondike_count_selected_btns());
    scr_reset();
}

void klondike_draw_space(int x, int y) {
//U+255x	═	║	╒	╓	╔	╕	╖	╗	╘	╙	╚	╛	╜	╝	╞	╟
    scr_pt(x,y, "\u2554   \u2557");
    scr_pt(x,y+4,"\u255A   \u255D");
}

void klondike_draw_btn(Button *btn, int x, int y) {
    if(btn->active) {
        if(btn->selected) {
            scr_pt_clr(x,y,237,255,"[ %c ]", btn->ch);
        } else {
            scr_pt(x, y, "[ %c ]", btn->ch);
        }
    }
    scr_reset();
}

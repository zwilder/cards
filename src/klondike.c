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

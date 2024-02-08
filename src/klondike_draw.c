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
        klondike_draw_btn(btn, 2+xo, 0+yo);
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

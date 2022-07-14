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

/*
 * 
        Box drawing reference
     	0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F
U+250x	─	━	│	┃	┄	┅	┆	┇	┈	┉	┊	┋	┌	┍	┎	┏

U+251x	┐	┑	┒	┓	└	┕	┖	┗	┘	┙	┚	┛	├	┝	┞	┟

U+252x	┠	┡	┢	┣	┤	┥	┦	┧	┨	┩	┪	┫	┬	┭	┮	┯

U+253x	┰	┱	┲	┳	┴	┵	┶	┷	┸	┹	┺	┻	┼	┽	┾	┿

U+254x	╀	╁	╂	╃	╄	╅	╆	╇	╈	╉	╊	╋	╌	╍	╎	╏

U+255x	═	║	╒	╓	╔	╕	╖	╗	╘	╙	╚	╛	╜	╝	╞	╟

U+256x	╠	╡	╢	╣	╤	╥	╦	╧	╨	╩	╪	╫	╬	╭	╮	╯

U+257x	╰	╱	╲	╳	╴	╵	╶	╷	╸	╹	╺	╻	╼	╽	╾	╿

        0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F
U+258x	▀	▁	▂	▃	▄	▅	▆	▇	█	▉	▊	▋	▌	▍	▎	▏

U+259x	▐	░	▒	▓	▔	▕	▖	▗	▘	▙	▚	▛	▜	▝	▞	▟
 */
/*         1         2         3         4         5         6         7         8
  12345678901234567890123456789012345678901234567890123456789012345678901234567890
 1                                                ╔╔╔═══╗                                           
 2             ╔╔═══╗ ♠═K═╗ ♣═J═╗ ♥═10╗ ♥═A═╗     ║║║╔╗/║                                          
 3             ║║╔╗/║ ║   ║ ║   ║ ║   ║ ║   ║     ║║║╚║╗║                                   
 4             ║║╚║╗║ ║ K ║ ║ J ║ ║ 10║ ║ A ║     ║║║/╚╝║                                   
 5             ║║/╚╝║ ║   ║ ║   ║ ║   ║ ║   ║     ╚╚╚═══╝                                             
 6             ╚╚═══╝ ╚═K═♠ ╚═J═♣ ╚═10♥ ╚═A═♥
 7                                                                                          
 8                    ╔═══════════════════════════════════════╗                             
 9             ╔♥═6═╗ ║ . .¡..¡ ..... ..... ..... ..... ..... ║ 5 
10             ║║   ║ ║ . ..... ..... ..... ..... ..... ..... ║    
 1             ║║ 6 ║ ║                                       ║ 
 2             ║║   ║ ║ . ..¡.. ...¡. ..... ..... ..... ..... ║ 
 3             ╚╚═6═♥ ║ . ..... ..... ..... ..... ..... ..... ║ 9
 4                    ╚═══════════════════════════════════════╝
 5                                     Count: 12
 6                    ♣═Q═╗ ♥═A═╗ ♣═4═╗                            
 7                    ║   ║ ║   ║ ║   ║           ♣═4═╗ ♣═4═╗      
 8                    ║ Q ║ ║ A ║ ║ 4 ║           ║   ║ ║   ║  
 9                    ║   ║ ║   ║ ║   ║           ║ 4 ║ ║ 4 ║  
20                    ╚═Q═♣ ╚═A═♥ ╚═4═♣           ║   ║ ║   ║     
 1                                                ╚═4═♣ ╚═4═♣    
 2                                                [ a ] [ s ]
 3zwilder: 3 15's for 6, 2 runs of 3 for 6, pair for 2 - 14 points!
 4computer: Pair - 2
 *   	0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F
U+255x	═	║	╒	╓	╔	╕	╖	╗	╘	╙	╚	╛	╜	╝	╞	╟
*/
void cribbage_draw(void) {
    int xo = g_screenW / 2 - 25; //80x24
    int yo = g_screenH / 2 - 12;
    uint8_t fg = 172;
    uint8_t bg = 238;
    scr_clear();
    cribbage_draw_board(fg,bg,xo,yo);
    cribbage_draw_score(fg,bg,xo,yo);
    cribbage_draw_cards(fg,bg,xo,yo);
    cribbage_draw_buttons(xo,yo);
    //Count text at 14,14
    //Messages
    xo -= 15;
    if(check_flag(g_cribbage->flags, CRIB_STDISCARD)) {
        scr_pt(xo,22+yo, "> Choose cards for the crib.");
    } else if(check_flag(g_cribbage->flags,CRIB_PTURN)) {
        scr_pt(xo,22+yo, "> Choose a card to play.");
    }
    //scr_pt(xo,23+yo,"%s", g_cribbage->msg);
    scr_reset();
}

void cribbage_draw_buttons(int xo, int yo) {
    int x;
    for(x = 0; x < 6; x++) {
        if(g_cribbage->btns[x]->active) {
            if(g_cribbage->btns[x]->selected) {
                scr_pt_clr(g_cribbage->btns[x]->x+xo, g_cribbage->btns[x]->y+yo,
                        237,255,
                        "[ %c ]", g_cribbage->btns[x]->ch);
                scr_reset();
            } else {
                scr_pt(g_cribbage->btns[x]->x+xo, g_cribbage->btns[x]->y+yo,
                        "[ %c ]", g_cribbage->btns[x]->ch);
            }
        }
    }
    
}

void cribbage_draw_cards(int fg,int bg,int xo,int yo) {
    int x,y,btn;
    int fl = g_cribbage->flags;
    Deck *tmp = NULL;
    if(g_cribbage->deck) {
        pt_deck_stack_clr_at(0+xo,8+yo,g_cribbage->deckcolor);
        if(check_flag(fl, CRIB_CTURN) ||
                check_flag(fl, CRIB_PTURN) ||
                check_flag(fl, CRIB_STSHOW) ||
                check_flag(fl, CRIB_CGO) ||
                check_flag(fl, CRIB_PGO)) {
            pt_card_clr_at(1+xo,8+yo,g_cribbage->deck->card);
        }
    }
    if(g_cribbage->crib) {
        if(check_flag(fl, CRIB_PCRIB)) {
            pt_deck_stack_clr_at(0+xo,16+yo,g_cribbage->deckcolor);
        } else if(check_flag(fl, CRIB_CCRIB)) {
            pt_deck_stack_clr_at(0+xo,0+yo,g_cribbage->deckcolor);
        }
    }
    if(g_cribbage->ctable) {
        tmp = g_cribbage->ctable;
        x = 7;
        y = 1;
        while(tmp) {
            pt_card_clr_at(x+xo,y+yo,tmp->card);
            x += 6;
            tmp = tmp->next;
        }
    }
    if(g_cribbage->computer->hand) {
        tmp = g_cribbage->computer->hand;
        x = 35;
        y = 0;
        while(tmp) {
            pt_card_back_clr_at(x+xo,y+yo,g_cribbage->deckcolor);
            x++;
            tmp = tmp->next;
        }
    }
    if(g_cribbage->ptable) {
        tmp = g_cribbage->ptable;
        x = 7;
        y = 15;
        while(tmp) {
            pt_card_clr_at(x+xo,y+yo,tmp->card);
            x += 6;
            tmp = tmp->next;
        }
    }
    if(g_cribbage->player->hand) {
        tmp = g_cribbage->player->hand;
        x = 41;
        y = 16;
        btn = 0;
        while(tmp) {
            pt_card_clr_at(x+xo,y+yo,tmp->card);
            //scr_pt(x+xo,y+yo+5,"[ %d ]",btn);
            g_cribbage->btns[btn]->active = true;
            g_cribbage->btns[btn]->x = x;
            g_cribbage->btns[btn]->y = y+5;
            x -= 6;
            tmp = tmp->next;
            btn++;
        }
    }
}

void cribbage_draw_score(int fg, int bg, int xo, int yo) {
    scr_set_style(ST_BOLD);
    if(g_cribbage->computer->score == 0) {
        scr_pt_clr(9+xo,8+yo,9,bg,"\u00A1");
        scr_pt_clr(9+xo,9+yo,9,bg,"\u00A1");
    } else {
        cribbage_draw_peg(10+xo,8+yo,9,bg,g_cribbage->computer->pegA);
        cribbage_draw_peg(10+xo,8+yo,9,bg,g_cribbage->computer->pegB);
    }
    if(g_cribbage->player->score == 0) {
        scr_pt_clr(9+xo,11+yo,10,bg,"\u00A1");
        scr_pt_clr(9+xo,12+yo,10,bg,"\u00A1");
    } else {
        cribbage_draw_peg(10+xo,11+yo,10,bg,g_cribbage->player->pegA);
        cribbage_draw_peg(10+xo,11+yo,10,bg,g_cribbage->player->pegB);
    }
    scr_reset();
    scr_pt_clr(49+xo,8+yo,9,0,"%d",g_cribbage->computer->score);
    scr_pt_clr(49+xo,11+yo,10,0,"%d",g_cribbage->player->score);
}

void cribbage_draw_peg(int x, int y, int fg, int bg, int sc) {
    int bx,by,ofs;
    if((sc > 0) && (sc < 31)) {
        by = 0;
        ofs = sc / 5;
        bx = sc + ofs;
    } else if((sc > 30) && (sc < 61)) {
        by = 1;
        ofs = (sc - 30) / 5;
        bx = 66 - sc - ofs;
    } else if((sc > 60) && (sc < 91)) {
        by = 0;
        ofs = (sc - 60) / 5;
        bx = (sc - 60) + ofs;
    } else if((sc > 90) && (sc < 121)) {
        by = 1;
        ofs = (sc - 90) / 5;
        bx = 126 - sc - ofs;
    } else {
        by = 1;
        bx = -1;
    }
    by += y;
    bx += x;
    scr_set_style(ST_BOLD);
    scr_pt_clr(bx,by,fg,bg,"\u00A1");
    scr_reset();
}

void cribbage_draw_board(int fg, int bg, int xo, int yo) {
    int x,y;
    scr_pt_clr(7+xo,7+yo,fg,bg,"\u2554");
    scr_pt_clr(7+xo,13+yo,fg,bg,"\u255A");
    scr_pt_clr(47+xo,7+yo,fg,bg,"\u2557");
    scr_pt_clr(47+xo,13+yo,fg,bg,"\u255D");
    for(x = 8; x < 47; x++) {
        scr_pt_clr(x+xo,7+yo,fg,bg,"\u2550");
        for(y = 8; y < 13; y++) {
            if(y == 10) {
                scr_pt_clr(x+xo,y+yo,fg,bg," ");
                continue;
            }
            if(x == 8 || x == 10 || x == 16 || x == 22 || x == 28 || x == 34 ||
                    x == 40 || x == 46) {
                scr_pt_clr(x+xo,y+yo,fg,bg," ");
            } else {
                scr_pt_clr(x+xo,y+yo,250,bg,".");
            }
        }
        scr_pt_clr(x+xo,13+yo,fg,bg,"\u2550");
    }
    for(y = 8; y < 13; y++) {
        scr_pt_clr(7+xo,y+yo,fg,bg,"\u2551");
        scr_pt_clr(47+xo,y+yo,fg,bg,"\u2551");
    }
}

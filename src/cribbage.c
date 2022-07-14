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

 * Game mockup - Obv the number of cards on the table is too many!             *
 * Just wanted to "see" what it would look like. Std 80x24 terminal.
                                   ╔╔╔═══╗                                           
╔╔═══╗ ♠═K═╗ ♣═J═╗ ♥═10╗ ♥═A═╗     ║║║╔╗/║                                          
║║╔╗/║ ║   ║ ║   ║ ║   ║ ║   ║     ║║║╚║╗║                                   
║║╚║╗║ ║ K ║ ║ J ║ ║ 10║ ║ A ║     ║║║/╚╝║                                   
║║/╚╝║ ║   ║ ║   ║ ║   ║ ║   ║     ╚╚╚═══╝                                             
╚╚═══╝ ╚═K═♠ ╚═J═♣ ╚═10♥ ╚═A═♥
                                                                             
       ╔═══════════════════════════════════════╗                             
╔♥═6═╗ ║ . .¡..¡ ..... ..... ..... ..... ..... ║   5 
║║   ║ ║ . ..... ..... ..... ..... ..... ..... ║    
║║ 6 ║ ║                                       ║ 
║║   ║ ║ . ..¡.. ...¡. ..... ..... ..... ..... ║ 
╚╚═6═♥ ║ . ..... ..... ..... ..... ..... ..... ║   9
       ╚═══════════════════════════════════════╝
                        Count: 12
       ♣═Q═╗ ♥═A═╗ ♣═4═╗                            
       ║   ║ ║   ║ ║   ║           ♣═4═╗ ♣═4═╗      
       ║ Q ║ ║ A ║ ║ 4 ║           ║   ║ ║   ║  
       ║   ║ ║   ║ ║   ║           ║ 4 ║ ║ 4 ║  
       ╚═Q═♣ ╚═A═♥ ╚═4═♣           ║   ║ ║   ║     
                                   ╚═4═♣ ╚═4═♣    
                                   [ 1 ] [ 2 ]
zwilder: 3 15's for 6, 2 runs of 3 for 6, pair for 2 - 14 points!
computer: Pair - 2
 */
Cribbage *g_game = NULL;

void cribbage_init(void) {
    if(g_game) cribbage_cleanup();
    g_game = malloc(sizeof(Cribbage));
    g_game->computer = create_cribbage_player();
    g_game->computer->strategy = 1;
    g_game->player = create_cribbage_player();
    g_game->deck = create_std_deck();
    g_game->flags = CRIB_STNONE;
    g_game->ctable = NULL;
    g_game->ptable = NULL;
    g_game->crib = NULL;

    g_game->btns = malloc(6 * sizeof(Button));
    g_game->btns[0] = create_button('h',0);
    g_game->btns[1] = create_button('g',1);
    g_game->btns[2] = create_button('f',2);
    g_game->btns[3] = create_button('d',3);
    g_game->btns[4] = create_button('s',4);
    g_game->btns[5] = create_button('a',5);
    
    cribbage_draw();
    cribbage_loop();
}

Button* create_button(char ch, int num) {
    Button *btn = malloc(sizeof(Button));
    btn->ch = ch;
    btn->active = false;
    btn->selected = false;
    btn->x = 0;
    btn->y = 0;
    btn->num = num;
    return btn;
}

void toggle_button(int btn) {
    if(!g_game->btns[btn]->active) return;
    g_game->btns[btn]->selected = !(g_game->btns[btn]->selected);
}

void cribbage_cleanup(void) {
    if(!g_game) return;
    int i;
    for(i = 0; i < 6; i++) {
        free(g_game->btns[i]);
        g_game->btns[i] = NULL;
    }
    free(g_game->btns);
    g_game->btns = NULL;
    destroy_cribbage_player(&g_game->player);
    destroy_cribbage_player(&g_game->computer);
    destroy_deck(&g_game->deck);
    destroy_deck(&g_game->ctable);
    destroy_deck(&g_game->ptable);
    destroy_deck(&g_game->crib);
    free(g_game);
    g_game = NULL;
}

void cribbage_loop(void) {
    bool running = true;
    cribbage_change_state(CRIB_STCUT);
    while(running) {
        running = cribbage_events();
        cribbage_update();
        cribbage_draw();
    }
    cribbage_cleanup();
}

void cribbage_change_state(int st) {
    if(!g_game) return;
    int fl = g_game->flags;
    remove_flag(&fl, CRIB_STNONE);
    remove_flag(&fl, CRIB_STCUT);
    remove_flag(&fl, CRIB_STDISCARD);
    remove_flag(&fl, CRIB_STSHOW);
    remove_flag(&fl, CRIB_PTURN);
    remove_flag(&fl, CRIB_CTURN);
    engage_flag(&fl, st);
    g_game->flags = fl;
}

bool cribbage_events(void) {
    bool running = true;
    int input = kb_get_char();
    switch(input) {
        case 'q': running = false; break;
        case 'a': toggle_button(5); break;
        case 's': toggle_button(4); break;
        case 'd': toggle_button(3); break;
        case 'f': toggle_button(2); break;
        case 'g': toggle_button(1); break;
        case 'h': toggle_button(0); break;
        case 'n': new_cribbage_round(); break;
        default: break;
    }
    return running;
}

void new_cribbage_round(void) {
    add_cards(&g_game->ctable, &g_game->deck);
    add_cards(&g_game->ptable, &g_game->deck);
    add_cards(&g_game->crib, &g_game->deck);
    add_cards(&g_game->computer->hand, &g_game->deck);
    add_cards(&g_game->player->hand, &g_game->deck);
    draw_cards(&g_game->deck, &g_game->computer->hand, 6);
    draw_cards(&g_game->deck, &g_game->player->hand, 6);
    cribbage_change_state(CRIB_STDISCARD);
}

/*         1         2         3         4         5         6         7         8
  12345678901234567890123456789012345678901234567890123456789012345678901234567890
 1                                   ╔╔╔═══╗                                           
 2╔╔═══╗ ♠═K═╗ ♣═J═╗ ♥═10╗ ♥═A═╗     ║║║╔╗/║                                          
 3║║╔╗/║ ║   ║ ║   ║ ║   ║ ║   ║     ║║║╚║╗║                                   
 4║║╚║╗║ ║ K ║ ║ J ║ ║ 10║ ║ A ║     ║║║/╚╝║                                   
 5║║/╚╝║ ║   ║ ║   ║ ║   ║ ║   ║     ╚╚╚═══╝                                             
 6╚╚═══╝ ╚═K═♠ ╚═J═♣ ╚═10♥ ╚═A═♥
 7                                                                             
 8       ╔═══════════════════════════════════════╗                             
 9╔♥═6═╗ ║ . .¡..¡ ..... ..... ..... ..... ..... ║ 5 
10║║   ║ ║ . ..... ..... ..... ..... ..... ..... ║    
 1║║ 6 ║ ║                                       ║ 
 2║║   ║ ║ . ..¡.. ...¡. ..... ..... ..... ..... ║ 
 3╚╚═6═♥ ║ . ..... ..... ..... ..... ..... ..... ║ 9
 4       ╚═══════════════════════════════════════╝
 5                        Count: 12
 6       ♣═Q═╗ ♥═A═╗ ♣═4═╗                            
 7       ║   ║ ║   ║ ║   ║           ♣═4═╗ ♣═4═╗      
 8       ║ Q ║ ║ A ║ ║ 4 ║           ║   ║ ║   ║  
 9       ║   ║ ║   ║ ║   ║           ║ 4 ║ ║ 4 ║  
20       ╚═Q═♣ ╚═A═♥ ╚═4═♣           ║   ║ ║   ║     
 1                                   ╚═4═♣ ╚═4═♣    
 2                                   [ a ] [ s ]
 3zwilder: 3 15's for 6, 2 runs of 3 for 6, pair for 2 - 14 points!
 4computer: Pair - 2
 *   	0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F
U+255x	═	║	╒	╓	╔	╕	╖	╗	╘	╙	╚	╛	╜	╝	╞	╟
*/
void cribbage_draw(void) {
    int xo = g_screenW / 2 - 40; //80x24
    int yo = g_screenH / 2 - 12;
    uint8_t fg = 172;
    uint8_t bg = 238;
    Deck *tmp = NULL;
    int x,y,btn;
    scr_clear();
    draw_board(fg,bg,xo,yo);
    draw_score(fg,bg,xo,yo);
    if(g_game->deck) {
        pt_deck_stack_clr_at(0+xo,8+yo,43);
        if(check_flag(g_game->flags, CRIB_CTURN) ||
                check_flag(g_game->flags, CRIB_PTURN) ||
                check_flag(g_game->flags, CRIB_STSHOW)) {
            pt_card_clr_at(1+xo,8+yo,g_game->deck->card);
        }
    }
    if(g_game->crib) {
        if(check_flag(g_game->flags, CRIB_PCRIB)) {
            pt_deck_stack_clr_at(0+xo,16+yo,43);
        } else if(check_flag(g_game->flags, CRIB_CCRIB)) {
            pt_deck_stack_clr_at(0+xo,0+yo,43);
        }
    }
    if(g_game->ctable) {
        tmp = g_game->ctable;
        x = 7;
        y = 1;
        while(tmp) {
            pt_card_clr_at(x+xo,y+yo,tmp->card);
            x += 6;
            tmp = tmp->next;
        }
    }
    if(g_game->computer->hand) {
        tmp = g_game->computer->hand;
        x = 35;
        y = 0;
        while(tmp) {
            pt_card_back_clr_at(x+xo,y+yo,43);
            x++;
            tmp = tmp->next;
        }
    }
    if(g_game->ptable) {
        tmp = g_game->ptable;
        x = 7;
        y = 15;
        while(tmp) {
            pt_card_clr_at(x+xo,y+yo,tmp->card);
            x += 6;
            tmp = tmp->next;
        }
    }
    if(g_game->player->hand) {
        tmp = g_game->player->hand;
        x = 41;
        y = 16;
        btn = 0;
        while(tmp) {
            pt_card_clr_at(x+xo,y+yo,tmp->card);
            //scr_pt(x+xo,y+yo+5,"[ %d ]",btn);
            g_game->btns[btn]->active = true;
            g_game->btns[btn]->x = x;
            g_game->btns[btn]->y = y+5;
            x -= 6;
            tmp = tmp->next;
            btn++;
        }
    }
    for(x = 0; x < 6; x++) {
        if(g_game->btns[x]->active) {
            if(g_game->btns[x]->selected) {
                scr_pt_clr(g_game->btns[x]->x+xo, g_game->btns[x]->y+yo,
                        237,255,
                        "[ %c ]", g_game->btns[x]->ch);
                scr_reset();
            } else {
                scr_pt(g_game->btns[x]->x+xo, g_game->btns[x]->y+yo,
                        "[ %c ]", g_game->btns[x]->ch);
            }
        }
    }
    
    //Count text
    //Messages
    if(check_flag(g_game->flags, CRIB_STDISCARD)) {
        scr_pt(xo,23+yo, "> Choose cards for the crib.");
    }
    scr_reset();
}

void draw_score(int fg, int bg, int xo, int yo) {
    scr_set_style(ST_BOLD);
    if(g_game->computer->score == 0) {
        scr_pt_clr(9+xo,8+yo,9,bg,"\u00A1");
        scr_pt_clr(9+xo,9+yo,9,bg,"\u00A1");
    } else {
        draw_peg(10+xo,8+yo,9,bg,g_game->computer->pegA);
        draw_peg(10+xo,8+yo,9,bg,g_game->computer->pegB);
    }
    if(g_game->player->score == 0) {
        scr_pt_clr(9+xo,11+yo,10,bg,"\u00A1");
        scr_pt_clr(9+xo,12+yo,10,bg,"\u00A1");
    } else {
        draw_peg(10+xo,11+yo,10,bg,g_game->player->pegA);
        draw_peg(10+xo,11+yo,10,bg,g_game->player->pegB);
    }
    scr_reset();
    scr_pt_clr(49+xo,8+yo,9,0,"%d",g_game->computer->score);
    scr_pt_clr(49+xo,11+yo,10,0,"%d",g_game->player->score);
}

void draw_peg(int x, int y, int fg, int bg, int sc) {
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

void draw_board(int fg, int bg, int xo, int yo) {
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

void cribbage_update(void) {

}

int get_cribbage_card_value(int card) {
    if(((card & CD_K) == CD_K) ||
            ((card & CD_Q) == CD_Q) ||
            ((card & CD_J) == CD_J)) {
        return 10;
    } else {
        return get_value(card);
    }
}

CribPlayer* create_cribbage_player(void) {
    CribPlayer *player = malloc(sizeof(CribPlayer));
    player->pegA = 0;
    player->pegB = 0;
    player->score = 0;
    player->hand = NULL;
    player->strategy = 0;
    return player;
}

void destroy_cribbage_player(CribPlayer **player) {
    if(!(*player)) return;
    CribPlayer *tmp = *player;
    *player = NULL;
    if(tmp->hand) {
        destroy_deck(&tmp->hand);
        tmp->hand = NULL;
    }
    free(tmp);
}

CribScore* create_cribscore(int qty, int pts, char *msg,...) {
    CribScore *score = malloc(sizeof(CribScore));
    va_list args;
    va_start(args, msg);
    score->qty = qty;
    score->pts = pts;
    score->msg = malloc(50 * sizeof(char));
    vsnprintf(score->msg, 50, msg, args);
    return score;
}

void destroy_cribscore(CribScore *score) {
    if(!score) return;
    if(score->msg) free(score->msg);
    free(score);
}

int score_cribbage_hand(Deck *hand, Deck *flop) {
    char *buf = malloc(500 * sizeof(char));
    buf[0] = '\0'; 
    int score = 0;
    CribScore *fifteens = count_15s(hand, flop);
    CribScore *runs = count_runs(hand, flop);
    CribScore *pairs = count_pairs(hand,flop);
    CribScore *flush = count_flush(hand,flop);
    CribScore *nobs = count_nobs(hand,flop);
    if(fifteens) {
        score += fifteens->pts;
        strcat(buf, fifteens->msg);
    }
    if(runs) {
        score += runs->pts;
        if(buf[0] != '\0') strcat(buf, ", ");
        strcat(buf, runs->msg);
    }
    if(pairs) {
        score += pairs->pts;
        if(buf[0] != '\0') strcat(buf, ", ");
        strcat(buf, pairs->msg);
    }
    if(flush) {
        /*Need to remember to add something to check if this is the crib...
         * since we need all 5 cards to be the same for a flush to count in the
         * crib */
        score += flush->pts;
        if(buf[0] != '\0') strcat(buf, ", ");
        strcat(buf, flush->msg);
    }
    if(nobs) {
        score += nobs->pts;
        if(buf[0] != '\0') strcat(buf, ", and ");
        strcat(buf, nobs->msg);
    }
    if(score) {
        scr_pt(0, g_screenH - 3, "%s - %d points!", buf, score);
    }
    destroy_cribscore(fifteens);
    destroy_cribscore(runs);
    destroy_cribscore(pairs);
    destroy_cribscore(flush);
    destroy_cribscore(nobs);
    free(buf);
    return score;
}

CribScore* count_runs(Deck *hand, Deck *flop) {
    if(!hand) return 0;
    if(!flop) return 0;
    if(count_deck(hand) != 4) return 0;
    CribScore *result = NULL;
    uint8_t matrix[13][5] = {{ 0 }};
    int x,y,cur,prev,r,m,br,bm;
    int cards[5];
    cards[0] = hand->card;
    cards[1] = hand->next->card;
    cards[2] = hand->next->next->card;
    cards[3] = hand->next->next->next->card;
    cards[4] = flop->card;
    for(x = 0; x < 13; x++) {
        for(y = 0; y < 5; ++y) {
            if(get_value(cards[y]) == (x+1)) {
                if((cards[y] & CD_H) == CD_H) matrix[x][0] += 1;
                if((cards[y] & CD_C) == CD_C) matrix[x][1] += 1;
                if((cards[y] & CD_D) == CD_D) matrix[x][2] += 1;
                if((cards[y] & CD_S) == CD_S) matrix[x][3] += 1;
            }
        }
        matrix[x][4] = matrix[x][0] + matrix[x][1] + matrix[x][2] + matrix[x][3];
    }
    br = bm = r = m = 1;
    cur = 0;
    prev = matrix[0][4];
    for(x = 1; x < 13; x++) {
        cur = matrix[x][4];
        if(cur && prev) {
            r++;
            if(cur > 1) m = cur;
        } else {
            if(r > br) {
                br = r;
                bm = m;
            }
            r = 1;
            m = 1;
        }
        prev = cur;
    }
    if(r > br) {
        br = r;
        bm = m;
    }
    if(br >= 3) {
        if(bm == 1) {
            result = create_cribscore(1, br, "a run of %d for %d", br,br);
        } else if(bm == 2) {
            result = create_cribscore(bm, br*bm, 
                    "a double run of %d for %d", br,br*bm);
        } else if(bm == 3) {
            result = create_cribscore(bm, br*bm, 
                    "a triple run of %d for %d", br,br*bm);
        }
    }
    return result;
}

CribScore* count_flush(Deck *hand, Deck *flop) {
    if(!hand) return 0;
    if(!flop) return 0;
    if(count_deck(hand) != 4) return 0;
    CribScore *result = NULL;
    int cards[5];
    cards[0] = hand->card;
    cards[1] = hand->next->card;
    cards[2] = hand->next->next->card;
    cards[3] = hand->next->next->next->card;
    cards[4] = flop->card;
    uint8_t matrix[4][5] = {{ 0 }};
    int x,y,n;
    for(y = 0; y < 4; y++) {
        if((cards[y] & CD_H) == CD_H) matrix[0][y] += 1;
        if((cards[y] & CD_S) == CD_S) matrix[1][y] += 1;
        if((cards[y] & CD_D) == CD_D) matrix[2][y] += 1;
        if((cards[y] & CD_C) == CD_C) matrix[3][y] += 1;
    }
    n = 1;
    for(x = 0; x < 4; x++) {
        matrix[x][4] = matrix[x][0] + matrix[x][1] + matrix[x][2] + matrix[x][3];
        if(matrix[x][4] > n) n = matrix[x][4];
    }
    if(n == 4) {
        if(get_suite_ch(cards[4]) == get_suite_ch(cards[0])) {
            result = create_cribscore(1,5, "flush for 5");
        } else {
            result = create_cribscore(1,4, "flush for 4");
        }
    }

    return result;
}

CribScore* count_nobs(Deck *hand, Deck *flop) {
    if(!hand) return 0;
    if(!flop) return 0;
    if(count_deck(hand) != 4) return 0;
    CribScore *result = NULL;
    int i = 0;
    int A = hand->card;
    int B = hand->next->card;
    int C = hand->next->next->card;
    int D = hand->next->next->next->card;
    int E = flop->card;
    if(((A & CD_J) == CD_J) && (get_suite_ch(E) == get_suite_ch(A))) i++;
    if(((B & CD_J) == CD_J) && (get_suite_ch(E) == get_suite_ch(B))) i++;
    if(((C & CD_J) == CD_J) && (get_suite_ch(E) == get_suite_ch(C))) i++;
    if(((D & CD_J) == CD_J) && (get_suite_ch(E) == get_suite_ch(D))) i++;
    if(i) {
        result = create_cribscore(1, 1, "one for his nobs!");
    }
    return result;
}

CribScore* count_pairs(Deck *hand, Deck *flop) {
    if(!hand) return 0;
    if(!flop) return 0;
    if(count_deck(hand) != 4) return 0;
    CribScore *result = NULL;
    int i = 0;
    int A = get_value(hand->card);
    int B = get_value(hand->next->card);
    int C = get_value(hand->next->next->card);
    int D = get_value(hand->next->next->next->card);
    int E = get_value(flop->card);
    if(A == B) i++;
    if(A == C) i++;
    if(A == D) i++;
    if(A == E) i++;
    if(B == C) i++;
    if(B == D) i++;
    if(B == E) i++;
    if(C == D) i++;
    if(C == E) i++;
    if(D == E) i++;
    if(i == 1) {
        result = create_cribscore(1, 2, "pair for 2");
    } else if(i == 2) {
        result = create_cribscore(2, 4, "two pairs for 4");
    } else if(i == 3) {
        result = create_cribscore(3, 6, "a pair royale for 6");
    } else if(i == 6) {
        result = create_cribscore(6, 12, "a double pair royale for 12");
    }
    return result;
}


CribScore* count_15s(Deck *hand, Deck *flop) {
    if(!hand) return 0;
    if(!flop) return 0;
    if(count_deck(hand) != 4) return 0;
    CribScore *result = NULL;
    int i = 0;
    int A = get_cribbage_card_value(hand->card);
    int B = get_cribbage_card_value(hand->next->card);
    int C = get_cribbage_card_value(hand->next->next->card);
    int D = get_cribbage_card_value(hand->next->next->next->card);
    int E = get_cribbage_card_value(flop->card);
    if(A+B == 15) i++;
    if(A+C == 15) i++;
    if(A+D == 15) i++;
    if(A+E == 15) i++;
    if(B+C == 15) i++;
    if(B+D == 15) i++;
    if(B+E == 15) i++;
    if(C+D == 15) i++;
    if(C+E == 15) i++;
    if(D+E == 15) i++;
    if(A+B+C == 15) i++;
    if(A+B+D == 15) i++;
    if(A+B+E == 15) i++;
    if(A+C+E == 15) i++;
    if(A+D+E == 15) i++;
    if(B+C+D == 15) i++;
    if(B+C+E == 15) i++;
    if(C+D+E == 15) i++;
    if(A+B+C+D == 15) i++;
    if(A+B+C+E == 15) i++;
    if(A+C+D+E == 15) i++;
    if(B+C+D+E == 15) i++;
    if(A+B+C+D+E == 15) i++;
    
    if(i == 1) {
        result = create_cribscore(1, 2, "15 for 2");
    } else if (i) {
        result = create_cribscore(i, i*2, "%d 15s for %d", i, i*2);
    }
    return result;
}

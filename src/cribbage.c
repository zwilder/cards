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

Cribbage *g_cribbage = NULL;

void cribbage_init(void) {
    if(g_cribbage) cribbage_cleanup();
    g_cribbage = malloc(sizeof(Cribbage));
    g_cribbage->computer = create_cribbage_player();
    g_cribbage->computer->strategy = 1;
    g_cribbage->player = create_cribbage_player();
    g_cribbage->deck = create_std_deck();
    g_cribbage->flags = CRIB_STNONE;
    g_cribbage->ctable = NULL;
    g_cribbage->ptable = NULL;
    g_cribbage->crib = NULL;

    g_cribbage->btns = malloc(6 * sizeof(Button));
    g_cribbage->btns[0] = create_button('h',0);
    g_cribbage->btns[1] = create_button('g',1);
    g_cribbage->btns[2] = create_button('f',2);
    g_cribbage->btns[3] = create_button('d',3);
    g_cribbage->btns[4] = create_button('s',4);
    g_cribbage->btns[5] = create_button('a',5);

    g_cribbage->deckcolor = 31; //Eventually there will be an option for this.
    
    //g_cribbage->msg = malloc(81 * sizeof(char));
    //g_cribbage->msg[0] = '\0';
    cribbage_draw();
    cribbage_loop();
}

void cribbage_cleanup(void) {
    if(!g_cribbage) return;
    int i;
    for(i = 0; i < 6; i++) {
        free(g_cribbage->btns[i]);
        g_cribbage->btns[i] = NULL;
    }
    free(g_cribbage->btns);
    g_cribbage->btns = NULL;
    destroy_cribbage_player(&g_cribbage->player);
    destroy_cribbage_player(&g_cribbage->computer);
    destroy_deck(&g_cribbage->deck);
    destroy_deck(&g_cribbage->ctable);
    destroy_deck(&g_cribbage->ptable);
    destroy_deck(&g_cribbage->crib);
    //free(g_cribbage->msg);
    //g_cribbage->msg = NULL;
    free(g_cribbage);
    g_cribbage = NULL;
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
    if(!g_cribbage) return;
    int fl = g_cribbage->flags;
    remove_flag(&fl, CRIB_STNONE);
    remove_flag(&fl, CRIB_STCUT);
    remove_flag(&fl, CRIB_STDISCARD);
    remove_flag(&fl, CRIB_STSHOW);
    remove_flag(&fl, CRIB_PTURN);
    remove_flag(&fl, CRIB_CTURN);
    engage_flag(&fl, st);
    g_cribbage->flags = fl;
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

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
CribPlayer *g_computer = NULL;
CribPlayer *g_player = NULL;
Deck *g_deck = NULL;

CribPlayer* create_cribbage_player(void) {
    CribPlayer *player = malloc(sizeof(CribPlayer));
    player->pegA = 0;
    player->pegB = 0;
    player->score = 0;
    player->hand = NULL;
    player->strategy = 0;
    return player;
}

void destroy_cribbage_player(CribPlayer *player) {
    if(player->hand) {
        destroy_deck(&(player->hand));
        player->hand = NULL;
    }
    free(player);
}

void cribbage_init(void) {
    if(g_computer) destroy_cribbage_player(g_computer);
    g_computer = create_cribbage_player();
    //g_computer->strategy = get_cribbage_strategy();
    g_computer->strategy = 1;
    if(g_player) destroy_cribbage_player(g_player);
    g_player = create_cribbage_player();
    g_deck = create_std_deck();
    
    cribbage_draw();
    cribbage_loop();
}

void cribbage_loop(void) {
    bool running = true;
    while(running) {
        running = cribbage_events();
        cribbage_update();
        cribbage_draw();
    }
}

bool cribbage_events(void) {
    bool running = true;

    return running;
}

void cribbage_draw(void) {

}

void draw_board(void) {

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
    int cards[5];
    cards[0] = hand->card;
    cards[1] = hand->next->card;
    cards[2] = hand->next->next->card;
    cards[3] = hand->next->next->next->card;
    cards[4] = flop->card;
    uint8_t matrix[13][5] = {{ 0 }};
    int x,y,cur,prev,run,mult,bestrun,bestmult;
    for(x = 0; x < 13; x++) {
        for(y = 0; y < 5; ++y) {
            if(get_value(cards[y]) == (x+1)) {
                if(get_suite_ch(cards[y]) == 'h') matrix[x][0] += 1;
                if(get_suite_ch(cards[y]) == 'c') matrix[x][1] += 1;
                if(get_suite_ch(cards[y]) == 'd') matrix[x][2] += 1;
                if(get_suite_ch(cards[y]) == 's') matrix[x][3] += 1;
            }
        }
        matrix[x][4] = matrix[x][0] + matrix[x][1] + matrix[x][2] + matrix[x][3];
    }
    bestrun = 1;
    bestmult = 1;
    cur = 0;
    prev = matrix[0][4];
    run = 1;
    mult = 1;
    for(x = 1; x < 13; x++) {
        cur = matrix[x][4];
        if(cur && prev) {
            run++;
            if(cur > 1) mult = cur;
        } else {
            if(run > bestrun) {
                bestrun = run;
                bestmult = mult;
            }
            run = 1;
            mult = 1;
        }
        prev = cur;
    }
    run = bestrun;
    mult = bestmult;
    if(run >= 3) {
        if(mult == 1) {
            result = create_cribscore(1, run, "a run of %d for %d", run,run);
        } else if(mult == 2) {
            result = create_cribscore(mult, run*mult, "a double run of %d for %d", run,run*mult);
        } else if(mult == 3) {
            result = create_cribscore(mult, run*mult, "a triple run of %d for %d", run,run*mult);
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
    if(((A & CD_J) == CD_J) && (strcmp(get_suite(E), get_suite(A)) == 0)) i++;
    if(((B & CD_J) == CD_J) && (strcmp(get_suite(E), get_suite(B)) == 0)) i++;
    if(((C & CD_J) == CD_J) && (strcmp(get_suite(E), get_suite(C)) == 0)) i++;
    if(((D & CD_J) == CD_J) && (strcmp(get_suite(E), get_suite(D)) == 0)) i++;
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
        result = create_cribscore(i, i*2, "a pair for %d", i*2);
    } else if(i == 2) {
        result = create_cribscore(i, i*2, "two pairs for %d", i*2);
    } else if(i == 3) {
        result = create_cribscore(i, i*2, "a pair royale for %d", i*2);
    } else if(i == 6) {
        result = create_cribscore(i, i*2, "a double pair royale for %d", i*2);
    }
    return result;
}


CribScore* count_15s(Deck *hand, Deck *flop) {
    /* A B C D E
     * Possible pairs are:
     * AB AC AD AE 
     * BC BD BE 
     * CD CE 
     * DE 
     *
     * Possible triplets are
     * ABC ABD ABE ACD ACE ADE
     * BCD BCE
     * CDE 
     *
     * Possible 4 are
     * ABCD ABCE ACDE
     * BCDE
     *
     * Possible 5 is
     * ABCDE
     *
     * Ugly brute force counting. */
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
        result = create_cribscore(i, i*2, "%d 15 for %d", i, i*2);
    } else if (i) {
        result = create_cribscore(i, i*2, "%d 15s for %d", i, i*2);
    }
    return result;
}

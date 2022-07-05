#include <cards.h>
/* Playing Cards
 * ♠ u2660, ♤ u2664
 * ♥ u2665, ♡ u2661
 * ♦ u2666, ♢ u2662
 * ♣ u2663, ♧ u2667
 *
 * Box drawing reference
 *   	0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F
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

 * Cards are 5x5 in size
 * ♠═══╗
 * ║   ║
 * ║ 5 ║
 * ║   ║
 * ╚═══♠
 * ♠═══╗
 * ║   ║
 * ║10 ║
 * ║   ║
 * ╚═══♠
 * ╔═══╗
 * ║╔╗/║
 * ║╚║╗║
 * ║/╚╝║
 * ╚═══╝
 * ╔╔╔═══╗
 * ║║║╔╗/║
 * ║║║╚║╗║
 * ║║║/╚╝║
 * ╚╚╚═══╝
 */
char* get_suite(int card) {
    char *suite = malloc(10 * sizeof(char));
    if((card & CD_H) == CD_H) {
        snprintf(suite, 10, "\u2665"); // Hearts
    } else if((card & CD_S) == CD_S) {
        snprintf(suite, 10, "\u2660"); // Spades 
    } else if((card & CD_C) == CD_C) {
        snprintf(suite, 10, "\u2663"); // Clubs 
    } else if((card & CD_D) == CD_D) {
        snprintf(suite, 10, "\u2666"); // Diamonds 
    } else {
        free(suite);
        suite = NULL;
    }
    return suite;
}

int get_value(int card) {
    int value = 0;
    if((card & CD_A) == CD_A) {
        value = 1;
    } else if ((card & CD_2) == CD_2) {
        value = 2;
    } else if ((card & CD_3) == CD_3) {
        value = 3;
    } else if ((card & CD_4) == CD_4) {
        value = 4;
    } else if ((card & CD_5) == CD_5) {
        value = 5;
    } else if ((card & CD_6) == CD_6) {
        value = 6;
    } else if ((card & CD_7) == CD_7) {
        value = 7;
    } else if ((card & CD_8) == CD_8) {
        value = 8;
    } else if ((card & CD_9) == CD_9) {
        value = 9;
    } else if ((card & CD_10) == CD_10) {
        value = 10;
    } else if ((card & CD_J) == CD_J) {
        value = 11;
    } else if ((card & CD_Q) == CD_Q) {
        value = 12;
    } else if ((card & CD_K) == CD_K) {
        value = 13;
    }
    return value;
}

char* get_value_str(int card) {
    char* value = malloc(3 * sizeof(char));
    if((card & CD_A) == CD_A) {
        snprintf(value, 3, "A");
    } else if ((card & CD_2) == CD_2) {
        snprintf(value, 3, "2");
    } else if ((card & CD_3) == CD_3) {
        snprintf(value, 3, "3");
    } else if ((card & CD_4) == CD_4) {
        snprintf(value, 3, "4");
    } else if ((card & CD_5) == CD_5) {
        snprintf(value, 3, "5");
    } else if ((card & CD_6) == CD_6) {
        snprintf(value, 3, "6");
    } else if ((card & CD_7) == CD_7) {
        snprintf(value, 3, "7");
    } else if ((card & CD_8) == CD_8) {
        snprintf(value, 3, "8");
    } else if ((card & CD_9) == CD_9) {
        snprintf(value, 3, "9");
    } else if ((card & CD_10) == CD_10) {
        snprintf(value, 3, "10");
    } else if ((card & CD_J) == CD_J) {
        snprintf(value, 3, "J");
    } else if ((card & CD_Q) == CD_Q) {
        snprintf(value, 3, "Q");
    } else if ((card & CD_K) == CD_K) {
        snprintf(value, 3, "K");
    } else {
        free(value);
        value = NULL;
    }
    return value;
}

int get_card(int value, char suite) {
    int card = CD_NONE;
    switch(value) {
        case 1:
            card = CD_A;
            break;
        case 2:
            card = CD_2;
            break;
        case 3:
            card = CD_3;
            break;
        case 4:
            card = CD_4;
            break;
        case 5:
            card = CD_5;
            break;
        case 6:
            card = CD_6;
            break;
        case 7:
            card = CD_7;
            break;
        case 8:
            card = CD_8;
            break;
        case 9:
            card = CD_9;
            break;
        case 10:
            card = CD_10;
            break;
        case 11:
            card = CD_J;
            break;
        case 12:
            card = CD_Q;
            break;
        case 13:
            card = CD_K;
            break;
        default:
            card = CD_NONE;
            break;
    }
    switch(suite) {
        case 'S':
        case 's':
            card |= CD_S;
            break;
        case 'H':
        case 'h':
            card |= CD_H;
            break;
        case 'D':
        case 'd':
            card |= CD_D;
            break;
        case 'C':
        case 'c':
            card |= CD_C;
            break;
        default:
            break;
    }

    return card;
}
    
void pt_card_at(int x, int y, int card) {
    if((card & CD_NONE) == CD_NONE) return;
    char *suite = get_suite(card);
    int val = get_value(card);
    char *value = get_value_str(card);

    if(value && suite) {
        scr_pt(x,y, "%s\u2550\u2550\u2550\u2557", suite);
        scr_pt(x,y+1, "\u2551   \u2551");
        if(val == 10) {
            scr_pt(x,y+2, "\u2551%s \u2551", value);
        } else {
            scr_pt(x,y+2, "\u2551 %s \u2551", value);
        }
        scr_pt(x,y+3, "\u2551   \u2551");
        scr_pt(x,y+4, "\u255A\u2550\u2550\u2550%s", suite);
    }

    free(value);
    free(suite);
}

void pt_card_clr_at(int x, int y, int card) {
    if((card & CD_NONE) == CD_NONE) return;
    char *suite = get_suite(card);
    int val = get_value(card);
    char *value = get_value_str(card);
    uint8_t fg,bg;
    if(((card & CD_S) == CD_S) || ((card & CD_C) == CD_C)) {
        fg = 239;
    } else {
        fg = 203;
    }
    bg = 15;

    if(value && suite) {
        scr_pt_clr(x,y,fg,0,"%s\u2550\u2550\u2550\u2557", suite);
        scr_pt_clr(x,y+1,fg,0, "\u2551   \u2551");
        scr_pt_clr(x+1,y+1,fg,bg, "   ");
        scr_pt_clr(x,y+2,fg,0, "\u2551   \u2551");
        if(val == 10) {
            scr_pt_clr(x+1,y+2,fg,bg, "%s ",value);
        } else {
            scr_pt_clr(x+1,y+2,fg,bg, " %s ", value);
        }
        scr_pt_clr(x,y+3,fg,0, "\u2551   \u2551");
        scr_pt_clr(x+1,y+3,fg,bg, "   ");
        scr_pt_clr(x,y+4,fg,0, "\u255A\u2550\u2550\u2550%s", suite);
    }

    free(value);
    free(suite);
}
/*
 *   	0	1	2	3	4	5	6	7	8	9	A	B	C	D	E	F
U+255x	═	║	╒	╓	╔	╕	╖	╗	╘	╙	╚	╛	╜	╝	╞	╟

U+258x	▀	▁	▂	▃	▄	▅	▆	▇	█	▉	▊	▋	▌	▍	▎	▏

U+259x	▐	░	▒	▓	▔	▕	▖	▗	▘	▙	▚	▛	▜	▝	▞	▟

U+256x	╠	╡	╢	╣	╤	╥	╦	╧	╨	╩	╪	╫	╬	╭	╮	╯
 * ╔═══╗
 * ║╔╗/║
 * ║╚║╗║
 * ║/╚╝║
 * ╚═══╝
 * ╔═══╗ 
 * ║▞▞▞║
 * ║▞▞▞║
 * ║▞▞▞║
 * ╚═══╝
 * ╔═══╗
 * ║▛▞▜║
 * ║▞ ▞║
 * ║▙▞▟║
 * ╚═══╝
 */
void pt_card_back_at(int x, int y) {
    scr_pt(x,y,"\u2554\u2550\u2550\u2550\u2557");
    scr_pt(x,y+1,"\u2551\u2554\u2557/\u2551");
    scr_pt(x,y+2,"\u2551\u255A\u2551\u2557\u2551");
    scr_pt(x,y+3,"\u2551/\u255A\u255D\u2551");
    scr_pt(x,y+4,"\u255A\u2550\u2550\u2550\u255D");
}

void pt_card_back_clr_at(int x, int y, int color) {
    scr_set_clr(color, 0);
    scr_pt(x,y,"\u2554\u2550\u2550\u2550\u2557");
    scr_pt(x,y+1,"\u2551\u2554\u2557/\u2551");
    scr_pt(x,y+2,"\u2551\u255A\u2551\u2557\u2551");
    scr_pt(x,y+3,"\u2551/\u255A\u255D\u2551");
    scr_pt(x,y+4,"\u255A\u2550\u2550\u2550\u255D");

    scr_pt(x+1,y+1,"\u2554\u2557/");
    scr_pt(x+1,y+2,"\u255A\u2551\u2557");
    scr_pt(x+1,y+3,"/\u255A\u255D");
    scr_reset();
}

void pt_deck_stack_at(int x, int y) {
    pt_card_back_at(x,y);
    pt_card_back_at(x+1,y);
    pt_card_back_at(x+2,y);
}

void pt_deck_stack_clr_at(int x, int y, int color) {
    pt_card_back_clr_at(x,y,color);
    pt_card_back_clr_at(x+1,y,color);
    pt_card_back_clr_at(x+2,y,color);
}

void pt_deck_stackv_at(int x, int y) {
    pt_card_back_at(x,y);
    pt_card_back_at(x,y+1);
    pt_card_back_at(x,y+2);
}

void pt_deck_stackv_clr_at(int x, int y, int color) {
    pt_card_back_clr_at(x,y,color);
    pt_card_back_clr_at(x,y+1,color);
    pt_card_back_clr_at(x,y+2,color);
}

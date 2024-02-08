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

/*************************
 * Card printing functions
 *************************/
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
 *┌─   ─┐
 *│♥═7═╗│♥═A═╗
 * ║   ║ ║   ║
 * ║ 7 ║ ║ A ║
 * ║   ║ ║   ║
 *│╚═7═♥│╚═A═♥
 *└─   ─┘
 *   ⇩  (u21E9)   
 * ♥═10╗ ♥═A═╗
 * ║   ║ ║   ║
 * ║ 10║ ║ A ║
 * ║   ║ ║   ║
 * ╚═10♥ ╚═A═♥
 * └───┘ (u2514u2500u2500u2500u2518)
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

char get_suite_ch(int card) {
    if((card & CD_H) == CD_H) {
        return 'h'; 
    } else if((card & CD_S) == CD_S) {
        return 's';
    } else if((card & CD_C) == CD_C) {
        return 'c';
    } else if((card & CD_D) == CD_D) {
        return 'd';
    } 
    return '\0';
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
        scr_pt(x,y+1, "\u2551   \u2551");
        if(val == 10) {
            scr_pt(x,y, "%s\u2550%s\u2557", suite, value);
            scr_pt(x,y+2, "\u2551 %s\u2551", value);
            scr_pt(x,y+4, "\u255A\u2550%s%s",value, suite);
        } else {
            scr_pt(x,y, "%s\u2550%s\u2550\u2557", suite,value);
            scr_pt(x,y+2, "\u2551 %s \u2551", value);
            scr_pt(x,y+4, "\u255A\u2550%s\u2550%s",value, suite);
        }
        scr_pt(x,y+3, "\u2551   \u2551");
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
        fg = 250;
    } else {
        fg = 203;
    }
    bg = 0;

    if(value && suite) {
        scr_pt_clr(x,y+1,fg,0, "\u2551   \u2551");
        scr_pt_clr(x+1,y+1,fg,bg, "   ");
        scr_pt_clr(x,y+2,fg,0, "\u2551   \u2551");
        if(val == 10) {
            scr_pt_clr(x,y,fg,0,"%s\u2550%s\u2557", suite,value);
            scr_pt_clr(x+1,y+2,fg,bg, " %s",value);
            scr_pt_clr(x,y+4,fg,0, "\u255A\u2550%s%s",value, suite);
        } else {
            scr_pt_clr(x,y,fg,0,"%s\u2550%s\u2550\u2557", suite,value);
            scr_pt_clr(x+1,y+2,fg,bg, " %s ", value);
            scr_pt_clr(x,y+4,fg,0, "\u255A\u2550%s\u2550%s",value, suite);
        }
        scr_pt_clr(x,y+3,fg,0, "\u2551   \u2551");
        scr_pt_clr(x+1,y+3,fg,bg, "   ");
    }

    scr_reset();
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
    //pt_card_back_at(x+2,y);
}

void pt_deck_stack_clr_at(int x, int y, int color) {
    pt_card_back_clr_at(x,y,color);
    pt_card_back_clr_at(x+1,y,color);
    //pt_card_back_clr_at(x+2,y,color);
    scr_reset();
}

void pt_deck_stackv_at(int x, int y) {
    pt_card_back_at(x,y);
    pt_card_back_at(x,y+1);
    //pt_card_back_at(x,y+2);
}

void pt_deck_stackv_clr_at(int x, int y, int color) {
    pt_card_back_clr_at(x,y,color);
    pt_card_back_clr_at(x,y+1,color);
    //pt_card_back_clr_at(x,y+2,color);
    scr_reset();
}

void pt_card_spc_at(int x, int y, char c, char *suite) {
    scr_pt(x,y, "%s\u2550\u2550\u2550\u2557", suite);
    scr_pt(x,y+1, "\u2551   \u2551");
    scr_pt(x,y+2, "\u2551 %c \u2551", c);
    scr_pt(x,y+3, "\u2551   \u2551");
    scr_pt(x,y+4, "\u255A\u2550\u2550\u2550%s", suite);
}

void pt_card_spc_clr_at(int x, int y, char c, char *suite, uint8_t fg, uint8_t bg) {
    scr_pt_clr(x,y,fg,0,"%s\u2550\u2550\u2550\u2557", suite);
    scr_pt_clr(x,y+1,fg,0, "\u2551   \u2551");
    scr_pt_clr(x+1,y+1,fg,bg, "   ");
    scr_pt_clr(x,y+2,fg,0, "\u2551   \u2551");
    scr_pt_clr(x+1,y+2,fg,bg, " %c ", c);
    scr_pt_clr(x,y+3,fg,0, "\u2551   \u2551");
    scr_pt_clr(x+1,y+3,fg,bg, "   ");
    scr_pt_clr(x,y+4,fg,0, "\u255A\u2550\u2550\u2550%s", suite);
    scr_reset();
}

void pt_card_active(int x, int y) {
    /*   
     *┌─   ─┐
     *│♥═══╗│♥═══╗
     * ║   ║ ║   ║
     * ║ 7 ║ ║ A ║
     * ║   ║ ║   ║
     *│╚═══♥│╚═══♥
     *└─   ─┘
     */
}

/****************
 * Deck functions
 ****************/
Deck* create_card(int card) {
    Deck *newcard = malloc(sizeof(Deck));
    newcard->card = card;
    newcard->next = NULL;
    return newcard;
}

Deck* find_card(Deck **headref, int card) {
    Deck *result = *headref;
    if(!result) return NULL;

    while(result->card != card) {
        result = result->next;
        if(!result) break;
    }
    return result;
}

Deck* get_card_at(Deck **headref, int card) {
    Deck *result = *headref;
    int i;
    int count = count_deck(result);
    for(i = 0; i < count; i++) {
        if(i == card) break;
        result = result->next;
    }
    return result;
}

Deck* get_last_card(Deck *headref) {
    Deck *result = headref;
    if(!result) return NULL;
    while(result->next) {
        result = result->next;
    }
    return result;
}

void move_card_to_back(Deck **deckto,Deck **deckfrom, Deck* card) {
    Deck *tmp = *deckfrom;
    while(tmp && tmp != card) {
        tmp = tmp->next;
    }
    if(!tmp) return;
    tmp = remove_card(deckfrom, card);
    push_card_back(deckto, tmp);
}

void move_card_to(Deck **deckto,Deck **deckfrom, Deck* card) {
    Deck *tmp = *deckfrom;
    while(tmp && tmp != card) {
        tmp = tmp->next;
    }
    if(!tmp) return;
    /* I **could** move the card to the "to" deck anyway, if the card exists...
     * But then I would likely end up with multiple references to the same card,
     * and then get some crazy error when freeing up the memory */
    tmp = remove_card(deckfrom, card);
    push_card(deckto, tmp);
}

Deck* remove_card(Deck **headref, Deck *card) {
    Deck *result = *headref;
    Deck *prev = NULL;
    if(!result || !card) return NULL;
    if(*headref == card) {
        result = *headref;
        *headref = (*headref)->next;
        result->next = NULL;
        return result;
    }
    while(result != card && result) {
        prev = result;
        result = result->next;
    }
    if(result) {
        prev->next = result->next;
        result->next = NULL;
    }
    return result;
}

void shuffle_deck(Deck **headref, int rounds) {
    int i,j,n;
    int decksize = count_deck(*headref);
    Deck *tmp = NULL;
    for(i = 0; i < rounds; i++) {
        //Get random number from 0 to decksize - 1
        n = mt_rand(0,decksize - 1);
        tmp = *headref;
        j = 0;
        while(j != n) {
           tmp = tmp->next;
           j++;
        } 
        push_card(headref, remove_card(headref, tmp));
    }
}

void push_card(Deck **headref, Deck *card) {
    if(!card) return;
    if(!(*headref)) {
        *headref = card;
        return;
    }
    card->next = *headref;
    *headref = card;
}

void push_card_back(Deck **headref, Deck *card) {
    if(!card) return;
    if(!(*headref)) {
        (*headref) = card;
        return;
    }
    Deck *tmp = *headref;
    while(tmp->next) {
        tmp = tmp->next;
    }
    card->next = NULL;
    tmp->next = card;
}

void destroy_deck(Deck **headref) {
    Deck *tmp = *headref;
    while(*headref) {
        tmp = *headref;
        *headref = (*headref)->next;
        free(tmp);
    }
}

int count_deck(Deck *cards) {
    if(!cards) return 0;
    return (count_deck(cards->next) + 1);
}

void merge_sort_deck(Deck **headref, bool bysuite) {
    Deck *head = *headref;
    Deck *a = NULL;
    Deck *b = NULL;

    if(!head || !head->next) return;
    ft_bk_splt(head, &a, &b);
    merge_sort_deck(&a,bysuite);
    merge_sort_deck(&b,bysuite);
    *headref = sorted_merge(a,b,bysuite);
}

Deck* sorted_merge(Deck *a, Deck *b, bool bysuite) {
    Deck *result = NULL;
    if(!a) {
        return b;
    } else if(!b) {
        return a;
    }
    int aval = get_value(a->card);
    int bval = get_value(b->card);
    if(bysuite) {
        if((a->card & CD_H) == CD_H) aval += 13;
        if((a->card & CD_S) == CD_S) aval += 26;
        if((a->card & CD_D) == CD_D) aval += 39;
        if((b->card & CD_H) == CD_H) bval += 13;
        if((b->card & CD_S) == CD_S) bval += 26;
        if((b->card & CD_D) == CD_D) bval += 39;
    }
    if(aval <= bval) {
        result = a;
        result->next = sorted_merge(a->next, b, bysuite);
    } else {
        result = b;
        result->next = sorted_merge(a, b->next, bysuite);
    }
    return result;
}

void ft_bk_splt(Deck *source, Deck **frontref, Deck **backref) {
    Deck *fast = NULL;
    Deck *slow = NULL;
    slow = source;
    fast = source->next;
    while(fast) {
        fast = fast->next;
        if(fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *frontref = source;
    *backref = slow->next;
    slow->next = NULL;
}

Deck* create_std_deck(void) {
    Deck *result = NULL;
    int n = 1;
    for(n = 1; n <= 13; n++) {
        push_card(&result, create_card(get_card(n, 'h')));
        push_card(&result, create_card(get_card(n, 'c')));
        push_card(&result, create_card(get_card(n, 'd')));
        push_card(&result, create_card(get_card(n, 's')));
    }
    return result;
}

void draw_card(Deck **from, Deck **to) {
    if(!(*from)) return;
    int i = 0;
    int n = mt_rand(0, count_deck(*from) - 1);
    Deck *tmp = *from;
    while(i != n) {
        tmp = tmp->next;
        i++;
    }
    push_card(to, remove_card(from, tmp));
}

void draw_cards(Deck **from, Deck **to, int n) {
    if(!(*from)) return;
    int i = 0;
    while(i < n) {
        draw_card(from,to);
        i++;
    }
}

void draw_card_back(Deck **from, Deck **to) {
    if(!(*from)) return;
    int i = 0;
    int n = mt_rand(0, count_deck(*from) - 1);
    Deck *tmp = *from;
    while(i != n) {
        tmp = tmp->next;
        i++;
    }
    push_card_back(to, remove_card(from, tmp));
}

void draw_cards_back(Deck **from, Deck **to, int n) {
    if(!(*from)) return;
    int i = 0;
    while(i < n) {
        draw_card_back(from,to);
        i++;
    }
}

void add_cards(Deck **from, Deck **to) {
    if(!(*from)) return;
    while(*from) {
        draw_card(from, to);
    }
}

/*******************
 * Card Interactions
 *******************/
bool card_hearts(int card) {
    return ((card & CD_H) == CD_H);
}

bool card_diamonds(int card) {
    return ((card & CD_D) == CD_D);
}

bool card_spades(int card) {
    return ((card & CD_S) == CD_S);
}

bool card_clubs(int card) {
    return ((card & CD_C) == CD_C);
}

bool card_black(int card) {
    return (card_spades(card) || card_clubs(card));
}

bool card_red(int card) {
    return (card_hearts(card) || card_diamonds(card));
}

bool card_alt_color(int a, int b) {
    // Haha, clever trick here
    return (card_black(a) != card_black(b));
}

bool card_same_suit(int a, int b) {
    return (get_suite_ch(a) == get_suite_ch(b));
}

bool card_in_asc_sequence(int a, int b) {
    // Is the value of card 'a' exactly one higher than card 'b'?
    return (get_value(a) == get_value(b) + 1);
}

bool card_in_dec_sequence(int a, int b) {
    // Is the value of card 'a' exactly one lower than card 'b'?
    return (get_value(a) == get_value(b) - 1);
}

void turn_deck_facedown(Deck *deck) {
    if(!deck) return;
    Deck *tmp = deck;
    while(tmp) {
        remove_flag(&(tmp->card), CD_UP);
        tmp = tmp->next;
    }
}

bool card_faceup(int card) {
    return ((card & CD_UP) == CD_UP);
}

bool card_facedown(int card) {
    return (false == card_faceup(card));
}

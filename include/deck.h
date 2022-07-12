#ifndef DECK_H
#define DECK_H

#define CARD_WIDTH 6
#define CARD_HEIGHT 6

typedef enum {
    CD_NONE     = 1 << 0,
    CD_A        = 1 << 1,
    CD_2        = 1 << 2,
    CD_3        = 1 << 3,
    CD_4        = 1 << 4,
    CD_5        = 1 << 5,
    CD_6        = 1 << 6,
    CD_7        = 1 << 7,
    CD_8        = 1 << 8,
    CD_9        = 1 << 9,
    CD_10       = 1 << 10,
    CD_J        = 1 << 11,
    CD_Q        = 1 << 12,
    CD_K        = 1 << 13,
    CD_H        = 1 << 14,
    CD_S        = 1 << 15,
    CD_C        = 1 << 16,
    CD_D        = 1 << 17,
} DeckFlags;

struct Deck {
    int card;
    struct Deck *next;
};
typedef struct Deck Deck;

/*************************
 * Deck printing functions
 *************************/
char* get_suite(int card);
char get_suite_ch(int card);
int get_value(int card);
char* get_value_str(int card);
int get_card(int value, char suite);
void pt_card_at(int x, int y, int card);
void pt_card_clr_at(int x, int y, int card);
void pt_card_back_at(int x, int y);
void pt_card_back_clr_at(int x, int y, int color);
void pt_deck_stack_at(int x, int y);
void pt_deck_stack_clr_at(int x, int y, int color);
void pt_deck_stackv_at(int x, int y);
void pt_deck_stackv_clr_at(int x, int y, int color);
void pt_card_spc_at(int x, int y, char c, char *suite);
void pt_card_spc_clr_at(int x, int y, char c, char *suite, uint8_t fg, uint8_t bg); 
void pt_card_active(int x, int y);

/****************
 * Deck functions
 ****************/
Deck* create_card(int card);
Deck* find_card(Deck **headref, int card);
Deck* remove_card(Deck **headref, Deck *card);
void shuffle_deck(Deck **headref, int rounds);
void push_card(Deck **headref, Deck *card);
void destroy_deck(Deck **headref);
int count_deck(Deck *cards);
void merge_sort_deck(Deck **headref, bool bysuite);
Deck* sorted_merge(Deck *a, Deck *b, bool bysuite);
void ft_bk_splt(Deck *source, Deck **frontref, Deck **backref);

Deck* create_std_deck(void);
void draw_card(Deck **from, Deck **to);
void draw_cards(Deck **from, Deck **to, int n);
void add_cards(Deck **from, Deck **to); 

#endif

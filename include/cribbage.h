#ifndef CRIBBAGE_H
#define CRIBBAGE_H

typedef struct {
    Deck *hand;
    uint8_t score; //Total player score
    uint8_t pegA; //Score location of peg
    uint8_t pegB;
    uint8_t strategy; //0 none, 1 easy, 2 standard, 3 pro
} CribPlayer;

typedef struct {
    uint8_t qty;
    uint8_t pts;
    char *msg;
} CribScore;
/**********************
 * cribbage.c functions
 **********************/
CribPlayer* create_cribbage_player(void);
void destroy_cribbage_player(CribPlayer *player);
void cribbage_init(void);
void cribbage_loop(void);
bool cribbage_events(void);
void cribbage_draw(void);
void draw_board(void);
void cribbage_update(void);

CribScore* create_cribscore(int qty, int pts, char *msg,...);
void destroy_cribscore(CribScore *score);

int get_cribbage_card_value(int card);
int score_cribbage_hand(Deck *hand, Deck *flop);
CribScore* count_runs(Deck *hand, Deck *flop);
CribScore* count_flush(Deck *hand, Deck *flop);
CribScore* count_15s(Deck *hand, Deck *flop);
CribScore* count_pairs(Deck *hand, Deck *flop);
CribScore* count_nobs(Deck *hand, Deck *flop);


#endif

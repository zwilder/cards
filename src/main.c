#include <cards.h>

int main(int argc, char **argv) {
    scr_init();
    kb_init();

    /*
    int x;
    for(x = 1; x < 14; x++) {
        pt_card_clr_at((6*x)-6,0,get_card(x,'h'));
        pt_card_clr_at((6*x)-6,6,get_card(x,'s'));
        pt_card_clr_at((6*x)-6,12,get_card(x,'d'));
        pt_card_clr_at((6*x)-6,18,get_card(x,'c'));
    }
    */
    /*
    pt_card_back_at(6,24);
    pt_card_back_clr_at(12,24,43);
    pt_deck_stack_at(6,30);
    pt_deck_stack_clr_at(14,30,43);

    pt_deck_stackv_at(6,36);
    pt_deck_stackv_clr_at(12,36,43);
    */
    Deck *deck = create_std_deck();
    Deck *tmp = deck;
    int x = 0;
    int y = 0;
    while(tmp) {
        pt_card_clr_at(x,y,tmp->card);
        x += 6;
        if(x >= 78) {
            x = 0;
            y += 6;
        }
        tmp = tmp->next;
    }
    destroy_deck(&deck);
    scr_pt(0,g_screenH - 1,"Press any key to exit.");
    kb_get_char();
    scr_clear();

    scr_restore();
    kb_restore();
    return 0;
}

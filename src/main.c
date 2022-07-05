#include <cards.h>

void draw_temp(Deck *deck) {

}

int main(int argc, char **argv) {
    scr_init();
    kb_init();
    init_genrand(time(NULL));

    /* int x;
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
    Deck *tmp = NULL;
    bool running = true;
    char input = 0;
    int x = 0;
    int y = 0;
    tmp = deck;
    while(tmp) {
        pt_card_clr_at(x,y,tmp->card);
        x += 6;
        if(x >= 78) {
            x = 0;
            y += 6;
        }
        tmp = tmp->next;
    }
    scr_pt(0,g_screenH - 1,"Deck Size: %d. Press q to exit, r to shuffle.", count_deck(deck));
    while(running) {
        // Handle events
        input = kb_get_char();
        // Update
        switch (input) {
            case 'q':
                running = false;
                break;
            case 'r':
                shuffle_deck(&deck, 500);
                break;
            default:
                break;
        }
        // Draw
        x = 0;
        y = 0;
        scr_clear();
        tmp = deck;
        while(tmp) {
            pt_card_clr_at(x,y,tmp->card);
            x += 6;
            if(x >= 78) {
                x = 0;
                y += 6;
            }
            tmp = tmp->next;
        }
        scr_pt(0,g_screenH - 1,"Deck Size: %d. Press q to exit, r to shuffle.", count_deck(deck));
    }

    scr_clear();

    destroy_deck(&deck);
    scr_restore();
    kb_restore();
    return 0;
}

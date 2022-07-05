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
    Deck *hand = NULL;
    Deck *tmp = NULL;
    bool running = true;
    char input = 0;
    int x = g_screenW / 2;
    int y = g_screenH / 2;
    y -= 3;
    pt_card_spc_at(x - 12,y,'C',"\u2660");
    pt_card_spc_at(x - 6,y,'a',"\u2665");
    pt_card_spc_at(x,y,'r',"\u2666");
    pt_card_spc_at(x + 6,y,'d',"\u2663");
    pt_card_spc_at(x + 12,y,'s',"\u2660");
    scr_pt(0,g_screenH - 1,"Deck Size: %d. Press q to exit, r to shuffle deck, d to draw card.", count_deck(deck));
    while(running) {
        // Handle events
        input = kb_get_char();
        // Update
        switch (input) {
            case 'q':
                running = false;
                break;
            case 'r':
                /*
                while(hand) {
                    draw_card(&hand, &deck);
                }*/
                add_cards(&hand, &deck);
                shuffle_deck(&deck, 500);
                break;
            case 'd':
                draw_card(&deck, &hand);
                break;
            default:
                break;
        }
        // Draw
        x = 6;
        y = 0;
        scr_clear();
        tmp = hand;
        while(tmp) {
            pt_card_clr_at(x,y,tmp->card);
            x += 6;
            if(x >= 78) {
                x = 6;
                y += 6;
            }
            tmp = tmp->next;
        }
        x = count_deck(deck);
        if(x > 1) {
            pt_deck_stack_clr_at(0,0,43);
        } else if (x == 1) {
            pt_card_back_clr_at(0,0,43);
        }
        scr_pt(0,g_screenH - 1,"Deck Size: %d. Press q to exit, r to shuffle deck, d to draw card.", count_deck(deck));
    }

    scr_clear();

    destroy_deck(&deck);
    destroy_deck(&hand);
    scr_restore();
    kb_restore();
    return 0;
}

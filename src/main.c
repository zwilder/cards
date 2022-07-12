#include <cards.h>

void draw_temp(Deck *deck) {

}

int main(int argc, char **argv) {
    scr_init();
    kb_init();
    init_genrand(time(NULL));

    Deck *deck = create_std_deck();
    Deck *hand = NULL;
    Deck *flop = NULL;
    Deck *tmp = NULL;
    bool running = true;
    char input = 0;
    int x = g_screenW / 2;
    int y = g_screenH / 2;
    y -= 3;
    pt_card_back_clr_at(x - 18,y,43);
    pt_card_spc_clr_at(x - 12,y,'C',"\u2660",250,0);
    pt_card_spc_clr_at(x - 6,y,'a',"\u2665",203,0);
    pt_card_spc_clr_at(x,y,'r',"\u2666",250,0);
    pt_card_spc_clr_at(x + 6,y,'d',"\u2663",203,0);
    pt_card_spc_clr_at(x + 12,y,'s',"\u2660",250,0);
    pt_card_back_clr_at(x + 18,y,43);
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
            case 'd':
                add_cards(&hand, &deck);
                add_cards(&flop, &deck);
                shuffle_deck(&deck, 500);
                draw_cards(&deck, &hand, 4);
                draw_card(&deck, &flop);
                break;
            case 's':
                merge_sort_deck(&hand,false);
                break;
            case 'a':
                merge_sort_deck(&hand,true);
                break;
            default:
                break;
        }
        // Draw
        x = 7;
        y = 0;
        scr_clear();
        tmp = hand;
        while(tmp) {
            pt_card_clr_at(x,y,tmp->card);
            x += 6;
            if(x >= 78) {
                x = 7;
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
        pt_card_clr_at(0,6,flop->card);
        score_cribbage_hand(hand, flop);
        scr_pt(0,g_screenH - 1,"Deck Size: %d. Press q to exit, r to shuffle deck, d to draw card.", count_deck(deck));
    }

    scr_clear();

    destroy_deck(&flop);
    destroy_deck(&deck);
    destroy_deck(&hand);
    scr_restore();
    kb_restore();
    return 0;
}

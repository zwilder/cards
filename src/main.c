#include <cards.h>

int main(int argc, char **argv) {
    scr_init();
    kb_init();

    scr_pt(0,0,"Hello, world!");
    scr_pt(0,5,"Press any key to exit.");
    kb_get_char();
    scr_clear();

    scr_restore();
    kb_restore();
    return 0;
}

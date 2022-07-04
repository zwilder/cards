#include <cards.h>

struct termios g_oldkbflags;

void kb_init(void) {
    struct termios newkbflags;
    if(tcgetattr(0, &g_oldkbflags)>=0) {
        tcgetattr(0, &newkbflags);
        newkbflags.c_lflag &= ~ICANON;
        newkbflags.c_lflag &= ~ECHO;
        newkbflags.c_cc[VMIN] = 1;
        newkbflags.c_cc[VTIME] = 0;
        tcsetattr(0, TCSANOW, &newkbflags);
    }
}

void kb_restore(void) {
    tcsetattr(0,TCSADRAIN,&g_oldkbflags);
}

char kb_get_char(void) {
    char c;
    read(STDIN_FILENO,&c,1);
    return c;
}

char* kb_get_str(void) {
    char* input = malloc(1000 * sizeof(char));
    kb_restore();
    printf("\x1b[?25h\x1b[3 q");
    scanf("%[^\n]s",input);
    printf("\x1b[?25l\x1b[0 q");
    kb_init();
    return input;
}

char* kb_get_str_at(int x, int y) {
    scr_set_curs(x,y);
    return kb_get_str();
}

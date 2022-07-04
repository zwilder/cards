#ifndef INPUT_H
#define INPUT_H

/*********
 * Globals
 *********/
extern struct termios g_oldkbflags;

/*******************
 * input.c functions
 *******************/
void kb_init(void);
void kb_restore(void);
char kb_get_char(void);
char* kb_get_str(void);
char* kb_get_str_at(int x, int y);

#endif

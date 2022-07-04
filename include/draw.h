#ifndef DRAW_H
#define DRAW_H

/*********
 * Globals
 *********/
extern int g_screenW;
extern int g_screenH;
/******************
 * draw.c functions
 ******************/
void scr_init(void);
void scr_restore(void);
void scr_reset(void);
void scr_clear(void);
void scr_pt_char(int x, int y, char c);
void scr_pt_clr_char(int x, int y, uint8_t fg, uint8_t bg, char c);
void scr_pt(int x, int y, char *fstr,...);
void scr_pt_clr(int x, int y, uint8_t fg, uint8_t bg, char *fstr,...);
void scr_set_clr(uint8_t fg, uint8_t bg);
void scr_set_curs(int x, int y);

#endif

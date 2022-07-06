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
void scr_set_style(int style);

/*******
 * Flags
 *******/
typedef enum {
    ST_NONE         = 1 << 0,
    ST_ULINE        = 1 << 1,
    ST_ITALIC       = 1 << 2,
    ST_BLINK        = 1 << 3,
    ST_BOLD         = 1 << 4,
    ST_STRIKE       = 1 << 5,
    ST_DIM          = 1 << 6
} StyleFlags;

#endif

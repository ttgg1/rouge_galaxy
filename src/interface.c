#include "interface.h"
#include "utils.h"
#include "vecMath.h"
#include <ncurses.h>

interface_t in_create(uint8_t cols, uint8_t lines) {
  interface_t res;

  res.w = cols;
  res.h = lines;

  // current terminal screen size
  uint8_t term_w, term_h;

  getmaxyx(stdscr, term_h, term_w);

  // check if Window fits inside the Terminal
  if (res.w > term_w) {
    debug_print("Window-Width doesnt fit in Terminal !");
    res.w = term_w;
  }
  if (res.h > term_h) {
    debug_print("Window-height doesnt fit in Terminal !");
    res.h = term_h;
  }

  res.pixels = malloc(res.w * res.h * sizeof(char));

  // intit ncurses
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  // create ncurses Window
  res.win = newwin(res.w, res.h, 0, 0);

  return res;
}

void in_destroy(interface_t *in) {
  free(in->pixels);
  delwin(in->win);
  endwin();
}

void in_drawAt(interface_t *in, chtype c, ivec2_t pos) {
  // save current curser pos
  ivec2_t old_pos;
  getyx(in->win, old_pos.y, old_pos.x);

  printf("old_x: %d, old_y: %d\n", old_pos.x, old_pos.y);

  // move cursor and draw char
  mvwaddch(in->win, pos.y, pos.x, c);

  // move back to orgiginal position
  wmove(in->win, old_pos.y, old_pos.x);
}
void in_drawRefresh(interface_t *in) { wrefresh(in->win); }

void in_clearScreen(void) { wrefresh(stdscr); }

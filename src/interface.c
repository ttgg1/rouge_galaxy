#include "interface.h"
#include "vecMath.h"
#include <ncurses.h>

interface_t in_create(uint8_t cols, uint8_t lines) {
  interface_t res;

  res.w = cols;
  res.h = lines;

  res.pixels = malloc(res.w * res.h * sizeof(char));

  // intit ncurses
  initscr();
  noecho();
  keypad(stdscr, TRUE);

  // create ncurses Window
  res.win = newwin(res.w, res.h, 0, 0);

  return res;
}

void in_destroy(interface_t *in) { free(in->pixels); }

void in_drawAt(interface_t *in, chtype c, ivec2_t pos) {
  // save current curser pos
  ivec2_t old_pos;
  getyx(in->win, old_pos.y, old_pos.x);

  // move cursor and draw char
  mvwaddch(in->win, pos.y, pos.x, c);

  // move back to orgiginal position
  wmove(in->win, old_pos.y, old_pos.x);
}
void in_drawRefresh(interface_t *in) { wrefresh(in->win); }

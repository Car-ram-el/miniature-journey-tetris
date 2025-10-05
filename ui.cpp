#include <ncurses.h>
#include <string>
#include <iostream>

#include "logic.h"

using namespace std;

void init_ncurses() {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
  timeout(0);
  curs_set(0); // invisible cursor
}

void init_colors() {
  start_color();

  init_pair(1, COLOR_GREEN,  COLOR_BLACK); // tetro
  init_pair(2, COLOR_YELLOW, COLOR_BLACK);
  init_pair(3, COLOR_BLUE,   COLOR_BLACK);
  init_pair(4, COLOR_MAGENTA,COLOR_BLACK);
  init_pair(5, COLOR_CYAN,   COLOR_BLACK);
  init_pair(6, COLOR_WHITE,  COLOR_BLACK);
  init_pair(7, COLOR_RED,    COLOR_BLACK);
  init_pair(8, COLOR_GREEN,  COLOR_GREEN); // filled line
  init_pair(9, COLOR_WHITE,  COLOR_WHITE); // wall
}

void render_cur_piece() {
  chmpn(y, 0, 4) chmpn(x, 0, 4) {
    if (tetrominoes[curPiece][rotate(x, y, curRotation)] == 'X') {
      attron(COLOR_PAIR(curPiece + 1));
      mvaddch(curY + y, curX + x, ' ' | A_REVERSE);
      attroff(COLOR_PAIR(curPiece + 1));
    }
  }
}

void render_score() {
  mvprintw(fieldHeight / 2, fieldWidth + 5, "Let's go: %d", score);
}

void render_board() {
  chmpn(y, 0, fieldHeight) chmpn(x, 0, fieldWidth) {
    unsigned char cell = board[y * fieldWidth + x];
    if (!cell) mvaddch(y, x, ' ');
    else {
      attron(COLOR_PAIR(cell));
      mvaddch(y, x, ' ' | A_REVERSE); // prints a colored block
      attroff(COLOR_PAIR(cell));
    }
  }
  render_cur_piece();
  render_score();
  refresh();
}

void get_user_input() {
  int ch = getch();
  switch (ch){
  case 'r':
    curRotation += (does_piece_fit(curPiece, curRotation + 1, curX, curY));
    break;
  case KEY_DOWN:
    curY += (does_piece_fit(curPiece, curRotation, curX, curY + 1));
    break;
  case KEY_LEFT:
    curX -= (does_piece_fit(curPiece, curRotation, curX - 1, curY));
    break;
  case KEY_RIGHT:
    curX += (does_piece_fit(curPiece, curRotation, curX + 1, curY));
    break;
  case 'q':
    gameOver = true;
  }
}

void game_loop() {
  while (!gameOver) {
    handle_game_timing();
    get_user_input();
    handle_user_input();
    render_board();
   
    refresh();
  }
}

int main() {
  init_ncurses();
  init_colors();
  init_tetrominoes();
  init_board();

  game_loop();

  endwin();
  printf("gg!\nScore: %d\n", score);
}
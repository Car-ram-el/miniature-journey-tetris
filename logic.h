#include <stdio.h>
#include <stdbool.h>
#include <string>

using namespace std;

#define chmpn(i,l,r) for(int i=l; i<r; ++i)
#define prime(i,r,l) for(int i=r; i>=l; --i)
#define rgx(a,b) for(auto &a:b)

const int fieldWidth = 25, fieldHeight = 20;
extern unsigned char *board; // map char (logic) -> color (ui)
const int TOTAL_TETROMINOES = 7;
extern string tetrominoes[TOTAL_TETROMINOES];
extern bool gameOver, moveTetroDown;

extern int curPiece, curRotation, curX, curY, speed, speedCounter, score;

void init_tetrominoes();

int rotate(int x, int y, int r);
bool does_piece_fit(int tetro, int rotation, int tx, int ty);
void init_board();
void handle_game_timing();
void handle_user_input();
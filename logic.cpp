#include <stdlib.h>
#include <iostream>
#include <thread>
#include <vector>
#include "logic.h"

using namespace std;

unsigned char *board = nullptr; // {wall 0, tetro 1-7, 8, wall 9}
string tetrominoes[TOTAL_TETROMINOES];
bool gameOver = false,
     moveTetroDown = false;
vector<int> filledLines;
int curPiece = rand() % 7, // 0-6
    curRotation = 0, curX = fieldWidth / 2, curY = 0,
    speed = 20, // ticks in 1s 
    speedCounter = 0,
    totalTetros = 0,
    score = 0;

void init_board() {
  board = new unsigned char[fieldWidth * fieldHeight];
  // boundary (leave top for pieces)
  chmpn(i, 0, fieldHeight) chmpn(j, 0, fieldWidth) {
    board[i * fieldWidth + j] = (j == 0 || j == fieldWidth - 1 || i == fieldHeight - 1) ? 9 : 0;
  }
}

void init_tetrominoes() {
  // using 1d array for pieces
  tetrominoes[0].append("..X.");
  tetrominoes[0].append("..X.");
  tetrominoes[0].append("..XX");
  tetrominoes[0].append("....");

  tetrominoes[1].append("...X");
  tetrominoes[1].append("...X");
  tetrominoes[1].append("...X");
  tetrominoes[1].append("...X");

  tetrominoes[2].append("..XX");
  tetrominoes[2].append("..XX");
  tetrominoes[2].append("....");
  tetrominoes[2].append("....");

  tetrominoes[3].append("...X");
  tetrominoes[3].append("..XX");
  tetrominoes[3].append("..X.");
  tetrominoes[3].append("....");

  tetrominoes[4].append("...X");
  tetrominoes[4].append("...X");
  tetrominoes[4].append("..XX");
  tetrominoes[4].append("....");

  tetrominoes[5].append("..X.");
  tetrominoes[5].append("..XX");
  tetrominoes[5].append("...X");
  tetrominoes[5].append("....");

  tetrominoes[6].append("...X");
  tetrominoes[6].append("..XX");
  tetrominoes[6].append("...X");
  tetrominoes[6].append("....");
}

int rotate(int x, int y, int r) {
  // converting 2d blocks to 1d array, according to degree of rotation
  switch (r % 4) {
    case 0:
      //   [0  1  2   3]
      //   [4  5  6   7]
      //   [8  9 10  11]
      //   [12 13 14 15]
      return 4*y + x;

    case 1:
      //   [12  8  4  0]
      //   [13  9  5  1]
      //   [14 10  6  2]
      //   [15 11  7  3]
      return 4*(3 - x) + y;

    case 2:
      //   [15 14 13 12]
      //   [11 10  9  8]
      //   [ 7  6  5  4]
      //   [ 3  2  1  0]
      return 15 - 4*y - x;

    case 3:
      //   [ 3  7 11 15]
      //   [ 2  6 10 14]
      //   [ 1  5  9 13]
      //   [ 0  4  8 12]
      return 4*x + (3 - y);
  }
  return 0;
}

bool is_valid_ind(int x, int y) {
  return x >= 0 && x < fieldWidth && y >= 0 && y < fieldHeight;
}

bool does_piece_fit(int tetro, int rotation, int fieldX, int fieldY) {
  chmpn(y, 0, 4) chmpn(x, 0, 4) {
    int pieceIndex = rotate(x, y, rotation),
        newX = fieldX + x, newY = fieldY + y,
        fieldIndex = newY * fieldWidth + newX;

    if (tetrominoes[tetro][pieceIndex] != '.' && 
        is_valid_ind(newX, newY) &&
        board[fieldIndex]) 
          return false; // collision
  }
  return true;
}

void handle_game_timing() {
  this_thread::sleep_for(50ms); // game tick
  speedCounter++;
  moveTetroDown = (speedCounter == speed); // piece down if ticks
}

void handle_user_input() {
  if (!moveTetroDown) return; // during time interval

  speedCounter = 0;
  if (does_piece_fit(curPiece, curRotation, curX, curY + 1)) {curY++; return;}

  // collided
  // lock the piece
  chmpn(y, 0, 4) chmpn(x, 0, 4)
    if (tetrominoes[curPiece][rotate(x, y, curRotation)] != '.') 
      board[(curY + y) * fieldWidth + (curX + x)] = curPiece + 1; // set in init colors(ui)
  totalTetros++;
  if (totalTetros % 10 == 0) speed--;

  // check horizontal lines- only the 4 the current tetro occupied
  chmpn(y, 0, 4) {
    bool isLine = true;
    int newY = curY + y;
    if (newY >= fieldHeight - 1) break;

    chmpn(x, 1, fieldWidth - 1)
      if (board[newY * fieldWidth + x] == 0) {isLine = false; break;}

    if (isLine) {
      chmpn(x, 1, fieldWidth - 1) board[newY * fieldWidth + x] = 8; // =
      filledLines.push_back(curY + y);
    }
  }

  // score
  score += 25;
  switch (filledLines.size()){
    case 1: score += 100; break;
    case 2: score += 300; break;
    case 3: score += 500; break;
    case 4: score += 800; break;
    default: break;
  }

  // clear lines
  this_thread::sleep_for(400ms); // delay
  rgx(filledLine, filledLines) {
    chmpn(x, 1, fieldWidth - 1) {
      prime(y, filledLine, 1)
        // shift lines below
        board[y * fieldWidth + x] = board[(y - 1) * fieldWidth + x];
      board[x] = 0;
    }
  }
  filledLines.clear();

  // next piece
  curPiece = rand() % 7, curRotation = 0, curX = fieldWidth/2, curY = 0;
  
  gameOver = !does_piece_fit(curPiece, curRotation, curX, curY);
}
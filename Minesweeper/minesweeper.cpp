#include <iostream>
#include <fstream>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <cassert>
#include "minesweeper.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* pre-supplied function to load mine positions from a file */
void load_board(const char *filename, char board[9][9]) {

  cout << "Loading board from file '" << filename << "'... ";

  ifstream in(filename);
  if (!in)
    cout << "Failed!" << endl;
  assert(in);

  char buffer[512];

  int row = 0;
  in.getline(buffer,512);
  while (in && row < 9) {
    for (int n=0; n<9; n++) {
      assert(buffer[n] == '.' || buffer[n] == '*' || buffer[n] == ' ' || buffer[n] == '?' || isdigit(buffer[n]));
      board[row][n] = buffer[n];
    }
    row++;
    in.getline(buffer,512);
  }

  cout << ((row == 9) ? "Success!" : "Failed!") << endl;
  assert(row == 9);
}

/* internal helper function */
void print_row(const char *data, int row) {
  cout << (char) ('A' + row) << "|";
  for (int i=0; i<9; i++) 
    cout << ( (data[i]=='.') ? ' ' : data[i]);
  cout << "|" << endl;
}

/* pre-supplied function to display a minesweeper board */
void display_board(const char board[9][9]) {
  cout << "  ";
  for (int r=0; r<9; r++) 
    cout << (char) ('1'+r);
  cout <<  endl;
  cout << " +---------+" << endl;
  for (int r=0; r<9; r++) 
    print_row(board[r],r);
  cout << " +---------+" << endl;
}

/* pre-supplied function to initialise playing board */ 
void initialise_board(char board[9][9]) {
  for (int r=0; r<9; r++)
    for (int c=0; c<9; c++)
      board[r][c] = '?';
}

/* add your functions here */

//returns true if all non-mines fields have been uncovered
bool is_complete(const char mines[9][9], const char revealed [9][9]) {
  
  //go through both boards
  for (int row = 0; row < 9; row++) {
    for (int col = 0; col < 9; col++) {
      //if mines has a mine, revealed must have a mine
      if (mines[row][col] == '*' && revealed[row][col] != '*')
	return false;
      //if mines has no mine, revealed must have a digit(1-8) or a space
      if (mines[row][col] == ' ' && (revealed[row][col] != ' ' && !isdigit(revealed[row][col])))
	return false;
    }
  }
  return true;
}

//returns number of mines around a particular square
int count_mines(const char* pos, const char board[9][9]) {

  int mines(0);
  //convert pos into row and col number
  int row = pos[0] - 'A', col = pos[1] - '1';

  //count all mines in row below and above
  for (int n = -1; n < 2; n++)
    for (int i = 0; i < 3; i++)
      if (board[row + n][(col - 1) + i] == '*') {
	if ((row + n) < 0 || (row + n) > 8 || ((col - 1) + i) < 0 || ((col - 1) + i) > 8)
	  continue;
	else
	  mines++;
      }
  
  return mines;
}

//helper converting ints to char
char to_char(int& num) {

  char ch;
  switch (num) {
  case 0: ch = '0'; break;
  case 1: ch = '1'; break;
  case 2: ch = '2'; break;
  case 3: ch = '3'; break;
  case 4: ch = '4'; break;
  case 5: ch = '5'; break;
  case 6: ch = '6'; break;
  case 7: ch = '7'; break;
  case 8: ch = '8'; break;
  }
  return ch;  
}

//helper converting the row to a char
char row_to_char(int& num) {

  char ch;
  switch (num) {
  case 0: ch = 'A'; break;
  case 1: ch = 'B'; break;
  case 2: ch = 'C'; break;
  case 3: ch = 'D'; break;
  case 4: ch = 'E'; break;
  case 5: ch = 'F'; break;
  case 6: ch = 'G'; break;
  case 7: ch = 'H'; break;
  case 8: ch = 'I'; break;
  }
  return ch;  
}

//helper converting ints to char
char col_to_char(int& num) {

  char ch;
  switch (num) {
  case 0: ch = '1'; break;
  case 1: ch = '2'; break;
  case 2: ch = '3'; break;
  case 3: ch = '4'; break;
  case 4: ch = '5'; break;
  case 5: ch = '6'; break;
  case 6: ch = '7'; break;
  case 7: ch = '8'; break;
  case 8: ch = '9'; break;
  }
  return ch;  
}

//helper that uncovers blank squares (without adjacent mines)
void uncover(int row, int col, const char mines[9][9], char board[9][9]) {

  //if row or colum get out of range, return
  if (row > 8 || row < 0 || col > 8 || col < 0)
    return;

  //if field has already been uncovered, return
  if (board[row][col] != '?' && board[row][col] != '0')
    return;

  //if field is a mine, return
  if (mines[row][col] == '*')
    return;
  
  //update position
  char pos[3] = {pos[0] = row_to_char(row), pos[1] = col_to_char(col), pos[3] = '\0'};

  //base case: if adjacent mine, alter board and return
  int num = count_mines(pos, mines);
  if (num > 0) {
    board[row][col] = to_char(num);
    return;
  }
  
  //uncover pos and all positios around pos;
  board[row][col] = ' ';
  for (int n = -1; n <= 1; n++)
    for (int i = 0; i < 3; i++) 
      uncover((row + n), (col - 1 + i), mines, board);
  
  return;
}

//makes a move and returns result of the move
MoveResult make_move(char* pos, const char mines[9][9], char board[9][9]) {

  MoveResult outcome(VALID_MOVE);
  //convert pos into row and column
  int row = pos[0] - 'A', col = pos[1] - '1';

  //check invalid move
  if (row < 0 || row > 8 || col < 0 || col > 8
      || strlen(pos) > 3 || strlen(pos) < 2 || (strlen(pos) == 3 && pos[2] != '*')) {
    outcome = INVALID_MOVE;
    return outcome;
  }

  //check redundant move
  if (board[row][col] != '?') {
    outcome = REDUNDANT_MOVE;
    return outcome;
  }

  //set signal if applicable
  if (strlen(pos) == 3) {
    board[row][col] = '*';
    return outcome;
  }

  //check blown up and alter board if applicable
  if (mines[row][col] == '*') {
    outcome = BLOWN_UP;
    board[row][col] = '*';
    return outcome;
  }

  //check if board is solved and alter if applicable
  int num = count_mines(pos, mines);
  board[row][col] = to_char(num);
  if (is_complete(mines, board)) {
    outcome = SOLVED_BOARD;
    return outcome;
  }
    
  //valid move and alter board calling recursive helper if necessary
  if (board[row][col] != '0')
    return outcome;
  uncover(row, col, mines, board);
  return outcome;
}

//helper to find a risk-free move
bool get_move(const char board[9][9], char* move, const int& row, const int& col) {

  //get number of mines
  int m_counter = board[row][col] - '0';

  //Declare/initialize counter and position holder
  int stars(0), qms(0);
  char pos[3] = {'\0'};

  //iterate through adjacent squares and increment counters
  for (int r = row - 1; r <= row + 1; r++) {
    for (int c = col - 1; c <= col + 1; c++) {
      if (r < 0 || r > 8 || c < 0 || c > 8)
	continue;
      if (r == row && c == col)
	continue;
      if (board[r][c] == '*')
	stars++;
      if (board[r][c] == '?') {
	qms++;
	pos[0] = row_to_char(r);
	pos[1] = col_to_char(c);
	pos[2] ='\0';
      }
    }
  }

  //if mines counter == stars, unreveal position
  if (m_counter == stars && *pos != '\0') {
    strcpy(move, pos);
    return true;
  }

  //if mines counter - stars equals qms, flag position
  else if ((m_counter - stars) == qms && *pos != '\0') {
    strcpy(move, pos);
    strcat(move, "*");
    return true;
  }

  //otherwise return false
  return false;
}

//finds safe move or returns empty string move if none available 
bool find_safe_move(const char board[9][9], char* move) {

  *move = '\0';

  //go through board to find move
  for (int row = 0; row < 9; row++)
    for (int col = 0; col < 9; col++)
      if (get_move(board, move, row, col))
	  return true;

  //if none found, return false
  return false;
}

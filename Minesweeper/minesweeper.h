#ifndef MINESWEEPER_H
#define MINESWEEPER_H

enum MoveResult { INVALID_MOVE=-3, REDUNDANT_MOVE=-2, BLOWN_UP=-1, SOLVED_BOARD=1, VALID_MOVE=0 };

void load_board(const char *filename, char board[9][9]);
void display_board(const char board[9][9]);
void initialise_board(char board[9][9]);

//returns true if board is solved
bool is_complete(const char mines[9][9], const char revealed [9][9]);

//returns number of mines around a particular square
int count_mines(const char* pos, const char board[9][9]);

//makes a move and returns result of the move
MoveResult make_move(char* pos, const char mines[9][9], char board[9][9]);

//finds safe move or returns empty string move if none available
bool find_safe_move(const char board[9][9], char* move);



#endif

#include <iostream>
#include <cctype>
#include <cstring>
#include "playfair.h"

using namespace std;

//transforms input into uppercase/digit sequence without punct or spaces
void prepare(const char* input, char* output) {

  int counter(0);
  while (*input) {
  
  //if input isalnum, copy into output
    if (isalnum(*input)) {
      counter++;
      if (isdigit(*input)) {
	*output++ = *input;
      }
      else if (isalpha(*input)) {
	*output++ = toupper(*input);
      }
    }
    input++;
  }

  //add X at end of output if length is odd
  if (counter % 2)
    strcat(output++, "X");
  *output = '\0';
}

//helper for grid() that checks if a char alraedy exists in a word
bool occurs_before(const char* str, char letter, int pos) {

  //go through string up to position and see if char is already in there
  for (int i = 0; i < pos; i++) {
    if (letter == str[i])
      return true;
  }
  return false; 
}

//fill 6x6 grid with codeword and letters/digits
void grid(const char* codeword, char board[6][6]) {

  int size = strlen(codeword);
  int row(0), col(0);

  //input elements of codeword in order into array
  board[row][col++] = codeword[0];
  for (int i = 1; i < size; i++) {
    if (!occurs_before(codeword, codeword[i], i)) {
	board[row][col] = codeword[i];
	if (col == 5) {
	  row++;
	  col = -1;
	}
	col++;
    }
  }
  //fill up remaining array with alphabet and digits
  char next_char = 'A';
  while (row < 6) {

    if (!occurs_before(codeword, next_char, size)) {
      board[row][col] = next_char;
      if (col == 5) {
	row++;
	col = -1;
      }
      col++;
    }
    next_char++;
    if (!isalnum(next_char))
	next_char = '0';
  }
}

//helper to find the row of a char in the board
int find_row(const char board[6][6], char ch) {

  //go through board and return row of ch
  for (int r = 0; r < 6; r++) {
    for (int c = 0; c < 6 ; c++) {
      if (board[r][c] == ch)
	return r;
    }
  }
  cerr << "error find_row\n";
  return -1;
}

//helper to find the col of a char in the board
int find_col(const char board[6][6], char ch) {

  //go through board and return row of ch
  for (int r = 0; r < 6; r++) {
    for (int c = 0; c < 6 ; c++) {
      if (board[r][c] == ch)
	return c;
    }
  }
  cerr << "error find_row\n";
  return -1;
}

//encoded single bigram using a given board
void bigram(const char board[6][6], const char inchar1, const char inchar2,
	    char& outchar1, char& outchar2) {

  //assign outchar1
  int row = find_row(board, inchar1);
  int col = find_col(board, inchar2);
  outchar1 = board[row][col];

  //assign outchar2
  row = find_row(board, inchar2);
  col = find_col(board, inchar1);
  outchar2 = board[row][col];
}

//encodes prepare into encoded using board
void encode(const char board[6][6], const char* prepared, char* encoded) {

  //if prepared is through, return
  if (!*prepared) {
    *encoded = '\0';
    return;
  }

  //grab two chars, create bigram, put into encoded
  bigram(board, *prepared, *(prepared + 1), *encoded, *(encoded + 1));

  //repeat
  return encode(board, &prepared[2], &encoded[2]);
}

//decodes encoded message
void decode(const char board[6][6], const char* encoded, char* decoded) {
  encode(board, encoded, decoded);
}

#ifndef PLAYFAIR_H
#define PLAYFAIR_H

//goes through input and returns uppercase string without space and punctuation
void prepare(const char* input, char* output);

//returns codeword in grid
void grid(const char* codeword, char board[6][6]);

//returns bigram for two input chars
void bigram(const char board[6][6], const char inchar1, const char inchar2,
	    char& outchar1, char& outchar2);

//encodes prepared into encoded using board
void encode(const char board[6][6], const char* prepared, char* encoded);

//decoes encoded into decoded using board
void decode(const char board[6][6], const char* encoded, char* decoded);

#endif

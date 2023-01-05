#ifndef GOGEN_H
#define GOGEN_H

/*  Pre-supplied functions for loading and printing board and word list */

char **load_board(const char *filename);
bool save_board(char **board, const char *filename);
void print_board(char **board);

char **load_words(const char *filename);
void print_words(char **words);

void delete_board(char **board);
void delete_words(char **words);

/* -----------8<-------------8<------------8<--------------8<------------ */

/* add your function prototypes here */

//sets row and col of a char in a board, or returns false and sets both to -1
bool get_position(char** board, char ch, int& row, int& column);

//returns true if a board is a solution to provided words
bool valid_solution(char** board, char**  words);

//updates mutually board and mask depending on ch
void update(char** board, char ch, Mask& mask);

//interesects two masks with d-neighborhood to one another
void neighbourhood_intersect(Mask& one, Mask& two);

//attempts to solve a board and returns it or returns false if no solution found
bool solve_board(char** board, char** words);


#endif

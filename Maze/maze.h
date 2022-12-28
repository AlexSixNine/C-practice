#ifndef MAZE_H
#define MAZE_H

#include <string>

/* You are pre-supplied with the functions below. Add your own 
   function prototypes to the end of this file. */

/* helper functions to allocate and deallocate dynamic 2D arrays */
char **allocate_2D_array(int rows, int columns);
void deallocate_2D_array(char **m, int rows);

/* pre-supplied function to load a maze from a file */
char **load_maze(const char *filename, int &height, int &width);

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width);

//finds row and column of ch in maze or sets both to -1 if not found
bool find_marker(char ch, char** maze, int height, int width, int& row, int& column);

//returns true if path is a valid solution for maze
bool valid_solution(const char* path, char** m, int height, int width);

//returns a string with the solution to a given maze
std::string find_path(char** m, int height, int width, char start, char end);

#endif

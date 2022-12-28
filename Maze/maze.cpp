#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include "maze.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

//finds coorindates of ch in maze or sets both to -1
bool find_marker(char ch, char** maze, int height, int width, int& row, int& column) {

  //loop through maze and find ch
  for (int r = 0; r < height; r++) {
    for (int c = 0; c < width; c++) {
      if (maze[r][c] == ch) {
	row = r;
	column = c;
	return true;
      }
    }
  }
  //set coordinates to -1 and return false if not found
  row = column = -1;
  return false;
}

//helper to translate the movement and return coordinates according to path
bool translate_movement(const char ch, int& row, int& col) {

  switch(ch) {
  case 'N': row -= 1; return true; break;
  case 'S': row += 1; return true; break;
  case 'W': col -= 1; return true; break;
  case 'E': col += 1; return true; break;
  default: return false;
  }
}

//returns true if path is a valid solution for maze
bool valid_solution(const char* path, char** m, int height, int width) {

  int row, col;
  //find and start at >
  if (!find_marker('>', m, height, width, row, col))
    return false;

  //take path char by char and translate movement
  while (*path) {
    if (!translate_movement(*path, row, col))
	return false;
    //check if new field is valid
    if (row < 0 || row > (height - 1) || col < 0 || col > (width - 1))
      return false;
    if (m[row][col] == '-' || m[row][col] == '+' || m[row][col] == '|' || m[row][col] == 'U')
      return false;
    //if char is X, return true
    if (m[row][col] == 'X')
      return true;
    path++;
  }

  //if sequence is over and no exit has been found, return false
  return false;
}

//helper to translate the movement and return coordinates according to path
void reverse_movement(const char ch, int& row, int& col) {

  switch(ch) {
  case 'N': row += 1;  break;
  case 'S': row -= 1;  break;
  case 'W': col += 1;  break;
  case 'E': col -= 1;  break;
  }
}

//helper to access if a move is valid
bool valid_move(char** m, int row, int col, int height, int width) {

  if (row < 0 || row > (height - 1) || col < 0 || col > (width - 1))
    return false;
  if (m[row][col] == '-' || m[row][col] == '+' || m[row][col] == '|' || m[row][col] == 'U'
      || m[row][col] == '#')
    return false;

  return true;
}
    

//helper for find_path that actually finds the path using recursion
bool solve_maze(char** m, int& row, int& col, string& path, char end, const char* options, int height, int width) {
  
  //overwrite starting position
  if (m[row][col] == '>') {
    m[row][col] = '#';
  }

  //loop through movement options
  for (int i = 0; i < 4; i++) {
    translate_movement(options[i], row, col);
    //if move is valid and lands on exit, fill up path and return true
    if (valid_move(m, row, col, height, width)) {
      if (m[row][col] == end) {
	m[row][col] = '#';
	path += options[i];
	return true;
      }
      //if valid and new field, add to path
      m[row][col] = '#';
      path += options[i];
      //if move leads to valid solution, return
      if (solve_maze(m, row, col, path, end, options, height, width)) {
	return true;
      }
      //if not, go back and try the next movement
      else {
	//print_maze(m, height, width);
	m[row][col] = ' ';
	path.pop_back();
      }
    }
    reverse_movement(options[i], row, col);
  }
  //if no movement leads to a solution, return false
  return false;
}

//returns the path to the exit for a given maze
string find_path(char** m, int height, int width, char start, char end) {

  string path;
  char options[5] = {'N', 'S', 'W', 'E', '\0'};
  int row, col;
  
  //find starting position
  if (!find_marker(start, m, height, width, row, col)) {
    path = "no starting position";
    return path;
  }

  //use helper to solve and print string
  if (!solve_maze(m, row, col, path, end, options, height, width)) {
    path = "no solution";
    return path;
  }

  //return string
  return path;
}

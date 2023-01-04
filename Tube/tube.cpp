include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

#include "tube.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) {
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

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) {

  bool success = get_map_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];
  char space[] = " ";

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }
  
  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) {
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in Question 3 */
const char *error_description(int code) {
  switch(code) {
  case ERROR_START_STATION_INVALID: 
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
  for (int n=0; n<8; n++) {
    if (!strcmp(token, strings[n])) 
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}

//function returning coordinates of target on the map or -1 for r and c if not on map
bool get_symbol_position(char** map, const int& height, const int& width,
			 const char target, int& r, int& c) {

  //go through map and compare char by char
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      //set r and col if found
      if (map[row][col] == target) {
	r = row;
	c = col;
	return true;
      }
    }
  }
  //if not, set r and c to -1 and return false
  r = c = -1;
  return false;
}

//helper to find the symbol for a station
void find_symbol(const char* target, char& ch, const char* filename) {

  //define stream and open instream
  ifstream in(filename);
  char symbol, temp;
  char name[MAX_LENGTH];

  //while !eof get symbol as char and name as char array
  while (!in.eof()) {
    in >> symbol;
    in.get(temp);
    int i(0);
    while (temp != '\n') {
      in.get(temp);
      if (temp == '\n')
	name[i] = '\0';
      else
	name[i++] = temp;
    }
    if (in.good() && !strcmp(target, name)) {
      ch = symbol;
      in.close();
      return;
    }
  }
  in.close();
}

//function returning the symbols for name to find on the map
char get_symbol_for_station_or_line(const char* name) {

  //set ch to null
  char ch = ' ';
  
  //first, look for station
  find_symbol(name, ch, "stations.txt");

  //if ch is null, look for line
  if (ch == ' ')
    find_symbol(name, ch, "lines.txt");

  //return symbol or space
  return ch;
}

//helper that returns false if direction is invalid
bool invalid_route(const char* route) {

  //check direction by direction
  char dir[3];
  int d = 0;
  Direction check;

  //while route still has directions
  while (*route) {
    
    //get direction
    while (*route != ',' && *route) {
      dir[d++] = *route++;
    }
    dir[d] = '\0';
    check = string_to_direction(dir);
    
    //check if direction is invalid
    if (check == INVALID_DIRECTION)
      return true;
    
    //repeat
    d = 0;
    if (*route == ',')
      route++;
  }
  return false;
}

//helper function that gets the next direction of the route
Direction get_direction(char* dir, int& d, char* route, int& r) {

  //grab next direction
  while (route[r] != ',' && route[r] != '\0') {
    dir[d++] = route[r++];
    if (!isalpha(route[r]) && route[r] != ',')
      route[r] = '\0';
  }
  dir[d] = '\0';

  //safe direction
  Direction temp = string_to_direction(dir);

  //update indices d and r
  d = 0;
  if (route[r] == ',')
    r++;

  //return direction
  return temp;
}

//helper that updates coordinates for map based on current direction
void change_row_col(int& row, int& col, Direction direction) {

  switch (direction) {
  case N: row--; break;
  case S: row++; break;
  case W: col--; break;
  case E: col++; break;
  case NE: row--; col++; break;
  case NW: row--; col--; break;
  case SE: row++; col++; break;
  case SW: row++; col--; break;
  default: return;
  }
}

//helper to assess if move is out of bounds
bool out_of_bound(const int& height, const int& width, const int& row, const int& col) {

  //check if any coordinate is out of bound
  if (row < 0 || row > (height - 1) || col < 0 || col > (width - 1))
    return true;

  return false;
}

//helper to assess if route is off track
bool off_track(char** map, const int& row, const int& col) {

  if (map[row][col] == ' ')
    return true;
  
  return false;
}

//helper that returns sets coordinates for previous pos
void set_prev_pos(int& prev_row, int& prev_col, const int& row, const int& col,
		  Direction direction) {

  switch (direction) {
  case N: prev_row = row + 1; prev_col = col; break;
  case S: prev_row = row - 1; prev_col = col; break;
  case W: prev_row = row; prev_col = col + 1; break;
  case E: prev_row = row; prev_col = col - 1 ;break;
  case NE: prev_row = row + 1; prev_col = col - 1; break;
  case NW: prev_row = row + 1; prev_col = col + 1; break;
  case SE: prev_row = row - 1; prev_col = col - 1; break;
  case SW: prev_row = row - 1; prev_col = col + 1; break;
  default: return;
  }
}

//helper to assess if a line change is made with a move and if it is valid
bool invalid_line_change(char** map, const int& row, const int& col, Direction direction,
			 int& line_change, char& prev_line) {

  //get previous position
  int prev_row(-3), prev_col(-3);
  set_prev_pos(prev_row, prev_col, row, col, direction);
  
  //check if line change is attempted
  if (map[prev_row][prev_col] != map[row][col] && !isalnum(map[row][col])) {
    //check if it is valid (if previous position was not a station)
    if (!isalnum(map[prev_row][prev_col]))
	return true;
    //else it was a station and is correct
    else if (prev_line != '~' && map[row][col] != prev_line) {
      line_change++;
      prev_line = map[row][col];
    }
  }
  
  return false;
}

//helper to retrieve the opposite direction of a direction
Direction opposite_direction(Direction dir) {

  switch (dir) {
  case N: return S;
  case S: return N;
  case W: return E;
  case E: return W;
  case NE: return SW;
  case NW: return SE;
  case SE: return NW;
  case SW: return NE;
  default: return INVALID_DIRECTION;
  }
}

//helper that checks whether backtracking between stations is attempted
bool backtracking_betw_stations(char** map, const int& row, const int& col,
				Direction direction, Direction prev_direction) {

  //get old positions
  int prev_row(-3), prev_col(-3);
  set_prev_pos(prev_row, prev_col, row, col, direction);
  
  //check if move goes back without having been at a station
  if (prev_direction != INVALID_DIRECTION && direction == opposite_direction(prev_direction)
      && !isalnum(map[prev_row][prev_col]))
    return true;

  return false;
}

//helper to get the name of a symbol out of stations.txt
void get_name(char* destination, const char& end_symbol) {

  //set parameters
  char name[MAX_LENGTH];
  char symbol, buffer;
  ifstream in("stations.txt");

  //while !eof get symbol as char and station as char array
  while (!in.eof()) {
    in >> symbol;
    in.get(buffer);
    int i(0);
    while (buffer != '\n') {
      in.get(buffer);
      if (buffer == '\n')
        name[i] = '\0';
      else
        name[i++] = buffer;
    }
    if (in.good() && symbol == end_symbol) {
      strcpy(destination, name);
      in.close();
      return;
    }
  }
  in.close();
  return;
}

//returns number of line changes required to reach destination or error code if route is invalid
int validate_route(char** map, const int& height, const int& width, const char* start,
		   char* route, char* destination) {
  
  //validate start
  int row, col;
  char start_symbol = get_symbol_for_station_or_line(start); 
  if (!get_symbol_position(map, height, width, start_symbol, row, col))
    return ERROR_START_STATION_INVALID;

  //validate route string
  if (invalid_route(route))
    return ERROR_INVALID_DIRECTION;

  //start tracking journey
  int line_change(0);
  char dir[3], prev_line = '~';
  Direction direction = INVALID_DIRECTION, prev_direction = INVALID_DIRECTION;
  int d(0), r(0);
  //while route has directions
  while (route[r] != '\0') {
    
    //get direction and adapt coordinates
    direction = get_direction(dir, d, route, r);
    change_row_col(row, col, direction);
    
    //if out of map bounds or off track, return error
    if (out_of_bound(height, width, row, col))
      return ERROR_OUT_OF_BOUNDS;
    
    if (off_track(map, row, col))
      return ERROR_OFF_TRACK;
    
    //if line change attempt is made outside of station, return error, if valid, count is incr.
    if (invalid_line_change(map, row, col, direction, line_change, prev_line))
      return ERROR_LINE_HOPPING_BETWEEN_STATIONS;

    if (!isalnum(map[row][col]))
	prev_line = map[row][col];
    
    //check backtracking between stations
    if (backtracking_betw_stations(map, row, col, direction, prev_direction))
      return ERROR_BACKTRACKING_BETWEEN_STATIONS;
    prev_direction = direction;

  }
  
  //check if endpoint is valid
  char end_symbol = map[row][col];
  if (!isalnum(end_symbol))
    return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;
  
  get_name(destination, end_symbol);
  return line_change;
}

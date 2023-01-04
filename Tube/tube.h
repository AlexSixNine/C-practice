#ifndef TUBE_H
#define TUBE_H

const int MAX_LENGTH = 60;

enum Direction {N, S, W, E, NE, NW, SE, SW, INVALID_DIRECTION};

/* error codes for Question 3 */
#define ERROR_START_STATION_INVALID -1 
#define ERROR_ROUTE_ENDPOINT_IS_NOT_STATION -2
#define ERROR_LINE_HOPPING_BETWEEN_STATIONS -3
#define ERROR_BACKTRACKING_BETWEEN_STATIONS -4
#define ERROR_INVALID_DIRECTION -5
#define ERROR_OFF_TRACK -6
#define ERROR_OUT_OF_BOUNDS -7

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width);

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width);

/* pre-supplied helper function to describe errors for Question 3 */
const char *error_description(int code);

/* presupplied helper function for converting string to Direction enum */
Direction string_to_direction(const char *token);

//function returning coordinates of target on the map or -1 for r and c if not on map
bool get_symbol_position(char** map, const int& height, const int& width,
			 const char target, int& r, int& c);

//function returning the symbols for name to find on the map
char get_symbol_for_station_or_line(const char* name);

//returns number of line changes required to reach destination or error code if route is invalid
int validate_route(char** map, const int& height, const int& width, const char* start,
		   char* route, char* destination);


#endif

#include <iostream>
#include <fstream>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include "river.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new (nothrow) char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new (nothrow) char[columns];
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

/* pre-supplied function which creates an empty ASCII-art scene */
char **create_scene() {
  char **scene = allocate_2D_array(SCENE_HEIGHT, SCENE_WIDTH);

  for (int i=0; i<SCENE_HEIGHT; i++) 
    for (int j=0; j<SCENE_WIDTH; j++)
      scene[i][j] = ' ';

  return scene;
}

/* pre-supplied function which frees up memory allocated for an ASCII-art scene */
void destroy_scene(char **scene) {
  deallocate_2D_array(scene, SCENE_HEIGHT);
}

/* pre-supplied function which displays an ASCII-art scene */
void print_scene(char **scene) {
  for (int i=0; i<SCENE_HEIGHT; i++) {
    for (int j=0; j<SCENE_WIDTH; j++)
      cout << scene[i][j];
    cout << endl;
  }
}

/* helper function which removes carriage returns and newlines from strings */
void filter(char *line) {
  while (*line) {
    if (*line >= ' ')
      line++;
    else 
      *line = '\0';
  }
}

/* pre-supplied function which inserts an ASCII-art drawing stored in a file
   into a given ASCII-art scene starting at coordinates (row,col)  */
bool add_to_scene(char **scene, int row, int col, const char *filename) {

  ifstream in(filename);
  if (!in)
    return false;

  int start_col = col;
  char line[512];
  in.getline(line,512);
  filter(line);
  while (!in.fail()) {
    for (int n=0; n<strlen(line); n++) {
      if (row >= SCENE_HEIGHT)
	return false;
      if (col >= SCENE_WIDTH)
	break;
      scene[row][col++] = line[n];
    }
    row++;
    col = start_col;
    in.getline(line,512);
    filter(line);
  }
  return true;
}

/* pre-supplied helper function to report the status codes encountered in Question 3 */
const char *status_description(int code) {
  switch(code) {
  case ERROR_INVALID_CANNIBAL_COUNT:
    return "Invalid cannibal count";
  case ERROR_INVALID_MISSIONARY_COUNT:
    return "Invalid missionary count";
  case ERROR_INVALID_MOVE:
    return "Invalid move";
  case ERROR_MISSIONARIES_EATEN:
    return "The missionaries have been eaten! Oh dear!";
  case ERROR_INPUT_STREAM:
    return "Unexpected input stream error";
  case ERROR_BONUS_NO_SOLUTION:
    return "No solution";
  case VALID_GOAL_STATE:
    return "Problem solved!";
  case VALID_NONGOAL_STATE:
    return "Valid state, everything is OK!";
  }
  return "Unknown error";
}


/* insert your functions here */

//helper that checks if left and boat have valid sizes and valid letters
bool valid_input(const char* left, const char* boat) {

  //check size
  int size_left = strlen(left), size_boat = strlen(boat); 
  if (size_left > 7 || size_boat > 2)
    return false;

  //check if input letters are valid for left and boat
  for (int i = 0; i < size_left; i++) {
    if (left[i] != 'M' && left[i] != 'C' && left[i] != 'B')
      return false;
  for (int i = 0; i < size_boat; i++)
    if (boat[i] != 'M' && boat[i] != 'C')
      return false;
  }
  
  return true;
}

//helper that returns if boat needs to be printed on left
bool is_boat_left(const char* left) {

  //go through, check if boat is on left
  while (*left) {
    if (*left++ == 'B')
      return true;
  }

  return false;
}

//helper that returns sequence that needs to be printed on left bank
char* sq_left_bank(const char* left, const char* boat, const bool boat_left) {

  char* left_bank = new char[8];
  int num_m(0), num_c(0);
  
  //count how many people need to be printed on left
  while (*left) {
    if (*left == 'M')
      num_m++;
    if (*left == 'C')
      num_c++;
    left++;
  }
  
  //subtract people in boat, if boat is left
  if (boat_left) {
    while (*boat) {
      if (*boat == 'M')
	num_m--;
      if (*boat == 'C')
	num_c--;
      boat++;
    }
  }
  //put into array and return
  int i = 0;
  for (int m = 0; m < num_m; m++)
    left_bank[i++] = 'M';
  for (int c = 0; c < num_c; c++)
    left_bank[i++] = 'C';
  left_bank[i] = '\0';

  return left_bank;
}

//helper that returns sequence that needs to be printed on right bank
char* sq_right_bank(const char* left, const char* boat, const bool boat_left) {

  char* right_bank = new char[8];
  int num_m(3), num_c(3); 
  
  //count how many people need to be printed on right
  while (*left) {
    if (*left == 'M')
      num_m--;
    if (*left == 'C')
      num_c--;
    left++;
  }
  
  //subtract people in boat, if boat is right and departing
  if (!boat_left) {
    while (*boat) {
      if (*boat == 'M')
	num_m--;
      if (*boat == 'C')
	num_c--;
      boat++;
    }
  }
  //put into array and return
  int i = 0;
  for (int m = 0; m < num_m; m++)
    right_bank[i++] = 'M';
  for (int c = 0; c < num_c; c++)
    right_bank[i++] = 'C';
  right_bank[i] = '\0';

  return right_bank;
}

//helper that returns boat passenger sequence
char* sq_boat_(const char* boat) {

  char* boat_ = new char[3];

  int i = 0;
  while (*boat) {
    if (*boat == 'M')
      boat_[i++] = 'M';
    if (*boat == 'C')
      boat_[i++] = 'C';
    boat++;
  }
  boat_[i] = '\0';

  return boat_;
}


//builds river scene depending on left and boat strings
char **make_river_scene(const char* left, const char* boat) {

  //check for valid input
  assert(valid_input(left, boat));
  
  char **scene = create_scene();

  //insert river banks, river, and sun
  add_to_scene(scene, 0, 0, "bank.txt");
  add_to_scene(scene, 0, 53, "bank.txt");
  add_to_scene(scene, 19, 19, "river.txt");
  add_to_scene(scene, 3, 30, "sun.txt");

  //fetch details for picture
  bool boat_left = is_boat_left(left);
  char* left_bank = sq_left_bank(left, boat, boat_left);
  char* right_bank = sq_right_bank(left, boat, boat_left);
  char* boat_ = sq_boat_(boat);
  
  //insert left
  char* temp = left_bank;
  int m_col = 1, c_col = 1;
  while (*left_bank) {
    if (*left_bank == 'M' ) {
      add_to_scene(scene, 2, m_col, "missionary.txt");
      m_col += 6;
    }
    if (*left_bank == 'C' ) {
      add_to_scene(scene, 11, c_col, "cannibal.txt");
      c_col += 6;
    }
    left_bank++;
  }
  if (*temp)
    delete[] temp; 
  
  //insert boat
  int b_col(0);
  if (boat_left) {
    add_to_scene(scene, 17, 19, "boat.txt");
    b_col = 22;
  }
  else if (!boat_left) {
    add_to_scene(scene, 17, 36, "boat.txt");
    b_col = 39;
  }
  temp = boat_;
  while (*boat_) {
    if (*boat_ == 'M') {
      add_to_scene(scene, 11, b_col, "missionary.txt");
      b_col += 6;
    }
    if (*boat_ == 'C') {
      add_to_scene(scene, 11, b_col, "cannibal.txt");
      b_col += 6;
    }
    boat_++;
  }
  if (*temp)
    delete[] temp;

  //insert right
  temp = right_bank;
  m_col = 54;
  c_col = 54;
  while (*right_bank) {
    if (*right_bank == 'M') { 
      add_to_scene(scene, 2, m_col, "missionary.txt");
      m_col += 6;
    }
    if (*right_bank == 'C') {
      add_to_scene(scene, 11, c_col, "cannibal.txt");
      c_col += 6;
    }
    right_bank++;
  }
  if (*temp)
    delete[] temp;
  
  return scene;
}

//helper function that returns number of character for a side and a character
int get_number_left(const char* left, const char* targets, char ch, bool boat_departing) {

  int number(0);
  char* sequence = sq_left_bank(left, targets, boat_departing);
  char* temp = sequence;

  while (*sequence) {
    if (*sequence == ch)
      number++;
    sequence++;
  }

  if (*temp)
    delete[] temp;
  return number;
}

//helper function that returns number of character for a side and a character
int get_number_right(const char* left, const char* targets, char ch, bool boat_left) {

  int number(0);
  char* sequence = sq_right_bank(left, targets, boat_left);
  char* temp = sequence;

  while (*sequence) {
    if (*sequence == ch)
      number++;
    sequence++;
  }
  if (*temp)
    delete[] temp;
  return number;
}

//helper that checks if a move is valid
bool invalid_move(const char* left, const char* targets, bool boat_left) {

  int ms_left(0), cs_left(0), ms_boat(0), cs_boat(0), ms_right(3), cs_right(3);
  //if boat is left, check if targets are on left side
  if(boat_left) {
    while (*left) {
      if (*left == 'M')
	ms_left++;
      if (*left == 'C')
	cs_left++;
      left++;
    }
    while (*targets) {
      if (*targets == 'M')
	ms_boat++;
      if (*targets == 'C')
	cs_boat++;
      targets++;
    }
    if (ms_boat > ms_left || cs_boat > cs_left)
      return true;
  }
  //if boat is right, check if targets are on right side
  else if(!boat_left) {
    while (*left) {
      if (*left == 'M')
	ms_right--;
      if (*left == 'C')
	cs_right--;
      left++;
    }
    while (*targets) {
      if(*targets == 'M')
	ms_boat++;
      if(*targets == 'C')
	cs_boat++;
      targets++;
    }
    if (ms_boat > ms_right || cs_boat > cs_right)
      return true;
  }
  return false;

}

//returns state of the game depending on crossing
int perform_crossing(char* left, char* targets) {

  int outcome(0);
  
  //check input stream
  if (!valid_input(left, targets)) {
    outcome = -5;
    return outcome;
  }

  //get numbers on location of people of move
  bool boat_left = is_boat_left(left);
  int ms_left = get_number_left(left, targets, 'M', boat_left);
  int ms_right = get_number_right(left, targets, 'M', boat_left);
  int ms_boat =  3 - ms_left - ms_right;
  int ms_total = ms_left + ms_right + ms_boat;
  int cs_left = get_number_left(left, targets, 'C', boat_left);
  int cs_right = get_number_right(left, targets, 'C', boat_left);
  int cs_boat = 3 - cs_left - cs_right;
  int cs_total = cs_left + cs_right + cs_boat;

  //check for count errors (ms and cs)
  if (ms_total != 3) {
    outcome = -2;
    return outcome;
  }
  if (cs_total != 3) {
    outcome = -1;
    return outcome;
  }

  //check invalid move (incl. empty boat)
  if (invalid_move(left, targets, boat_left) || !*targets) {
    outcome = -3;
    return outcome;
  }

  //check missionaries eaten
  if ((cs_left > ms_left && ms_left != 0) || (cs_right > ms_right && ms_right != 0)) {
    outcome = -4;
    return outcome;
  }
      
  //load the boat
  cout << "Loading the boat...\n";
  char** scene = make_river_scene(left, targets);
  print_scene(scene);
  destroy_scene(scene);

  //update left
  char temp_left[8];
  int count = 0;
  for (int i = 0; i < ms_left; i++)
    temp_left[count++] = 'M';
  for (int i = 0; i < cs_left; i++)
    temp_left[count++] = 'C';
  if (!boat_left)
    temp_left[count++] = 'B';
  temp_left[count] = '\0';
  strcpy(left, temp_left);
  if (!boat_left)
    strcat(left, targets);

  //cross the river
  cout << "Crossing the river...\n";
  scene = make_river_scene(left, targets);
  print_scene(scene);
  destroy_scene(scene);

  //update targets
  char temp_targets[3] = {'\0', '\0', '\0'};
  
  //unload the boat
  cout << "Unloading the boat...\n";
  scene = make_river_scene(left, temp_targets);
  print_scene(scene);
  destroy_scene(scene);

  //get numbers on result and assess outcome
  boat_left = is_boat_left(left);
  ms_left = get_number_left(left, temp_targets, 'M', boat_left);
  ms_right = get_number_right(left, temp_targets, 'M', boat_left);
  cs_left = get_number_left(left, temp_targets, 'C', boat_left);
  cs_right = get_number_right(left, temp_targets, 'C', boat_left);

  //check missionaries eaten
  if ((cs_left > ms_left && ms_left != 0) || (cs_right > ms_right && ms_right != 0)) {
    outcome = -4;
    return outcome;
  }

  //check goal state
  if (ms_right == 3 && cs_right == 3) {
    outcome = 2;
    return outcome;
  }

  //if no other, nongoal state
  outcome = 1;
  return outcome;
}

//returns outcome of the game depending how a player played
int play_game() {

  //print starting situation
  cout << "Starting the game...\n";
  char left[8] = {"BCCCMMM"};
  char** scene = make_river_scene(left, "");
  print_scene(scene);
  destroy_scene(scene);

  //do while game is not over
  int result = 0;
  do {
    //ask for move
    char boat[3];
    cout << "Input your move: ";
    cin >> boat;
    
    //perform move
    result = perform_crossing(left, boat);

    //set move outcome and exit loop if applicable
  } while (result == 1);

  //return outcome
  return result;
}

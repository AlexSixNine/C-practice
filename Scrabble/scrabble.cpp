#include <iostream>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>
#include "scrabble.h"

using namespace std;

/* insert your function definitions here */

//returns the tile score of the given tile or -1 if not valid
int tile_score(char tile) {

  //if blank
  if (tile == ' ' || tile == '?') {
    return 0;
  }

  //if not a char
  if (!isalpha(tile)) {
    return -1;
  }

  //if char
  tile = toupper(tile);
  if (tile == 'Q' || tile == 'Z') {
    return 10;
  }
  if (tile == 'J' || tile == 'X') {
    return 8;
  }
  if (tile == 'K') {
    return 5;
  }
  if (tile == 'F' || tile == 'H' || tile == 'V' || tile == 'W' || tile == 'Y') {
    return 4;
  }
  if (tile == 'B' || tile == 'C' || tile == 'M' || tile == 'P') {
    return 3;
  }
  if (tile == 'D' || tile == 'G') {
    return 2;
  }
  return 1;
}

//helper that checks if a word is still avaible in given tiles
bool available(const char ch, char* copy, int& idx) {

  int blank = 0;
  //loop thorugh copy and see if char is available
  for (int i = 0; i < 7; i++) {
    //if yes, adapt idx and return true
    if (ch == copy[i]) {
      idx = i;
      return true;
    }
  }

  //check for ? or blank
  for (int i = 0; i < 7; i++) {
    if (copy[i] == ' ' || copy[i] == '?') {
      idx = i;
      return true;
    }
  }
  
  return false;
}

//checks if given word can be formed from given tiles and returns true and played tiles if possible
bool can_form_word_from_tiles(const char* word, const char* tiles, char* played_tiles, int i) {

  //base case: if word is over, return
  if (!*word) {
    played_tiles[i] = '\0';
    return true;
  }

  //define copy to compare to tiles
  char copy[8];
  strcpy(copy, tiles);
  int idx = 0;
  
  //go through copy of tiles and check if char is in there; if not false
  if (!available(*word, copy, idx)) {
      return false;
  }

  //if yes, put char in played tiles and "remove" tile from copy
  played_tiles[i] = copy[idx];
  copy[idx] = '1';
  word++;

  //go again
  return can_form_word_from_tiles(word, copy, played_tiles, i + 1);
}

//returns score of a set of played tiles given a score modifier
int compute_score(const char* played_tiles, const ScoreModifier* score_modifiers) {

  int s = 0;
  bool dws = false;
  bool tws = false;

  int i = 0;
  //go tile by tile
  while (played_tiles[i]) {
    if (score_modifiers[i] == NONE) {
      s += tile_score(played_tiles[i]);
    }
    else if (score_modifiers[i] == DOUBLE_LETTER_SCORE) {
      s += (2 * tile_score(played_tiles[i]));
    }
    else if (score_modifiers[i] == TRIPLE_LETTER_SCORE) {
      s += (3 * tile_score(played_tiles[i]));
    }
    else if (score_modifiers[i] == DOUBLE_WORD_SCORE) {
      s += tile_score(played_tiles[i]);
      dws = true;
    }
    else if (score_modifiers[i] == TRIPLE_WORD_SCORE) {
      s += tile_score(played_tiles[i]);
      tws = true;
    }
    i++;
  }

  //double and tripple word score
  if (dws) {
    s *= 2;
  }
  if (tws) {
    s *= 3;
  }

  //if seven tiles have been used, add 50
  if (i >= 6) {
    s += 50;
  }

  return s;
}

//returns score of highest scoring word that can be formed with given tiles and word; or -1
int highest_scoring_word_from_tiles(const char* tiles, const ScoreModifier* score_modifiers,
				    char* word) {

  //declare params and open dictionary
  int score = -1, final_score = -1;
  char temp[512] = {'\0'}, played_tiles[80] = {'\0'};
  ifstream in("words.txt");

  //loop thorugh dictionary, form word and calculate score
  while (!in.eof() && in.good()) {
    in >> temp;
    if (can_form_word_from_tiles(temp, tiles, played_tiles)) {
      score = compute_score(played_tiles, score_modifiers);
      
      //if score higher than previous score, take that word and score
      if (score > final_score) {
	final_score = score;
	strcpy(word, temp);
      }
    } 
  }

  //return highest score or -1
  in.close();
  return final_score;
}

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>

using namespace std;

#include "dictionary.h"
#include "doublets.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* Looks up a given word in the dictionary of approved words. 
   Returns true if the word is in the dictionary.
   Otherwise returns false. */

bool dictionary_search(const char *word) {
  static Dictionary dictionary("words.txt");
  return dictionary.search(word);
}

/* add your function definitions here */

//returns true, if changing current_word to next_word is valid according to doublet rules
bool valid_step(const char* current_word, const char* next_word) {
   
  //check if only one letter was changed, and if the letter occupies the same place as the replaced
  int count = 0;
  for (int i = 0; current_word[i]; i++) {
    if (current_word[i] != next_word[i]) {
      count++;
    }
  }
  if (count != 1) {
    return false;
  }

  //check if the new word is in the glossary
  if (!dictionary_search(next_word)) {
    return false;
  }

  return true;
}

//helper that returns length of a chain
int get_length(const char** chain) {

  int count = 0;
  while (chain[count]) {
    count++;
  }

  return count;
}


// prints given chain of words including doublets to outstream
bool display_chain(const char** chain, ostream& out) {

  //count words to prepare printing of first and last one
  int count = get_length(chain);

  //print all to outstream
  int i = 0;
  for (; i < count; i++) {
    //print first
    if (i == 0) {
      out << chain[i] << endl;
    }
    //print links
    else if (i > 0 && i < count - 1) {
      char temp[strlen(chain[i])];
      int n = 0;
      for (;n < strlen(chain[i]); n++) {
	temp[n] = tolower(chain[i][n]);
      }
      temp[n] = '\0';
      out << temp << endl;
    }
    //print last
    else if (i == count - 1) {
      out << chain[i] << endl;
    }
  }

  return (i == count ? true : false); 
}


//check if a given chain is a valid chain
bool valid_chain(const char** chain) {

  int count = get_length(chain);

  //check if chain is a chain
  if (count < 3) {
    return false;
  }

  //check if each step is a valid step by comparing current and next word
  for (int i = 0; i < count - 1; i++) {
    if (!valid_step(chain[i], chain[i + 1])) {
      return false;
    }
  }

  //check if a word appears twice in the chain
  for (int i = 0; chain[i]; i++) {
    int appearance = 0;
    //loop through chain again to count word appearance
    for (int n = 0; chain[n]; n++) {
      if (!strcmp(chain[i], chain[n])) {
	  appearance++;
      }
    }
    //if word appears more than once, return false
    if (appearance > 1) {
      return false;
    }
  }

  return true;
}

//returns true if it finds a solution together with the solution in answer_chain, false otherwise
bool find_chain(const char* start_word, const char* target_word, const char** answer_chain,
		const int max_steps, int row) {

  //expand answer chain
  answer_chain[row] = start_word;

  //base case; if start_word equals exit word, exit
  if (!strcmp(start_word, target_word)) {
    answer_chain[row + 1] = NULL;
    if (valid_chain(answer_chain)) {
      return true;
    }
  }

  //if max steps is reached, exit
  if (max_steps == 0) {
    return false;
  }

  //declare word on the heap and copy start_word in
  char* word = new char[MAX_LENGTH];
  strcpy(word, start_word);
  char prev_char = '!';

  //loop for every word
  for (int idx = 0; word[idx]; idx++) {
  //loop through every letter
    for (char ch = 'A'; ch <= 'Z'; ch++) {
      
      //if letters are equal in both words, continue
      if (word[idx] == target_word[idx]) {
	continue;
      }
      
      //otherwise, insert char into word
      prev_char = word[idx];
      word[idx] = ch;
      
      //if that is a valid step and if that leads to a solution return true
      if (valid_step(start_word, word)) {
	if (find_chain(word, target_word, answer_chain, max_steps - 1, row + 1)) {
	return true;
	}
      }

      //otherwise put back the character and try the next char
      word[idx] = prev_char;
    }
  }

  //delete word and return false
  delete [] word;
  return false;
}

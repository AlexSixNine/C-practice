#include <iostream>
#include "soundex.h"

using namespace std;

bool isValid(const char *string) {

  //cout << string << endl;
  
  while (*string) {
    if (isalpha(*string) == 0) {
      cerr << *string << " is not a valid input\n";
      return false;
    }
    string++;
  }
  return true;
}

char assign(const char s) {

  switch (s) {
  case 'b':
  case 'f':
  case 'p':
  case 'v':
    return '1';
    break;
  case 'c':
  case 'g':
  case 'j':
  case 'k':
  case 'q':
  case 's':
  case 'x':
  case 'z':
    return '2';
    break;
  case 'd':
  case 't':
    return '3';
    break;
  case 'l':
    return '4';
    break;
  case 'm':
  case 'n':
    return '5';
    break;
  case 'r':
    return '6';
    break;
  default:
    return ' ';
  }  
}

bool getChar(char c, char soundex[5], int idx) {

  char ch = assign(c);

  if (ch == ' ') {
    return false;
  }

  if (ch == soundex[idx - 1]) {
    return false;
  }

  soundex[idx]  = ch;
  return true;
}

bool encode(const char *string, char soundex[5]) {

  //check input
  if (!isValid(string)) {
    return false;
  }
  
  //input first letter into soundex code
  soundex[0] = toupper(*string);
  string++;

  //loop through the string and assign corresponding soundex values
  int idx = 1;
  for (const char* i = string; *i && idx < 4; i++) {
    if(getChar(*i, soundex, idx)) {
      idx++;
    }
  }

  //if surname too short, fill up with 0s
  for( ; idx < 4; idx++) {
    soundex[idx] = '0';
  }

  soundex[4] = '\0';

  return true;
}

bool compare(char *sound1, char* sound2) {

  if (*sound1 == '\0') {
    return true;
  }
  
  if (*sound1 == *sound2) {
    sound1++;
    sound2++;
    return compare(sound1, sound2);
  }

  return false;
}

void clearWord(char word[20]) {

  int i(0);
  while (word[i] != '\0') {
    word[i] = '\0';
    i++;
  } 
}

int count(char *sName, char *sentence) {

  int count(0);

  //encode given surname
  char soundex[5];
  encode(sName, soundex);

  char word[20];
  int idx = 0;
  char soundexWord[5];

  //loop through sentence, encode and compare each word in the sentence to surname soundex
  while (*sentence != '\0') {

    //if a word is completed, encode and compare
    if (*sentence == ' ' || *sentence == '.') {
      word[idx] = '\0';
      encode(word, soundexWord);
      count += compare(soundex, soundexWord);
      clearWord(word);
      idx = 0;
    }
    //otherwise fill the word array
    else if (isalpha(*sentence) != 0) {
      word[idx] = *sentence;
      idx++;
    }
    sentence++;
  } 

  return count;
}

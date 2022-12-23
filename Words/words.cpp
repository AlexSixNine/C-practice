#include <iostream>
#include "words.h"

using namespace std;


bool palindrome(const char* string) {

  int size = strlen(string); 
  char reversed[size + 1];

  reverse(string, reversed);

  return (compare(string, reversed)); 
}

//helper to assess if a string has no more alphabet characers
bool isComplete(const char* str) {

  while (*str) {
    if (isalpha(*str) != 0) {
      return false;
    }
    str++;
  }

  return true;
}

bool compare(const char *str1, const char *str2) {

  //if both strings have no more alphabet characters, they are the same
  if (isComplete(str1) && isComplete(str2)) {
    return true;
  }

  //jump over non-alphabet characters
  while (isalpha(*str1) == 0 || isalpha(*str2) == 0) {
    if (isalpha(*str1) == 0) {
      str1++;
    }
    if (isalpha(*str2) == 0) {
      str2++;
    }
  }

  //if alphabet characters are the same, go again
  if (toupper(*str1) == toupper(*str2)) {
    str1++;
    str2++;
    return compare(str1, str2);
  }

  //if different, return 0
  return false;
}


void reverse(const char *string, char *reversed) {

  //counter to insert chars into reversed
  int i = 0;
  //loop through string and input charactes from back into reversed
  for (int count = strlen(string); count > 0; count--) {
    reversed[i] = string[count - 1];
    i++;
  }
  reversed[i] = '\0';
}

//helper to sort a char array according to ASCI values
void sortString(char* string, char* copy) {

  int i = 0;
  //Convert the copy into upper case to enable sorting
  while (*string) {
    copy[i] = toupper(*string); 
    string++;
    i++;
  }
  copy[i] = '\0';

  //Sort copy array
  for (int n = 0; n < strlen(copy); n++) {
    for (int t = 1; t < strlen(copy); t++) {
      if (copy[n] < copy[t]) {
	char temp = copy[n];
	copy[n] = copy[t];
	copy[t] = temp;
      }
    }
  }
}

bool anagram(char* str1, char* str2) {

  //sort strings
  char copy1[strlen(str1) + 1];
  char copy2[strlen(str2) + 1];
  sortString(str1, copy1);
  sortString(str2, copy2);

  //compare strings
  return (compare(copy1, copy2));
}


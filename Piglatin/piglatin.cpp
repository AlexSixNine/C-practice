#include <iostream>
#include "piglatin.h"

using namespace std;

int findFirstVowel(const char* word) {

  int length = strlen(word);
  int position = 0;
  for (;position < length; position++) {
    if (isVowel(word[position], position, length)) {
      return position;
    }
  }
  return -1;
}

bool isVowel(char ch, int position, int length) {

  ch = tolower(ch);

  //a, e, i, o, u
  if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u') {
    return true;
  }

  //y
  if (ch == 'y' && (position != 0 && position != (length - 1))) {
    return true;
  }
  
  return false;
}

void translateWord(const char* english, char* piglatin) {

  //cout << english << endl;
  
  int vowel = findFirstVowel(english);
  bool capital_letter = isupper(*english);
  
  //if first character is a digit
  if (isdigit(*english)) {
    strcpy(piglatin, english);
    return;
  }

  //if first character is punct
  if (ispunct(*english)) {
    strcpy(piglatin, english);
    return;
  }
  
  //if word beginns with vowel, add way
  else if (vowel == 0) {
    strcpy(piglatin, english);
    strcat(piglatin, "way");
    return;
  }

  //if no vowels
  else if (vowel == -1) {
    strcpy(piglatin, english);
    strcat(piglatin, "ay");
    return;
  }
  
  //if first character not a vowel
  int n = 0;
  for (int i = vowel; english[i] != '\0'; i++) {
    piglatin[n] = english[i];
    n++;
  }
  for (int i = 0; i < vowel; i++) {
    piglatin[n] = tolower(english[i]);
    n++;
  }
  piglatin[n++] = 'a';
  piglatin[n++] = 'y';
  piglatin[n] = '\0';

  //if first character is uppercase
  if (capital_letter) {
    *piglatin = toupper(*piglatin);
  }
  return;
}

void translateStream(ifstream& in, ostream& out) {

  if (in.eof()) {
    return;
  }

  char ch = in.get();
  char word[64];
  char piglatin[70];
  
  int i = 0;
  if (isalnum(ch)) {
    while (isalnum(ch)) {
      word[i] = ch;
      i++;
      ch = in.get();
    }
    if (ispunct(ch)) {
      in.putback(ch);
    }
    word[i] = '\0';
    translateWord(word, piglatin);
    if (ispunct(ch)) {
      out << piglatin;
    }
    else {
      out << piglatin << ' ';
    }
  }
  else if (ispunct(ch)) {
    word[i] = ch;
    word[i + 1] = '\0';
    translateWord(word, piglatin);
    ch = in.get();
    if (isalnum(ch)) {
      out << piglatin;
    }
    else {
      out << piglatin << ' ';
    }
    in.putback(ch);
  }

  return translateStream(in, out);
}

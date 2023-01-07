#include <iostream>
#include <cctype>
#include <cstring>
#include <fstream>
#include "multitap.h"

using namespace std;

//returns number of keystrokes needed to encode char and puts encoding in multitap
int encode_character(const char ch, char* multitap) {

  //clear
  *multitap = '\0';

  //if number
  if (isdigit(ch)) {
    multitap[0] = '*';
    multitap[1] = ch;
    multitap[2] = '\0';
  }

  //cast ch to lower
  char temp = tolower(ch);
  int s = 0;
  
  //produce encoding and fill multitap
  switch (temp) {
  //letters
  case 'a': strcat(multitap, "2"); s += 1; break;
  case 'b': strcat(multitap, "22"); s += 2; break;
  case 'c': strcat(multitap, "222"); s += 3; break;
  case 'd': strcat(multitap, "3"); s += 1; break;
  case 'e': strcat(multitap, "33"); s += 2; break;
  case 'f': strcat(multitap, "333"); s += 3; break;
  case 'g': strcat(multitap, "4"); s += 1; break;
  case 'h': strcat(multitap, "44"); s += 2; break;
  case 'i': strcat(multitap, "444"); s += 3; break;
  case 'j': strcat(multitap, "5"); s += 1; break;
  case 'k': strcat(multitap, "55"); s += 2; break;
  case 'l': strcat(multitap, "555"); s += 3; break;
  case 'm': strcat(multitap, "6"); s += 1; break;
  case 'n': strcat(multitap, "66"); s += 2; break;
  case 'o': strcat(multitap, "666"); s += 3; break;
  case 'p': strcat(multitap, "7"); s += 1; break;
  case 'q': strcat(multitap, "77"); s += 2; break;
  case 'r': strcat(multitap, "777"); s += 3; break;
  case 's': strcat(multitap, "7777"); s += 4; break;
  case 't': strcat(multitap, "8"); s += 1; break;
  case 'u': strcat(multitap, "88"); s += 2; break;
  case 'v': strcat(multitap, "888"); s += 3; break;
  case 'w': strcat(multitap, "9"); s += 1; break;
  case 'x': strcat(multitap, "99"); s += 2; break;
  case 'y': strcat(multitap, "999"); s += 3; break;
  case 'z': strcat(multitap, "9999"); s += 4; break;
  //punctuation
  case '.': strcpy(multitap, "1"); s = 1; break;
  case ',': strcpy(multitap, "11"); s = 2; break;
  case '!': strcpy(multitap, "111"); s = 3; break;
  case '?': strcpy(multitap, "1111"); s = 4; break;
  case ' ': strcpy(multitap, "0"); s = 1; break;
  case '*': strcpy(multitap, "*"); s = 1; break;
  case '#': strcpy(multitap, "#"); s = 1; break;
  }; 

  //return int
  return strlen(multitap);
}

//encodes plaintext and puts code into multitap
void encode(const char* plaintext, char* multitap) {

  //declare needeed params
  char string[512] = {'\0'};
  char temp[20], prev_temp[20];
  int idx(0);

  //copy char by char into string
  while (plaintext[idx]) {
    
    //if first letter is uppercase, insert '#'
    if ((idx == 0  && isalpha(plaintext[idx]) && !islower(plaintext[idx])) ||
	(plaintext[idx - 1] == ' ' && isalpha(plaintext[idx]) && !islower(plaintext[idx]))) {
      strcat(string, "#");
    }
    
    //if case switch, insert #
    if ((isalpha(plaintext[idx]) && isalpha(plaintext[idx - 1]) &&
       (islower(plaintext[idx]) && !islower(plaintext[idx - 1]))) ||
       (!islower(plaintext[idx]) && islower(plaintext[idx - 1]))) {
      strcat(string, "#");
    }
    
    //encode next char
    encode_character(plaintext[idx++], temp);
    
    //if char with same number, insert break
    if (temp[0] == prev_temp[0]) {
      strcat(string, "|");
    }
    //insert char
    strcat(string, temp);
    strcpy(prev_temp, temp);
  }
  
  //cpy string into multitap
  strcpy(multitap, string);
}

//helper that helps decode elements for a multitap code
char decode_element(char* code) {

  int s = strlen(code);

  if (*code == '0') {
    return ' ';
  }
  if (*code == '1') {
    if (s == 1)
      return '.';
    if (s == 2)
      return ',';
    if (s == 3)
      return '!';
    if (s == 4)
      return '?';
  }
  
  char ch = (((*code - 50) * 3) + s) + ('A' - 1);
  if (*code == '8' || *code == '9') {
    ch++;
  }

  return ch;
}

//decodes multitap code provided by input and prints it to output
void decode(istream& in, ostream& out) {

  //declare needed params
  char ch, next_ch, decoded;
  bool upper = false, next = false;
  char temp[20] = {'\0'};

  //get char by char out of straem
  while (!in.eof()) {
    in.get(ch);

    //if skippable go directly to next char
    if (next) {
      next = false;
      continue;
    }

    //if #, change of case and go to next char
    if (ch == '#') {
      upper = (upper ? false : true);
      continue;
    }

    //if digit, print digit to ostream and set flag
    if (ch == '*') {
      decoded = in.peek();
      next = true;
      out << decoded;
    }

    //if letter, go into decoding process
    if (isdigit(ch)) {

      //input ch into temp that is to be decoded when element is complete
      char cpy[2] = {ch, '\0'};
      strcat(temp, cpy);

      //check if element is complete and print out if yes
      next_ch = in.peek();
      if (ch != next_ch) {
	decoded = decode_element(temp);
	if (!upper) {
	  decoded = tolower(decoded);
	}
	out << decoded;
	temp[0] = '\0';
      }
    }
  }   
}

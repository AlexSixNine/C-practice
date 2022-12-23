#include <iostream>
#include "braille.h"

using namespace std;

//returns string with braille code
void encode(const char *text, char *braille) {
 
  if (!*text) {
    return;
  }

  int idx = encode_character(*text, braille);
  text++;
  return encode(text, &braille[idx]);
}

//helper with switch statement for letters
void assign_braille(const char ch, char *braille) {

  switch (ch) {
    case 'a': strcat(braille, "0....."); break;
    case 'b': strcat(braille, "00...."); break;
    case 'c': strcat(braille, "0..0.."); break;
    case 'd': strcat(braille, "0..00."); break;
    case 'e': strcat(braille, "0...0."); break;
    case 'f': strcat(braille, "00.0.."); break;
    case 'g': strcat(braille, "00.00."); break;
    case 'h': strcat(braille, "00..0."); break;
    case 'i': strcat(braille, ".0.0.."); break;
    case 'j': strcat(braille, ".0.00."); break;
    case 'k': strcat(braille, "0.0..."); break;
    case 'l': strcat(braille, "000..."); break;
    case 'm': strcat(braille, "0.00.."); break;
    case 'n': strcat(braille, "0.000."); break;
    case 'o': strcat(braille, "0.0.0."); break;
    case 'p': strcat(braille, "0000.."); break;
    case 'q': strcat(braille, "00000."); break;
    case 'r': strcat(braille, "000.0."); break;
    case 's': strcat(braille, ".000.."); break;
    case 't': strcat(braille, ".0000."); break;
    case 'u': strcat(braille, "0.0..0"); break;
    case 'v': strcat(braille, "000..0"); break;
    case 'w': strcat(braille, ".0.000"); break;
    case 'x': strcat(braille, "0.00.0"); break;
    case 'y': strcat(braille, "0.0000"); break;
    case 'z': strcat(braille, "0.0.00"); break;
    case '.': strcat(braille, ".0..00"); break;
    case ',': strcat(braille, ".0.000"); break;
    case ';': strcat(braille, ".00..."); break;
    case '-': strcat(braille, "..0..0"); break;
    case '!': strcat(braille, ".00.0."); break;
    case '?': strcat(braille, ".00..0"); break;
    case '(': strcat(braille, ".00.00"); break;
    case ')': strcat(braille, ".00.00"); break;
    default: strcat(braille, "......");
  }
}

//helper to encode letters
void encode_letter(const char ch, char *braille) {

  //if capital letter, input capital sign
  if (isupper(ch)) {
    strcpy(braille, ".....0");
  }

  //input letter code
  assign_braille(tolower(ch), braille);
}

//helper to encode digits
void encode_digit(const char ch, char *braille) {

  strcpy(braille, "..0000");

  assign_braille(ch + 48, braille);
}

//helper to encode punctuation
void encode_punct(const char ch, char *braille) {
  assign_braille(ch, braille);
}

int encode_character(const char ch, char *braille) {

  //clear char array;
  *braille = '\0';
  
  //if letter, assign the letter code
  if (isalpha(ch)) {
    encode_letter(ch, braille);
  }

  //if number, assign the number braille sequence
  else if (isdigit(ch)) {
    encode_digit(ch, braille);
  }

  //if punctuation, assign punctuation braille suequence
  else if (ispunct(ch)) {
    encode_punct(ch, braille);
  }

  //if none of the above, create empty sequence
  else {
    strcpy(braille, "......");
  }

  //cout << braille << endl;

  //Get the size
  int size = strlen(braille);
  return size;
}


void print(const char* text, const char* braille, ostream& out_stream) {

  //print matrices
  int size = strlen(text);
  for (int i = 0; i < 3; i++) {
    int idx = i;
    for (int n = 0; text[n] != '\0' && n < size; n++) {
      cout << braille[idx] << braille[idx + 3] << ' ';
      //out_stream << braille[idx] << braille[idx + 3] << ' ';
      idx += 6;

      if (isupper(text[n])) {
	cout << braille[idx] << braille[idx + 3] << ' ';
	idx += 6;
      }
    }
    cout << endl;
  }

  //print character
  while (*text) {
    if (isupper(*text)) {
      cout << "   ";
    }

    cout << *text << "  ";
    text++;
  }
  cout << endl;
}


void print_braille(const char *text, ostream &out_stream) {

  const char *copy = text;
  
  //encode string
  int size = strlen(text);
  char braille[size];
  encode(text, braille);

  //print
  print(copy, braille, out_stream);
}

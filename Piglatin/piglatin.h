#ifndef PIGLATIN_H
#define PIGLATIN_H

#include <cctype>
#include <cstring>
#include <fstream>

//return position of first vowel in a word or -1 if none found
int findFirstVowel(const char* word);

//returns true if char at given position is a vowel
bool isVowel(char ch, int position, int length);

//returns translated char array
void translateWord(const char* english, char* piglatin);

//translates text from a file into piglatin and displays on screen
void translateStream(std::ifstream& in, std::ostream& out);

#endif

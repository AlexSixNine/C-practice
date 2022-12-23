#ifndef SOUNDEX_H
#define SOUNDEX_H

#include <cstring>
#include <cctype>
#include <string>

bool isValid(const char *string);
char assign(const char s);
bool getChar(char c, char soundex[5], int idx);
bool encode(const char *string, char soundex[5]);
bool compare(char *sound1, char *sound2);
void clearWord(char word[20]);
int count(char *sName, char *sentence);

#endif

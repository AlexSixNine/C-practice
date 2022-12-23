#ifndef WORDS_H
#define WORDS_H

#include <cstring>
#include <cctype>


const int MAX_CHAR = 50;

void reverse(const char *string, char *reversed);

bool compare(const char *str1, const char *str2);

bool palindrome(const char* string);

bool anagram(char* str1, char* str2);

#endif

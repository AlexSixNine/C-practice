#ifndef CORRECT_H
#define CORRECT_H

#include <cstring>

void ascii_to_binary(char letter, char *output);
char binary_to_ascii(char *binary);

void text_to_binary(const char* str, char* binary);
void binary_to_text(const char* binary, char* text);
void add_error_correction(const char* data, char* corrected);
int decode(const char* received, char* decoded);

#endif

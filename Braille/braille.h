#ifndef BRAILLE_H
#define BRAILLE_H

#include <cctype>
#include <cstring>
#include <fstream>

int encode_character(const char ch, char *braille);

void encode(const char *text, char *braille);

void print_braille(const char *text, std::ostream& out_stream);

#endif

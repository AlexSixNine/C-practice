#ifndef MULTITAP_H
#define MULTITAP_H

//returns number of keystrokes needed to encode char and puts encoding in multitap
int encode_character(const char ch, char* multitap);

//encodes plaintext and puts code into multitap
void encode(const char* plaintext, char* multitap);

//decodes multitap code provided by input and prints it to output
void decode(std::istream& in, std::ostream& out);


#endif

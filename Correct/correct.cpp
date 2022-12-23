#include <iostream>

using namespace std;

/* You are supplied with two helper functions */

/* converts a character into a binary string representation */
void ascii_to_binary(char ch, char *binary) {
  for (int n = 128; n; n >>= 1) 
    *binary++ = (ch & n) ? '1' : '0';
  *binary = '\0';
}

/* converts a binary string representation into a character */
char binary_to_ascii(char *binary) {
  int ch = 0;
  for (int n=0, slide=128; n<8; n++, slide >>= 1) {
    if (binary[n] == '1')
      ch = ch | slide;
  }
  return ch;
}

/* now add your own functions here */

void text_to_binary(const char* str, char* binary) {

  //exit when over
  if (!*str) {
    return;
  }

  //convert character and move to next one
  ascii_to_binary(*str++, binary);
  
  return text_to_binary(str, &binary[8]);
}

void binary_to_text(const char* binary, char* text) {

  //exit when over
  if (!*binary) {
    *text = '\0';
    return;
  }

  //convert character one by one
  char bits[9];
  for (int i = 0; i < 8; i++) {
    bits[i] = *binary++;
  }
  char ch = binary_to_ascii(bits);
  *text++ = ch;

  return binary_to_text(binary, text);
}

//returns parity values for add_error_correction
int parity(int a, int b, int c) {

  int sum = a + b + c;

  if (sum % 2 == 0) {
    return 0;
  }
  return 1;
}

//returns partiy values for decode
int parity(int a, int b, int c, int d) {

  int sum = a + b + c + d;

  if (sum % 2 == 0) {
    return 0;
  }
  return 1;
}

void add_error_correction(const char* data, char* corrected) {

  if (!*data) {
    return;
  }
  
  //get next 4 data bits
  char bits[5];
  for (int i = 0; i < 4; i++) {
    bits[i] = *data++;
  }
  bits[4] = '\0';

  //calculate check bits
  char c1 = (parity(bits[0], bits[1], bits[3]) == 1 ? '1' : '0');
  char c2 = (parity(bits[0], bits[2], bits[3]) == 1 ? '1' : '0');
  char c3 = (parity(bits[1], bits[2], bits[3]) == 1 ? '1' : '0');

  //create and add 7 bit code to corrected
  corrected[0] = c1;
  corrected[1] = c2;
  corrected[2] = bits[0];
  corrected[3] = c3;
  int n = 1;
  for (int i = 4; i < 7; i++) {
    corrected[i] = bits[n++];
  }
  corrected[7] = '\0';

  return add_error_correction(data, &corrected[7]);
}

int decimal_value(const char* position) {

  int p(0);

  if (position[0] == '1')
    p += 4;
  if (position[1] == '1')
    p += 2;
  if (position[2] == '1')
    p += 1;

  return p;
}

int decode(const char* received, char* decoded) {

  //cout << "received enters: " << received << endl;
  if (!*received) {
    return 0;
  }

  bool error = false;
  
  //consider first 7 bits
  char bits[8];
  for (int i = 0; i < 7; i++) {
    bits[i] = *received++;
    //cout << received << endl;
  }
  bits[8] = '\0';

  //calculate parity bits
  char p1 = (parity(bits[3], bits[4], bits[5], bits[6]) == 1 ? '1' : '0');
  char p2 = (parity(bits[1], bits[2], bits[5], bits[6]) == 1 ? '1' : '0');
  char p3 = (parity(bits[0], bits[2], bits[4], bits[6]) == 1 ? '1' : '0');

  //if error, get decimal value of parities and flip corresponding bit; recover data stream
  if (p1 == '1' || p2 == '1' || p3 == '1') {
    error = true;
    char position[4];
    position[0] = p1;
    position[1] = p2;
    position[2] = p3;
    position[3] = '\0';
    bits[decimal_value(position) - 1] = (bits[decimal_value(position) - 1] == '1' ? '0' : '1');
  }
  
  //Recover original data stream
  decoded[0] = bits[2];
  int n = 4;
  for (int i = 1; i < 4; i++)
    decoded[i] = bits[n++];
  decoded[4] = '\0';

  //cout << "received recurses: " << received << endl;
  int error_num = decode(received, &decoded[4]);
  return (error ? error_num + 1 : error_num);
}

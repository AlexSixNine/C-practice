#include <iostream>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>
#include "stamp.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

// helper function for internal use only
// transforms raw binary hash value into human-friendly hexademical form
void convert_hash(const unsigned char *str, char *output, int hash_length) {
  char append[16];
  strcpy (output, "");
  for (int n=0; n<hash_length; n++) {
    sprintf(append,"%02x",str[n]);
    strcat(output, append);
  }
}

// pre-supplied helper function
// generates the SHA1 hash of input string text into output parameter digest
// ********************** IMPORTANT **************************
// ---> remember to include -lcrypto in your linking step <---
// ---> so that the definition of the function SHA1 is    <---
// ---> included in your program                          <---
// ***********************************************************
void text_to_SHA1_digest(const char *text, char *digest) {
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1( (const unsigned char *) text, strlen(text), hash);
  convert_hash(hash, digest, SHA_DIGEST_LENGTH);
}

/* add your function definitions here */


/* returns number of leading 0s of a hexadecimal string and -1 if string is not valid
 * const char* is the string is checks */
int leading_zeros(const char* digest) {

  //check if string is valid, otherwise return -1
  for (int i = 0; digest[i]; i++) {
    if (!isxdigit(digest[i])) {
      return -1;
    }
  }

  //check how many leading 0s up to 5
  int count = 0;
  while (*digest) {
    if (*digest == '0') {
      count++;
      digest++;
    }
    if (*digest != '0' || count == 5) {
      break;
    }
  }
  return count;
}

/* returns the diegest of a content of a textfile in digest; or false and "error" in digest if file * could not be opened */
bool file_to_SHA1_digest(const char* filename, char* digest) {

  //open file and connect in_stream
  ifstream in(filename);

  //return error and false if in_stream could not be opened
  if (!in.good()) {
    strcpy(digest, "error");
    return false;
  }

  //otherwise read in message into string buffer
  char buffer[23000] = {'\0'};
  char string[512];
  while (!in.eof()) {
    in.getline(string, 512);
    strcat(buffer, string);
  }

  //turn buffer into SHA1
  text_to_SHA1_digest(buffer, digest);

  //close stream and return true
  in.close();
  return true;
} 

/* prepares header for a given recipient (mail address) and body (filename); returns false if mess * age body cannot be read or a valid counter is not found after 10.000 attempts */
bool make_header(const char* e_mail, const char* filename, char* header) {

  //generate SHA1 digest for body and check filename
  char digest_body[41];
  if (!file_to_SHA1_digest(filename, digest_body)) {
    return false;
  }

  //calculates counter and loop until valid solution is found
  char digest_header[41];
  const char* number = nullptr;
  int counter = 0;
  for (;counter <= 10000000; counter++) {
    
    //build header
    strcpy(header, e_mail);
    strcat(header, ":");
    strcat(header, digest_body);
    strcat(header, ":");

    //append counter
    number = to_string(counter).c_str();
    strcat(header, number);

    //check header digest
    text_to_SHA1_digest(header, digest_header);
    if (leading_zeros(digest_header) == 5) {
      return true;
    }
  }

  return false;
}

/* helper that checks if header is in correct format */
bool correct_format(const char* header) {

  //set counter
  int fields(0);
  int colons(0);
  
  //loop through header
  while (*header) {
    if (*header == ':') {
      colons++;
      fields++;
    }
    header++;
  }

  if (colons == 2 && fields == 2) {
    return true;
  }
  return false;
}

/*helper that verifies recipient in header */
bool correct_recipient(const char* email, const char* header) {

  int size = strlen(email);
  char copy[size + 1];

  for (int i = 0; i < size; i++) {
    copy[i] = header[i];
  }
  copy[size] = '\0';

  return (!strcmp(copy, email) ? true : false);
}

/* helper that returns the body digest */
void get_body_digest(const char* header, char* digest) {

  while (*header != ':') {
    header++;
  }
  header++;

  int idx = 0;
  while (*header != ':') {
    digest[idx++] = *header++;
  }
  digest[41] = '\0';
}

//helper to compare digests
bool compare(const char* digest, const char* digest_file) {

  while (*digest) {
    if (*digest == *digest_file) {
      digest_file++;
      digest++;
    }
    else
      return false;
  }
  return true;
}

/* returns enum value depending if message is valid; checks e-mail, header, and body digest */
MessageStatus check_header(const char* e_mail, const char* header, const char* filename) {

  //confirm header is in the correct format
  if (!correct_format(header)) {
    return INVALID_HEADER;
  }

  //check recipient email in header matches email
  if (!correct_recipient(e_mail, header)) {
    return WRONG_RECIPIENT;
  }
  
  //check SHA1 body
  char digest_file[41] = { '\0' };
  file_to_SHA1_digest(filename, digest_file);
  char digest[41] = { '\0' };
  get_body_digest(header, digest);
  if (!compare(digest, digest_file)) {
    return INVALID_MESSAGE_DIGEST;
  } 

  //check SHA1 of header
  char digest_header[41];
  text_to_SHA1_digest(header, digest_header);
  if (leading_zeros(digest_header) != 5) {
    return INVALID_HEADER_DIGEST;
  }

  return VALID_EMAIL;
}

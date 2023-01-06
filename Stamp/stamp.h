#ifndef STAMP_H
#define STAMP_H

enum MessageStatus {
  INVALID_HEADER=-1,
  WRONG_RECIPIENT=-2,
  INVALID_MESSAGE_DIGEST=-3,
  INVALID_HEADER_DIGEST=-4,
  VALID_EMAIL=1
};

/* pre-supplied helper function */
/* generates the SHA1 hash of input string text into output parameter digest */
void text_to_SHA1_digest(const char *text, char *digest);


/* returns number of leading 0s of a hexadecimal string and -1 if string is not valid
 * const char* is the string is checks */
int leading_zeros(const char* digest);

/* returns the diegest of a content of a textfile in digest; or false and "error" in digest if file * could not be opened */
bool file_to_SHA1_digest(const char* filename, char* digest);

/* prepares header for a given recipient (mail address) and body (filename); returns false if mess * age body cannot be read or a valid counter is not found after 10.000 attempts */
bool make_header(const char* e_mail, const char* filename, char* header);

/* returns enum value depending if message is valid; checks e-mail, header, and body digest */
MessageStatus check_header(const char* e_mail, const char* header, const char* filename);

#endif

#ifndef DOUBLETS_H
#define DOUBLETS_H


/*  This pre-supplied function looks up a given word in the dictionary
    of approved words. It is assumed that the input word is in uppercase.
    The function returns true if the word is in the dictionary.
    Otherwise returns false. */

bool dictionary_search(const char *word);

/* add your function prototypes here */

const int MAX_LENGTH = 50;


/* returns true, if changing current_word to next_word is valid according to doublet rules
 *  current word is the current word to be adapted; next_word the word it is changed into */
bool valid_step(const char* current_word, const char* next_word);

/* prints given chain of words including doublets to outstream
 * chain is the provided chain of words, ostream is the outsream */
bool display_chain(const char** chain, ostream& out);


/* check if a given chain is a valid chain
 * chain is the chain of words to be checked */
bool valid_chain(const char** chain);

/* returns true if it finds a solution together with the solution in answer_chain, false otherwise */
bool find_chain(const char* start_word, const char* target_word, const char** answer_chain,
		const int max_steps, int row = 0);

#endif

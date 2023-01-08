#ifndef SCRABBLE_H
#define SCRABBLE_H

enum ScoreModifier { NONE, DOUBLE_LETTER_SCORE, TRIPLE_LETTER_SCORE, DOUBLE_WORD_SCORE, TRIPLE_WORD_SCORE };

/* insert your function prototypes here */

//returns the tile score of the given tile or -1 if not valid
int tile_score(char tile);

//checks if given word can be formed from given tiles and returns true and played tiles if possible
bool can_form_word_from_tiles(const char* word, const char* tiles, char* played_tiles, int i = 0);

//returns score of a set of played tiles given a score modifier
int compute_score(const char* played_tiles, const ScoreModifier* score_modifiers);

//returns score of highest scoring word that can be formed with given tiles and word; or -1
int highest_scoring_word_from_tiles(const char* tiles, const ScoreModifier* score_modifiers,
				    char* word);


#endif

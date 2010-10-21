/*
 *  Created by Lionel Sambuc on 03.dec.2009.
 *  Copyright 2009. All rights reserved.
 *
 */

/**************************************************************************
 *                           Tokenizer interface                          *
 **************************************************************************/
#ifndef _TOKENIZER_H_
#define _TOKENIZER_H_

/**
 * This function pointer has to be initialized before any call to the 
 * tk_next_token or tk_previous_token. If not, the default separator
 * (tk_is_white_space) will be used.
 *
 * As this vairable is globally shared, ensure it always points to the 
 * separator detector you wish before calling the tk_*_token function, as well
 * as not modify it during their execution.
 */
extern int (*tk_is_separator)(char c);

/**
 * This function return 1 if c is one of the following characters, 
 * otherwise 0 :
 * ' '  : space
 * '\t' : horizontal tabulation
 * '\v' : vertical tabulation
 * '\r' : carriage return
 * '\n' : line feed
 */
int tk_is_white_space(char c);

/**
 * Moves from to the first character of the next token and updates length to 
 * it's total length. Return 1 on success or 0 if the token wasn't found.
 */
int tk_next_token(char const *str, int *from, size_t *length);

/**
 * Moves from to the last character of the previous token and updates length 
 * to it's total length. Return 1 on success or 0 if the token wasn't found.
 */
int tk_previous_token(char const *str, int *from, size_t *length);

#endif /* _TOKENIZER_H_ */
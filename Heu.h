/*
 *  Created by Lionel Sambuc on 03.dec.2009.
 *  Copyright 2009. All rights reserved.
 *
 */

/**************************************************************************
 *                           Heu lexer interface                          *
 **************************************************************************/
#ifndef _HEU_H_
#define _HEU_H_

/**
 * The Heu language is an isomorphism of the brainfuck language with the 
 * following associations : 
 *
 * Language operator définition :
 * ******************************
 *   >	Heu!
 *   <	Heu?
 *   +	Heu...
 *   -	Heu.
 *   .	Heu, j'avais pensé que.
 *   ,	Heu, t'en penses quoi ?
 *   [	Heu, heu!
 *   ]	Heu, heu?
 */

/**
 * Initialize the Heu lexer.
 */
void heu_lexer_init();

/**
 * Read the previous token found starting from the token_start position in 
 * program.
 * Parameters : 
 *  * program		the program to tokenize.
 *  * token_start	the position in the program from which to begin the search
 *					of the previous token, this value is update to point 
 *					on the last character of the returned token.
 *  * length		the total length of the program.
 */
int heu_read_previous_token(char const *program, int *token_start, size_t length);

/**
 * Read the next token found starting from the token_start position in 
 * program.
 * Parameters : 
 *  * program		the program to tokenize.
 *  * token_start	the position in the program from which to begin the search
 *					of the next token, this value is updated to point after 
 *					the last character of returned token.
 *  * length		the total length of the program.
 */
int heu_read_next_token(char const *program, int *token_start, size_t length);

#endif /* _HEU_H_ */
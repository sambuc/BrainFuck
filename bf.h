/*
 *  Created by Lionel Sambuc on 03.dec.2009.
 *  Copyright 2009. All rights reserved.
 *
 */

/**************************************************************************
 *                        Brainfuck lexer interface                       *
 **************************************************************************/
#ifndef _BF_H_
#define _BF_H_

/**
 * The following as been taken from : 
 * http://en.wikipedia.org/wiki/Brainfuck#Language_design
 *
 * Some more information about the language and it's features:
 * http://www.muppetlabs.com/~breadbox/bf/standards.html
 * 
 * For some example you may go to :
 * http://en.wikipedia.org/wiki/Brainfuck#Trivial
 * http://esoteric.sange.fi/brainfuck/bf-source/prog/
 * 
 * Language operator définition :
 * ******************************
 *   >	increment the data pointer (to point to the next cell to the right).
 *   <	decrement the data pointer (to point to the next cell to the left).
 *   +	increment (increase by one) the byte at the data pointer.
 *   -	decrement (decrease by one) the byte at the data pointer.
 *   .	output the value of the byte at the data pointer.
 *   ,	accept one byte of input, storing its value in the byte at the data 
 *			pointer.
 *   [	if the byte at the data pointer is zero, then instead of moving the 
 *			instruction pointer forward to the next command, jump it forward to 
 *			the command after the matching ] command*.
 *   ]	if the byte at the data pointer is nonzero, then instead of moving the 
 *			instruction pointer forward to the next command, jump it back to the 
 *			command after the matching [ command*.
 */

/**
 * Initialize the brainfuck lexer.
 */
void brainfuck_lexer_init();

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
int brainfuck_read_previous_token(char const *program, int *token_start, size_t length);

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
int brainfuck_read_next_token(char const *program, int *token_start, size_t length);

#endif /* _BF_H_ */
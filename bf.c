/*
 *  Created by Lionel Sambuc on 03.dec.2009.
 *  Copyright 2009. All rights reserved.
 *
 */

/**************************************************************************
 *                             Brainfuck lexer                            *
 **************************************************************************/
#include <string.h>

#include "interpreter.h"
#include "tokenizer.h"

#include "bf.h"

static char bf[NB_KWS];

/**************************************************************************
 *                            PRIVATE INTERFACE                           *
 **************************************************************************/
/**
 * Brainfuck separator detector.
 * Simply check whether the given character is one of the 8 reserved 
 * characters, if not it is a separator.
 */
static int brainfuck_is_separator(char c)
{
	return !(c == '<' || c == '>' ||
					 c == '+' || c == '-' || 
					 c == '.' || c == ',' || 
					 c == '[' || c == ']');
}

/**
 * Read a token from the program stream, in the given direction.
 * Parameters :
 *  * direction			positive : go from the left to the right
 *						negative : go from the right to the left
 *						zero	 : invalid direction.
 *  * program			the program from which to read a token
 *  * token_start		the actual position within the program
 *  * length			the length of the program
 */
static int read_token(int direction, char const *program, int *token_start, size_t length)
{
	char token;
	size_t token_length;
	
	/* Check that we have a sane direction. */
	if(direction == 0)
		return UKW;
	
	/* Read the next token. */
	if(direction > 0)
	{
		/* Try to find the next token. */
		if(!tk_next_token(program, token_start, &token_length))
			return EOT; /* It was not found. */
		
		/* Save the token to be analyzed. */
		token = *(program + *token_start);
	
		/* Update the pointer to be __after__ the token we are analyzing.
		 * A simple increment does the job as we have only one character tokens. 
		 */
		(*token_start)++;
	}
	
	/* Read the previous token. */
	if(direction < 0)
	{
		/* Update the pointer to be __on the last character__ of the token we
		 * are analyzing. A simple decrement does the job as we have only one
		 * character tokens. 
		 */
		(*token_start)--;

		/* Try to find the previous token. */
		if(!tk_previous_token(program, token_start, &token_length))
			return SOT;/* It was not found. */
		
		/* Save the token to be analyzed. */
		token = *(program + *token_start);
	}
	
	/* Match the token characters to a known token value. */
	if(bf[MVL] == token)
		return MVL;
	
	if(bf[MVR] == token)
		return MVR;
	
	if(bf[INC] == token)
		return INC;
	
	if(bf[DEC] == token)
		return DEC;
	
	if(bf[PUT] == token)
		return PUT;
	
	if(bf[GET] == token)
		return GET;
	
	if(bf[JZF] == token)
		return JZF;
	
	if(bf[JNB] == token)
		return JNB;
	
	return UKW;
}

/**************************************************************************
 *                            PUBLIC  INTERFACE                           *
 **************************************************************************/
void brainfuck_lexer_init()
{
	bf[MVR] = '>';
	bf[MVL] = '<';
	bf[INC] = '+';
	bf[DEC] = '-';
	bf[PUT] = '.';
	bf[GET] = ',';
	bf[JZF] = '[';
	bf[JNB] = ']';
	
	tk_is_separator = brainfuck_is_separator;
}

int brainfuck_read_next_token(char const *program, int *token_start, size_t length)
{
	return read_token(+1, program, token_start, length);
}

int brainfuck_read_previous_token(char const *program, int *token_start, size_t length)
{
	return read_token(-1, program, token_start, length);
}
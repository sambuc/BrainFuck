/*
 *  Created by Lionel Sambuc on 03.dec.2009.
 *  Copyright 2009-2010. All rights reserved.
 *
 */
#include <stdio.h>
#include <string.h>

#include "interpreter.h"
#include "tokenizer.h"

#include "Heu.h"

static char *heu[NB_KWS];

/**************************************************************************
 *                            PRIVATE INTERFACE                           *
 **************************************************************************/
/**
 * Heu separator detector.
 */
int heu_is_separator(char c)
{
	return !(c == 'H');
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
	size_t token_length, keyword_length;
	char const *token;
	
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
		token = program + *token_start;
		
		/* We are updating the token_start position later as we have variable
		 * length tokens.
		 */
	}
	
	/* Read the previous token. */
	if(direction < 0)
	{
		
		--*token_start;

		/* Try to find the previous token. */
		if(!tk_previous_token(program, token_start, &token_length))
			 return SOT;/* It was not found. */
			 
		/* Save the token to be analyzed. */
		token = program + *token_start;
	}
	
	
	/* Match the token characters to a known token value.
	 * also update token_start as well. */
	if(*token_start + (keyword_length = strlen(heu[MVL])) <= length &&
		 strncmp(heu[MVL], token, keyword_length) == 0)
	{
		*token_start += (direction > 0) ? keyword_length : -1;
		return MVL;
	}
		
	if(*token_start + (keyword_length = strlen(heu[MVR])) <= length &&
		 strncmp(heu[MVR], token, keyword_length) == 0)
	{
		*token_start += (direction > 0) ? keyword_length : -1;
		return MVR;
	}
	if(*token_start + (keyword_length = strlen(heu[INC])) <= length &&
		 strncmp(heu[INC], token, keyword_length) == 0)
	{
		*token_start += (direction > 0) ? keyword_length : -1;
		return INC;
	}
	if(*token_start + (keyword_length = strlen(heu[DEC])) <= length &&
		 strncmp(heu[DEC], token, keyword_length) == 0)
	{
		*token_start += (direction > 0) ? keyword_length : -1;
		return DEC;
	}
	
	if(*token_start + (keyword_length = strlen(heu[PUT])) <= length &&
		 strncmp(heu[PUT], token, keyword_length) == 0)
	{
		*token_start += (direction > 0) ? keyword_length : -1;
		return PUT;
	}
	
	if(*token_start + (keyword_length = strlen(heu[GET])) <= length &&
		 strncmp(heu[GET], token, keyword_length) == 0)
	{
		*token_start += (direction > 0) ? keyword_length : -1;
		return GET;
	}
	
	if(*token_start + (keyword_length = strlen(heu[JZF])) <= length &&
		 strncmp(heu[JZF], token, keyword_length) == 0)
	{
		*token_start += (direction > 0) ? keyword_length : -1;
		return JZF;
	}
	
	if(*token_start + (keyword_length = strlen(heu[JNB])) <= length &&
		 strncmp(heu[JNB], token, keyword_length) == 0)
	{
		*token_start += (direction > 0) ? keyword_length : -1;
		return JNB;
	}

	return UKW;
}

/**************************************************************************
 *                            PUBLIC  INTERFACE                           *
 **************************************************************************/
void heu_lexer_init()
{
	heu[MVR] = "Heu !";
	heu[MVL] = "Heu ?";
	heu[INC] = "Heu...";
	heu[DEC] = "Heu.";
	heu[PUT] = "Heu, j'avais pensé que.";
	heu[GET] = "Heu, t'en penses quoi ?";
	heu[JZF] = "Heu, heu!";
	heu[JNB] = "Heu, heu?";
	
	tk_is_separator = heu_is_separator;
}

int heu_read_next_token(char const *program, int *token_start, size_t length)
{
	return read_token(+1, program, token_start, length);
}

int heu_read_previous_token(char const *program, int *token_start, size_t length)
{
	return read_token(-1, program, token_start, length);
}
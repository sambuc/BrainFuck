/*
 *  Created by Lionel Sambuc on 03.dec.2009.
 *  Copyright 2009. All rights reserved.
 *
 */

/**************************************************************************
 *                           Tokenizer functions                          *
 **************************************************************************/
#include <stddef.h>
#include <string.h>

#include "tokenizer.h"

int (*tk_is_separator)(char c) = tk_is_white_space;
/**************************************************************************
 *                            PRIVATE INTERFACE                           *
 **************************************************************************/
/* Nothing ATM */

/**************************************************************************
 *                            PUBLIC  INTERFACE                           *
 **************************************************************************/
int tk_is_white_space(char c)
{
	return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v';
}

int tk_next_token(char const *str, int *from, size_t *length)
{
  const int taille = strlen(str);
  int i;
	
  /* Skip all the separators starting from *from */
  while ((*from < taille) && tk_is_separator(str[*from])) ++(*from);
	
  /* Go ahead until the first separator is found or the end of the string. */
  *length = 0;
  for (i = *from; ((i < taille) && !tk_is_separator(str[i])); ++(*length), ++i);
	
  return (*length != 0);
}

int tk_previous_token(char const *str, int *from, size_t *length)
{
  int i;
	
  /* Skip all the separators starting from *from */
  while ((*from > 0) && tk_is_separator(str[*from])) --(*from);
	
  /* Go back until the first separator is found or the start of the string. */
  *length = 0;
  for (i = *from; ((i > 0) && !tk_is_separator(str[i])); ++(*length), --i);
	
  return (*length != 0);
}
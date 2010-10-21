/*
 *  Created by Lionel Sambuc on 03.dec.2009.
 *  Copyright 2009-2010. All rights reserved.
 *
 */

/**************************************************************************
 *                  Brainfuck and derivatives interpreter                 *
 **************************************************************************/
#include <stdio.h>

#include "bf.h"
#include "Heu.h"

#include "interpreter.h"

/* Program Input */
static char *bfi_program;
static int bfi_token_start;
static size_t bfi_length;

/* Program tokenizer functions */
static int (*bfi_read_previous_token)(char const *program, int *token_start, size_t length);
static int (*bfi_read_next_token)(char const *program, int *token_start, size_t length);
static void (*bfi_lexer_init)(void);

/* Machine hardware */
static char bfi_memory[NB_MEMORY_WORDS];

/* Reading/Writing Head, which points to a memory word */
static char *bfi_head;				

/**************************************************************************
 *                            PRIVATE INTERFACE                           *
 **************************************************************************/
/**
 * Prints the error message and dump the interpreter memory before returning 
 * the given error code.
 */
static int bfi_execution_error(char* reason, int code)
{
	int i;
	printf( COLOR_RST "\n" COLOR_ERR "%s" COLOR_MSG "\n", reason);
	
	printf( COLOR_RST "At position : %s" COLOR_MSG "\n", (bfi_program + bfi_token_start));
	
	for(i = 0; i < MEMORY_DUMP_SIZE; i++)
		printf("%s%02x", i%4==0? (i%16==0?"\n": "::"): " ", bfi_memory[i]);
	printf(COLOR_RST "\n");
	
	return code;
}

/**
 * Jump to the associated closing bracket and returns the next instruction 
 * to be executed.
 * Hypotheses: bfi_token_start points just after the opening bracket.
 */
static int bfi_skip_forward(void)
{
	int context = 0;
	int next = -1;

	while((next = bfi_read_next_token(bfi_program, &bfi_token_start, bfi_length)) != EOT)
	{
		switch(next)
		{ /* Opening bracket */
			case JZF: context++; 
				break;
			/* Closing bracket */
			case JNB: context--;
				if(context == 0)
					return next;
				break;
			/* End of the program or unknown instruction. */
			case EOT:
			case SOT:
			case UKW:
				return next;
		}
	}
	
	return next;
}

/**
 * Jump to the associated opening bracket and returns the next instruction 
 * to be executed.
 * Hypotheses: bfi_token_start points just before the closing bracket.
 */
int bfi_skip_backward(void)
{
	int context = 0;
	int prev = -1;
	
	while((prev = bfi_read_previous_token(bfi_program, &bfi_token_start, bfi_length)) != SOT)
	{
		switch(prev)
		{ /* Opening bracket */
			case JZF: context--; 
				if(context == 0)
					return prev;
				break;
			/* Closing bracket */
			case JNB: context++; 
				break;
			/* End of the program or unknown instruction. */
			case EOT:
			case SOT:
			case UKW:
				return prev;
		}
	}
	
	return prev;
}

/**
 * This function initializes the lexer associated to the given brainfuck 
 * dialect. It also zero the whole memory band and reset the head to the 
 * first word.
 * it returns 0 on success or -1 if the dialect is unknown.
 */
static int bfi_initialize(enum bfi_dialects dialect)
{
	int i;
	
	/* Reset memory state. */
	for(i = 0; i < NB_MEMORY_WORDS; i++)
		bfi_memory[i] = 0;
	
	bfi_head = bfi_memory;
	
	/* Initialize the parser functions. */
	switch(dialect)
	{
		case BRAINFUCK:
			bfi_lexer_init = brainfuck_lexer_init;
			bfi_read_previous_token = brainfuck_read_previous_token;
			bfi_read_next_token = brainfuck_read_next_token;

			break;
		case HEU:
			bfi_lexer_init = heu_lexer_init;
			bfi_read_previous_token = heu_read_previous_token;
			bfi_read_next_token = heu_read_next_token;
			
			break;
		default:
			return -1;
			break;
	}

	/* Initialize the chosen lexer. */
	bfi_lexer_init();
	return 0;
}

/**************************************************************************
 *                            PUBLIC  INTERFACE                           *
 **************************************************************************/
int bfi_execute(char* program, size_t length, enum bfi_dialects dialect, char print_num)
{
	int next = EOT;
	bfi_program = program;
	bfi_length = length;
	
	if(bfi_initialize(dialect) != 0)
		return -UNKNOWN_DIALECT;

	while((next = bfi_read_next_token(bfi_program, &bfi_token_start, bfi_length)) != EOT && next != SOT) 
	{
		switch(next)
		{
			case MVL:
				--bfi_head;
				
				if(bfi_head < bfi_memory)
					return bfi_execution_error("HEAD Got past the start of memory!", -INVALID_MEMORY_ADDRESS);
				break;
			case MVR:
				++bfi_head;
				
				if(bfi_head >= bfi_memory + NB_MEMORY_WORDS)
					return bfi_execution_error("HEAD Got past the end of memory!", -INVALID_MEMORY_ADDRESS);
				break;
			case INC:
				++*bfi_head;
				break;
			case DEC:
				--*bfi_head;
				break;
			case PUT:
				if(print_num != 0)
					printf("%i", *bfi_head);
				else
					putchar(*bfi_head);
				break;
			case GET:
				*bfi_head = getchar();
				break;
			case JZF:
				if(*bfi_head == 0)
					next = bfi_skip_forward();
				break;
			case JNB:
				if(*bfi_head != 0)
					next = bfi_skip_backward();
				break;
			case UKW:
			default:
				return bfi_execution_error("Unknown keyword !", -INVALID_OPERATION);
		}
	}
	
	bfi_execution_error("End of Program", *bfi_head);
	/* Ensure there is an end of line */
	printf("\n");
	return (int)*bfi_head;
}


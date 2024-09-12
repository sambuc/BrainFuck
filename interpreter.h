/*
 *  Created by Lionel Sambuc on 03.dec.2009.
 *  Copyright 2009-2010. All rights reserved.
 *
 */

/**************************************************************************
 *             Brainfuck and derivatives interpreter interface            *
 **************************************************************************/
#ifndef	_INTERPRETOR_H_
#define _INTERPRETOR_H_
#include <stddef.h>

#include "colors.h"

/* The 8 Brainfuck operations */
#define NB_KWS				8

#define MVL						0	/* Move Left						*/
#define MVR						1	/* Move Right						*/
#define INC						2	/* Increment Value					*/
#define DEC						3	/* Decrement Value					*/
#define PUT						4	/* Putchar							*/
#define GET						5	/* Getchar							*/
#define JZF						6	/* Jump if Zero Forward				*/
#define JNB						7	/* Jump if Non zero Backward		*/

#define EOT						-1	/* End of Text						*/
#define SOT						-2	/* Start of Text					*/
#define UKW						-3	/* Unknown keyword					*/

/* Machine hardware */
#define NB_MEMORY_WORDS			30000
#define MEMORY_DUMP_SIZE		   64

/* Execution error codes (beware! must be outside the memory word value 
 * domain, i.e outside the range 0-255 at the moment) */
#define UNKNOWN_DIALECT			512
#define INVALID_MEMORY_ADDRESS	513
#define INVALID_OPERATION		514

enum bfi_dialects {BRAINFUCK, HEU};

/**
 * Main parser function.
 *
 * Reset the turing machine state and immediatly begin the execution of the 
 * given program.
 * On success interpretation the value of the currently selected memory word 
 * is returned (which is a value between 0 and 255).
 * Parameters:
 *  * program		the program to execute
 *  * length		the total length of the program
 *  * dialect		the specific brainfuck dialect of the program
 *  * print_num		if 0 print the value of the memory cell as a character, 
 *					otherwise print as a number.
 *
 * It may return the following error code :
 *  * -INVALID_MEMORY_ADDRESS when the interpreted program tries to go beyond 
 *                            the limits of the band. 
 *  * -UNKNOWN_DIALECT		  when the given dialect does not correspond to 
 *							  any of the implemented lexer.
 *  * -INVALID_OPERATION	  when the lexer could not parse the keyword into 
 *							  a token or it returned a token which is not 
 *							  supported by the interpreter. 
 */
int bfi_execute(char* program, size_t length, enum bfi_dialects dialect, char print_num);

/* Default, aka output of the interpreted program */
#define COLOR_RST		CREATE_COLOR(NORMAL, DEFAULT_COLOR, DEFAULT_COLOR)

/* Messages from the interpreter. */
#define COLOR_MSG		CREATE_COLOR(BOLD, DEFAULT_COLOR, DEFAULT_COLOR)

/* Error messages*/
#define COLOR_ERR		CREATE_COLOR(BOLD, RED, DEFAULT_COLOR)

#endif /* _INTERPRETOR_H_ */
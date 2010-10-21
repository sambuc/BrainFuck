/*
 *  Created by Lionel Sambuc on 03.12.09.
 *  Copyright 2009. All rights reserved.
 *
 */

/**************************************************************************
 *                  Brainfuck and derivative interpreter                  *
 **************************************************************************/
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "interpreter.h"

static struct
{
	enum bfi_dialects dialect;
	char print_num;
	char dialect_override;
	char const *filename;
	FILE *file;
	char *buffer;
	size_t size;
}
options;

/**************************************************************************
 *                            PRIVATE INTERFACE                           *
 **************************************************************************/
/**
 * Prints a short help.
 */
static void usage(void)
{
	printf("Usage : bfi [-b|-e] [-h] [-n] filename\n"
				 "\t-b Force plain brainfuck.\n"
				 "\t-e Force Heu dialect.\n"
				 "\t-n Print memory cells as numbers instead of characters.\n");
}

/**
 * Parses the parameters given on the command line.
 */
static int parse_cmd_line(int argc, const char *argv[])
{
	int i;
	char const *extension;
	options.dialect = -1;
	options.filename = NULL;
	options.dialect_override = 0;
	options.print_num = 0;
	
	for(i = 1; i < argc; i++) {
		/* The help option was activated. */
		if(strncmp("-h", argv[i], 2) == 0)
		{
			usage();
			return -2;
		}

		/* The brainfuck dialect override is activated */
		if(strncmp("-b", argv[i], 2) == 0 && options.dialect_override != 1)
		{
			options.dialect = BRAINFUCK;
			options.dialect_override = 1;
			continue;
		}
		
		/* The Heu dialect override is activated */
		if(strncmp("-e", argv[i], 2) == 0 && options.dialect_override != 1)
		{
			options.dialect = HEU;
			options.dialect_override = 1;
			continue;
		}
		
		/* Print memory cell values as number instead of characters. */
		if(strncmp("-n", argv[i], 2) == 0)
		{
			options.print_num = 1;
			continue;
		}
		
		for(extension = argv[i] + strlen(argv[i]);
				*extension != '.' && extension >= argv[i];
				extension--) /* Work already done! */;

		if(extension != argv[i])
		{
			if(strncmp(extension, ".bf", 3) == 0 && options.dialect == -1)
				options.dialect = BRAINFUCK;
			
			if(strncmp(extension, ".heu", 3) == 0 && options.dialect == -1)
				options.dialect = HEU;
		}

		options.filename = argv[i];
	}
	
	if(options.dialect == -1 || options.filename == NULL)
		return -1;
	
	return 0;
}

/**
 * Open and load into memory the given program.
 * Return 0 on succes or any error as set by fopen, malloc and fread.
 */
static int init(void) 
{
	int read = 0;
	struct stat file_stats;
	
	options.file = fopen(options.filename, "rb");

	if(options.file == NULL)
		return errno;
	
	/* Retrieve the file size. */
	fstat(fileno(options.file), &file_stats);
	options.size = file_stats.st_size;
	
	/* Allocate the memory buffer for the content of the file.
	 * We add one to ensure the buffer is terminated by at least one null 
	 * character. */
	options.buffer = malloc(options.size + 1);
	
	if(options.buffer == NULL)
		return errno;
	
	/* Read the file into memory. */
	read = fread(options.buffer, sizeof(char), options.size, options.file);
	if(read != options.size)
		return errno;
	
	/* Append the null character. */
	options.buffer[options.size] = '\0';
	
	/* Close the file, as we don't need it anymore. */
	fclose(options.file);
	
	return 0;
}

/**************************************************************************
 *                            PUBLIC  INTERFACE                           *
 **************************************************************************/
int main(int argc, const char *argv[])
{
	int err;
	
	/* Load parameters. */
	if((err = parse_cmd_line(argc, argv)) != 0)
	{
		if(err != -2)
		{
			printf(COLOR_ERR "Error during argument parsing." COLOR_RST "\n");
			usage();
			return 1;
		}
		else
			return 0;
	}
	
	/* Initialize the program buffer. */
	if((err = init()) != 0)
	{
		perror("init");
		printf(COLOR_ERR "Error during the loading of '%s'." COLOR_RST "\n", options.filename);
		return 1;
	}
	
	/* Print the program as loaded into memory. */
	printf(COLOR_MSG "Program : \n%s" COLOR_RST "\n", options.buffer);
	
	/* Execute the program. */
	printf(COLOR_MSG "Starting execution :" COLOR_RST "\n");
	if((err = bfi_execute(options.buffer, options.size, options.dialect, options.print_num)) > 255)
	{
		/* Return the allocated memory, and print the interpretation error. */
		free(options.buffer);

		printf(COLOR_ERR "Error %i during execution" COLOR_RST "\n", err);
		
		return err;
	}

	/* Return the allocated memory and print the program's ending code. */
	free(options.buffer);
	
	printf(COLOR_MSG "Execution ended with return value %i." COLOR_RST "\n", err);

	/* Return succes to the shell. */
	return 0;
}

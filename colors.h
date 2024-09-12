#ifndef __COLORS_H__
#define __COLORS_H__
/**
 * This macro permits to create the terminal code to modify the terminal font
 * and background color, as well as the font style (normal, bold, underline).
 *
 * @param style The font style to apply, may be one of 
 *              { NORMAL, BOLD, UNDERLINE }
 * @param fg    The font foreground color to apply, may be one of 
 *              { BLACK, RED, GREEN, YELLOW, BLUE, VIOLET, CYAN, WHITE }
 * @param bg    The font background color to apply, may be one of 
 *              { BLACK, RED, GREEN, YELLOW, BLUE, VIOLET, CYAN, WHITE }
 *
 * Authors: Lionel Sambuc
 * Copyright: © 2010 Lionel Sambuc. All rights reserved.
 */
#define _CREATE_COLOR(style, fg, bg) \
	"\033[" #style ";3" #fg ";4" #bg "m"

/* This step is needed to ensure proper replacement of the constant names to 
 * their values. */
#define CREATE_COLOR(style, fg, bg) _CREATE_COLOR(style, fg, bg)

/* Reset to standard colors */
#define DEFAULT_COLOR 8

/* Font Styles */
#define NORMAL		0
#define BOLD		1
#define UNDERLINE	4

/* Colors */
#define BLACK		0
#define RED			1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define VIOLET		5
#define CYAN		6
#define WHITE		7

#endif /* __COLORS_H__ */

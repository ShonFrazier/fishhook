//
//  tackle_example.c
//  tackle
//
//  Created by Shon Frazier on 9/4/15.
//  Copyright (c) 2015 Shon Frazier. All rights reserved.
//

#include <fcntl.h>
#include <stdarg.h>
#include "tackle_example.h"


// Declare the interposed function with the same signature as the original
FILE *my_fopen(const char *restrict filename, const char *restrict mode);

// Set up the struct for fopen
Tackle tackle_fopen = {
	.name = FUNC_NAME_fopen,
	.replacement = my_fopen,
};

int my_open(const char *path, int oflag, ...);
Tackle tackle_open = {
	.name = FUNC_NAME_open,
	.replacement = my_open,
};

Tackle *tackle_array[] = {
	&tackle_fopen,
	&tackle_open,
};


// Implement the interposed version
FILE *my_fopen(const char *restrict filename, const char *restrict mode)
{
	// Log the current call if you like
	fprintf(stdout, "Called %s(%s, %s)\n", __FUNCTION__, filename, mode);
	
	// If we're calling the original, we need the pointer
	fp_fopen real_fopen = (fp_fopen)TackleGetOriginalFunctionPointer(&tackle_fopen);
	
	// Call the original
	FILE *result = real_fopen(filename, mode);
	
	// Log the result if you like
	fprintf(stdout, "  result: %p\n", result);
	
	return result;
}


int my_open(const char *path, int oflag, ...)
{
	mode_t mode = 0;
	if (oflag & O_CREAT) // If we require the mode parameter...
	{
		va_list args;
		va_start(args, oflag);
		mode = va_arg(args, int); // get the mode paramater
		va_end(args);
	}
	
	fprintf(stdout, "Called %s(%s, %i, %i)\n", __FUNCTION__, path, oflag, mode);
	
	fp_open real_open = (fp_open)TackleGetOriginalPointerForFunctionNamed(FUNC_NAME_open);
	int result = real_open(path, oflag, mode);
	
	fprintf(stdout, "  result: %i\n", result);
	
	return result;
}


void ATTR_CONSTRUCT tackle_example_init(void)
{
	fprintf(stdout, "%s\n", __FUNCTION__);
	
	Tackle *t = NULL;
	int index = 0;
	int item_count = sizeof(tackle_array) / sizeof(Tackle *);
	
	for (index = 0; index < item_count; index++)
	{
		t = tackle_array[index];
		TackleHook(t);
	}
}

void ATTR_DESTRUCT tackle_example_deinit(void)
{
	fprintf(stdout, "%s\n", __FUNCTION__);
	
	Tackle *t = NULL;
	int index = 0;
	int item_count = sizeof(tackle_array) / sizeof(Tackle *);
	
	for (index = 0; index < item_count; index++)
	{
		t = tackle_array[index];
		TackleUnhook(t);
	}
}

//
//  tackle_example_main.c
//  tackle
//
//  Created by Shon Frazier on 9/4/15.
//  Copyright (c) 2015 Shon Frazier. All rights reserved.
//

#include <stdio.h>

int main(int argc, char *argv[])
{
	const char *path = "/dev/null";
	
	if (argc > 1)
	{
		path = argv[1];
	}
	
	FILE *file = fopen(path, "r");
	
	fprintf(stdout, "Opened %s at %p\n", path, file);
	
	fclose(file);
	file = NULL;
	
	return 0;
}

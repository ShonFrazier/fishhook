//
//  tackle_example.h
//  tackle
//
//  Created by Shon Frazier on 9/4/15.
//  Copyright (c) 2015 Shon Frazier. All rights reserved.
//

#ifndef __tackle__tackle_example__
#define __tackle__tackle_example__

#include <stdio.h>
#include "tackle.h"

// Create a typedef for the original function signature. Prefixing the original name with 'fp_' provides a description
// and a unique name.
typedef FILE *(*fp_fopen)(const char *restrict filename, const char *restrict mode);

// If you'll be looking up functions by name from other places in your app, define such that the compiler helps us out.
#define FUNC_NAME_fopen "fopen"

// Let's also look at a variadic function...
typedef int(*fp_open)(const char *path, int oflag, ...);
#define FUNC_NAME_open "open"


// Set this to 0 to call tackle_example_init from elsewhere rather than on load
#define AUTO_CALL_INIT	1

#if AUTO_CALL_INIT
#define ATTR_CONSTRUCT __attribute__((constructor))
#define ATTR_DESTRUCT __attribute__((destructor))
#else
#define ATTR_CONSTRUCT
#define ATTR_DESTRUCT
#endif

// This function performs interposing. Strictly speaking, if AUTO_CALL_INIT is set to 1, this declaration in the header
// isn't required.
void ATTR_CONSTRUCT tackle_example_init(void);
void ATTR_DESTRUCT tackle_example_deinit(void);

#endif /* defined(__tackle__tackle_example__) */

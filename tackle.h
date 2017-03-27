//
//  tackle.h
//  tackle
//
//  Created by Shon Frazier on 9/3/15.
//  Copyright (c) 2015 Shon Frazier. All rights reserved.
//

#ifndef __tackle__tackle__
#define __tackle__tackle__

#import <stdbool.h>
#include "fishhook.h"


#if __cplusplus
extern "C" {
#endif


//MARK: Types
/**
 *  Tackle struct contains metadata for interposing functions. Should be treated as opaque. Do not serialize.
 */
typedef struct Tackle
{
	const char *name; // original function name to interpose
	void *original; // addres of original function
	void *replacement; // address of replacement function
	int lastRebindStatus; // the return value of the most recent call to rebind_symbol for this Tackle struct
	bool currentlyHooked; // whether the replacement function is currently interposed in place of the original function
} Tackle;
	


/**
 *  An alias
 */
typedef struct rebinding rebinding;


//MARK: Lifecycle
	
/**
 *  Create a new Tackle struct. The function_name parameter must match the exported symbol name. For example, to
 *  interpose fopen(3), this string will contain "fopen". In the case of C++ functions, the mangled name must be used.
 *
 *  @param function_name       A C string representing the name of the function to interpose.
 *  @param replacement_pointer A pointer to your implementation of the function.
 *
 *  @return A pointer to a new Tackle struct. If function_name is NULL, return value is NULL.
 */
Tackle *TackleCreate(const char *function_name, void *replacement_pointer);
	
/**
 *  Free the memory assigned to a Tackle struct.
 *
 *  @param t A pointer to the memory to be freed.
 */
void TackleFree(Tackle *t);


//MARK: Properties

/**
 *  Get the address of the original function stored in this Tackle struct.
 *
 *  @param t A Tackle object. If NULL, return value is NULL.
 *
 *  @return A pointer to the original function. NULL if not found, or if parameter t is NULL.
 */
void *TackleGetOriginalFunctionPointer(Tackle *t);
	
/**
 *  Get the string name of the function stored in this Tackle struct.
 *
 *  @param t A Tackle object. If NULL, return value is NULL.
 *
 *  @return A pointer to a C string representing the function's name. NULL if parameter t is NULL.
 */
const char *TackleGetFunctionName(Tackle *t);
	
/**
 *  Get a fishhook rebinding struct that represents the interposed function. Used for initial function hooking.
 *
 *  @param t t A Tackle object. If NULL, the return value's members are NULL.
 *
 *  @return A rebinding struct.
 */
rebinding TackleGetReplacementRebinding(Tackle *t);
	
/**
 *  Get a fishhook rebinding struct that represents the oritinal function. Used when deinterposing functions.
 *
 *  @param t A Tackle object. If NULL, the return value's members are NULL.
 *
 *  @return A rebinding struct.
 */
rebinding TackleGetOriginalRebinding(Tackle *t);


//MARK: Verbs

/**
 *  Fills in the 'original' member with the real function's address, assuming interposition has not yet occurred.
 *
 *  @param t A Tackle object. If NULL, nothing happens.
 */
void TackleFill(Tackle *t);

/**
 *  Perform function interposition based on the values in the Tackle object.
 *
 *  @param t A Tackle object. If NULL, no interposition is performed and false is returned.
 *
 *  @return true if rebinding is successful. false if t is NULL or if rebinding fails.
 */
bool TackleHook(Tackle *t);
	
/**
 *  Undo interposition based on the values in the Tackle object.
 *
 *  @param t A Tackle object. If NULL, no interposition is performed and false is returned.
 *
 *  @return true if rebinding is successful. false if t is NULL or if rebinding fails.
 */
bool TackleUnhook(Tackle *t);


//MARK: Utility

/**
 *  Get the original function pointer for the named function whether interposed or not.
 *
 *  @param function_name A C string representing the function.
 *
 *  @return A pointer to the desired function, or NULL if a matching Tackle object cannot be found or if function_name 
 *  is NULL.
 */
void *TackleGetOriginalPointerForFunctionNamed(const char *function_name);


#if __cplusplus
}
#endif


#endif /* defined(__tackle__tackle__) */

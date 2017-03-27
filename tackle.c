//
//  tackle.c
//  tackle
//
//  Created by Shon Frazier on 9/3/15.
//  Copyright (c) 2015 Shon Frazier. All rights reserved.
//

#include "tackle.h"

#import <string.h>
#import <stdlib.h>
#import <dlfcn.h>



/**
 *  For keeping track of all Tackle structs so that we can find by name later.
 */
typedef struct TackleListNode TackleListNode;
typedef struct TackleList TackleList;

struct TackleListNode
{
	Tackle *tackle;
	TackleListNode *nextNode;
	TackleListNode *previousNode;
};

struct TackleList
{
	TackleListNode *head;
	TackleListNode *tail;
	size_t nodeCount;
};


TackleListNode *TackleListCreateNodeForTackle(Tackle *t);
TackleListNode *TackleListGetNodeForTackle(TackleList *tl, Tackle *t);
void TackleListNodeFree(TackleListNode *tln);

TackleList *TackleListCreate(void);
void TackleListRemoveTackle(TackleList *list, Tackle *t);
void TackleListAddTackle(TackleList *list, Tackle *t);
bool TackleListContainsTackle(TackleList *list, Tackle *t, TackleListNode **ptln);


/**
 *  Creat a new list node for the given Tackle object
 *
 *  @param t A Tackle object.
 *
 *  @return A new TackleListNode object.
 */
TackleListNode *TackleListCreateNodeForTackle(Tackle *t)
{
	if (t == NULL)
	{
		return NULL;
	}
	
	TackleListNode *tln = (TackleListNode *)malloc(sizeof(TackleListNode));
	tln->tackle = t;
	tln->nextNode = NULL;
	
	return tln;
}

/**
 *  Find the existing node for a Tackle object.
 *
 *  @param tl The list that might contain the Tackle object.
 *  @param t  The Tackle object.
 *
 *  @return The node containing the Tackle object, or NULL if not found or if either parameter is NULL.
 */
TackleListNode *TackleListGetNodeForTackle(TackleList *tl, Tackle *t)
{
	if (tl == NULL || t == NULL)
	{
		return NULL;
	}
	
	TackleListNode *tln = tl->head;
	while (tln != NULL)
	{
		if (tln->tackle == t)
		{
			return tln;
		}
		
		tln = tln->nextNode;
	}
	
	return NULL;
}

/**
 *  Release the memory held for a node.
 *
 *  @param tln The node whose memory should be reclaimed.
 */
void TackleListNodeFree(TackleListNode *tln)
{
	if (tln != NULL)
	{
		free(tln);
	}
}

/**
 *  Create a new list.
 *
 *  @return The new TackleList object.
 */
TackleList *TackleListCreate(void)
{
	TackleList *tl = (TackleList *)malloc(sizeof(TackleList));
	if (tl != NULL)
	{
		tl->head = NULL;
		tl->tail = NULL;
		tl->nodeCount = 0;
	}
	
	return tl;
}

/**
 *  Release the memory held for a list object. Does indeed remove and release all nodes still in the list. DOES NOT free
 *  the contained Tackle objects.
 *
 *  @param tl The list whose memory should be reclaimed.
 */
void TackleListFree(TackleList *tl)
{
	if (tl != NULL)
	{
		TackleListNode *tln = tl->tail;
		while (tln != NULL)
		{
			TackleListRemoveTackle(tl, tl->tail->tackle);
			
			tln = tl->tail;
		}
		
		free(tl);
		tl = NULL;
	}
}

/**
 *  Append a node to the end of the list. If either parameter is NULL, this function does nothing.
 *
 *  @param list A TackleList object.
 *  @param node A TackleListNode object.
 */
void TackleListAppendNode(TackleList *list, TackleListNode *node)
{
	if (list == NULL || node == NULL)
	{
		return;
	}
	
	if (list->head == NULL)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		TackleListNode *previous = list->tail;
		previous->nextNode = node;
		
		node->previousNode = previous;
		
		list->tail = node;
	}
	
	list->nodeCount++;
}

/**
 *  Remove a node from within a list. This function will release the node. If either parameter is NULL, this function
 *  does nothing.
 *
 *  @param list A TackleList object.
 *  @param node A TackleListNode object.
 */
void TackleListRemoveNode(TackleList *list, TackleListNode *node)
{
	if (list == NULL || node == NULL)
	{
		return;
	}
	
	TackleListNode *previous = node->previousNode;
	TackleListNode *next = node->nextNode;
	
	if (previous == NULL) // current is head node
	{
		list->head = next; // next is new head node
		next->previousNode = NULL; // head has no previous
	}
	else if (next != NULL)
	{
		next->previousNode = previous; // skip back to previous
	}
	
	node->previousNode = NULL;
	
	
	if (next == NULL) // current is tail node
	{
		list->tail = previous; // previous is new tail
		previous->nextNode = NULL; // tail has no next
	}
	else if (previous != NULL)
	{
		previous->nextNode = next; // skip forward to next
	}
	
	node->nextNode = NULL;
	
	list->nodeCount--;
	
	TackleListNodeFree(node);
	node = NULL;
}

/**
 *  Adds a Tackle object to the list. A new node is created for the Tackle object and the node is appended to the list.
 *  If the object is already in the list, this function DOES NOT create a new node object and DOES NOT modify the list.
 *  If either parameter is NULL, this function does nothing.
 *
 *  @param list    A TackleList object.
 *  @param tackle  A Tackle object to add to the list.
 */
void TackleListAddTackle(TackleList *list, Tackle *tackle)
{
	if (list == NULL || tackle == NULL)
	{
		return;
	}
	
	if (TackleListContainsTackle(list, tackle, NULL))
	{
		return;
	}
	
	TackleListNode *node = TackleListCreateNodeForTackle(tackle);
	if (node == NULL)
	{
		return;
	}
	
	TackleListAppendNode(list, node);
}

/**
 *  Remove a Tackle object's node from a list. The node will be freed. If wither parameter is NULL, this function does
 *  nothing.
 *
 *  @param list    A TackleList object.
 *  @param tackle  A Tackle object to add to the list.
 */
void TackleListRemoveTackle(TackleList *list, Tackle *tackle)
{
	if (list == NULL || tackle == NULL)
	{
		return;
	}
	
	TackleListNode *node = TackleListGetNodeForTackle(list, tackle);
	if (node == NULL)
	{
		return;
	}
	
	TackleListRemoveNode(list, node);
}

/**
 *  Determine whether a list contains a Tackle object. Optionally, fetch the node for the Tackle object.
 *
 *  @param list    A TackleList object.
 *  @param tackle  A Tackle object to add to the list.
 *  @param pnode   A pointer to a TackleListNode pointer. If not NULL, *ptln will point to the located node, or will be
 *                 set to NULL if the Tackle object could not be found in the list.
 *
 *  @return true if the Tackle object was found in the list, false otherwise. False if either list or node are NULL.
 */
bool TackleListContainsTackle(TackleList *list, Tackle *tackle, TackleListNode **pnode)
{
	if (list == NULL || tackle == NULL)
	{
		return false;
	}
	
	TackleListNode *node = TackleListGetNodeForTackle(list, tackle);
	
	if (pnode != NULL)
	{
		*pnode = node;
	}
	
	bool found = (node != NULL);
	
	return found;
}


/**
 *  The list of Tackle objects.
 */
static TackleList *FullTackleList = NULL;
TackleList *GetTackleList(void)
{
	if (FullTackleList == NULL) {
		FullTackleList = TackleListCreate();
	}
	
	return FullTackleList;
}

/**
 *  Obtain a Tackle object based on the original function name.
 *
 *  @param name The name of the original function.
 *
 *  @return If the function name was found, a pointer to a Tackle object. NULL if not found or if name is NULL.
 */
Tackle *TackleByName(const char *name)
{
	if (name == NULL)
	{
		return NULL;
	}
	
	TackleList *tl = GetTackleList();
	if (tl == NULL)
	{
		return NULL;
	}
	
	TackleListNode *tln = tl->head;
	while (tln != NULL)
	{
		const char *currentTackleName = tln->tackle->name;
		if (currentTackleName == NULL)
		{
			continue;
		}
		
		if (strcmp(name, currentTackleName) == 0)
		{
			return tln->tackle;
		}
		
		tln = tln->nextNode;
	}
	
	return NULL;
}

Tackle *TackleCreate(const char *name, void *replacement)
{
	if (name == NULL)
	{
		return NULL;
	}
	
	Tackle *t = (Tackle *)malloc(sizeof(Tackle));
	t->name = name;
	t->replacement = replacement;
	t->original = NULL;
	
	TackleListAddTackle(GetTackleList(), t);
	
	TackleFill(t);
	
	return t;
}

void TackleFree(Tackle *tackle)
{
	if (tackle != NULL)
	{
		free(tackle);
		tackle = NULL;
	}
}

void TackleFill(Tackle *tackle)
{
	if (tackle == NULL)
	{
		return;
	}
	
	if (tackle->name == NULL)
	{
		return;
	}
	
	if (tackle->original == NULL)
	{
		tackle->original = dlsym(RTLD_DEFAULT, tackle->name);
	}
}

rebinding TackleGetReplacementRebinding(Tackle *tackle)
{
	rebinding r;
	
	if (tackle != NULL)
	{
		r.name = tackle->name;
		r.replacement = tackle->replacement;
	}
	else
	{
		r.name = NULL;
		r.replacement = NULL;
	}
	
	return r;
}

rebinding TackleGetOriginalRebinding(Tackle *t)
{
	rebinding r;
	
	r.name = t->name;
	r.replacement = t->original;
	
	return r;
}

void *TackleGetOriginalFunctionPointer(Tackle *t)
{
	return t->original;
}

const char *TackleGetFunctionName(Tackle *t)
{
	return t->name;
}

bool TackleHook(Tackle *t)
{
	if (t == NULL) // Can't operate on NULL.
	{
		return false;
	}
	
	TackleFill(t);
	
	if(t->original == NULL || t->replacement == NULL) // Can't rebind NULL pointers.
	{
		t->lastRebindStatus = (int)TackleHook; //FIXME: Need result codes?
		
		return false;
	}
	
	if (t->currentlyHooked == true) // Don't rebind if we're already hooked.
	{
		return true;
	}
	
	rebinding r = TackleGetReplacementRebinding(t);
	
	int result = rebind_symbols(&r, 1);
	bool success = (result == 0);
	
	t->lastRebindStatus = result;
	t->currentlyHooked = success;
	
	return success;
}

bool TackleUnhook(Tackle *t)
{
	if (t == NULL) // Can't operate on NULL.
	{
		return false;
	}
	
	if (t->original == NULL || t->replacement == NULL) // Can't rebind NULL pointers.
	{
		t->lastRebindStatus = (int)TackleUnhook; //FIXME: Need result codes?
		
		return false;
	}
	
	if (t->currentlyHooked == false) // Don't rebind if we're already unhooked.
	{
		return true;
	}
	
	rebinding r = TackleGetOriginalRebinding(t);
	
	int result = rebind_symbols(&r, 1);
	bool success = (result == 0);
	
	t->lastRebindStatus = result;
	t->currentlyHooked = success;
	
	return success;
}

void *TackleGetOriginalPointerForFunctionNamed(const char *name)
{
	if (name == NULL)
	{
		return NULL;
	}
	
	void *fp = NULL;
	
	Tackle *t = TackleByName(name);
	if (t != NULL)
	{
		TackleFill(t); // Just in case we haven't populated the 'original' member.
		
		fp = TackleGetOriginalFunctionPointer(t);
	}
	
	return fp;
}

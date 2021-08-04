#ifndef __included_neighbors_h
#define __included_neighbors_h

extern int malloc_count;

#include "Define.h"

/* Circular Doubly Linked List that holds X & Y coordinates instead of full nodes */

/* Create a new list */
struct neighbor_xy_list *neighbor_xy_new();

/* Clean list */
void neighbor_xy_clean(struct neighbor_xy_list *head);

/* Add to the list */
struct neighbor_xy_list *neighbor_xy_insert_right(struct neighbor_xy_list *list, int x, int y);

/* Find all neighbors adjecent to a node ( return X/Y coordinate list, rather than full nodes ) */
//struct neighbor_xy_list *_findNeighbors(NODE* activeNode);

#endif

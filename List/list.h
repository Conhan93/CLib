#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdlib.h>
#include <assert.h>

typedef struct listnode {
    void* element;
    struct listnode* last;
    struct listnode* next;
} ListNodeT;


typedef struct listT {
    unsigned long size;
    ListNodeT* first;
    ListNodeT* last;

    int (*addFirst)(struct listT*, const void*);
    int (*addLast)(struct listT*, const void*);
    void* (*find)(struct listT*, const void*);
    int (*drop)(struct listT*, void*);
    void (*sort)(struct listT* _list, int (*compare)(void*, void*));


} ListT;

typedef struct list_iter {
    ListNodeT* current;
    ListNodeT* first;

    void* (*next)(struct list_iter*);
    void* (*prev)(struct list_iter*);

} ListIterT;

ListT* newList();
void destroyList(ListT** _list);

ListIterT* newListIterator(ListT* _list);
int destroyListIterator(ListIterT** _iter);

#endif
#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdlib.h>
#include <assert.h>

typedef struct list_node {
    void* element;
    struct list_node* last;
    struct list_node* next;
} list_node_t;


typedef struct list_source {
    unsigned long size;
    list_node_t* first;
    list_node_t* last;

    int (*push_front)(struct list_source*, const void*);
    int (*push_back)(struct list_source*, const void*);
    void* (*find)(struct list_source*, const void*);
    void* (*remove)(struct list_source*, void*);
    void (*sort)(struct list_source* _list, int (*compare)(void*, void*));
    void (*reverse)(struct list_source* _list);
    void* (*pop_back)(struct list_source*);
    void* (*pop_front)(struct list_source*);


} list_t;

typedef struct list_iter {
    list_node_t* current;
    list_node_t* first;

    void* (*next)(struct list_iter*);
    void* (*prev)(struct list_iter*);

} list_iter_t;

list_t* newList();
void destroyList(list_t** _list);

list_iter_t* newListIterator(list_t* _list);
int destroyListIterator(list_iter_t** _iter);

#endif
#include "list.h"
#include <stdio.h>

// instantiates a new list and initializes memory

void destroyList(ListT** _list) {

    ListNodeT* node = (*_list)->first, *temp = NULL;

    while(node) {
        temp = node;
        node = node->next;
        free(temp);
    }
    free(node);
    free(*_list);

    *_list = NULL;
}

static int plistaddFirst(ListT* _list, const void* _element) {

    assert(_list);
    assert(_element);

    ListNodeT* new = malloc(sizeof(ListNodeT));
    assert(new);

    
    if(!_list->size) {
        _list->last = new;
    }

    new->element = _element;
    new->last = NULL;
    new->next = _list->first;
    _list->first = new;

    _list->size++;

    return 1;
}

static int plistaddLast(ListT* _list, const void* _element) {
    
    assert(_list);
    assert(_element);

    ListNodeT* new = malloc(sizeof(ListNodeT));
    assert(new);

    new->element = _element;

    if(!_list->size) {
        _list->first = _list->last = new;
        
    } else {
        _list->last->next = new;
        new->last = _list->last;
        _list->last = new;
        _list->last->next = NULL;
    }
    
    _list->size++;

    return 1;
}

static void* plistfind(ListT* _list, const void* _element) {
    int count = 0;

    for(
        ListNodeT* cur = _list->first ;
        cur != NULL /*_list->last*/ && count < _list->size ;
        cur = cur->next, count++
        ) {
            printf("looping, val : %s\n", (char*)cur->element);
            if(cur->element == _element)
                return (void*) cur->element;
            count++;
        }
    return NULL;
}
static int plistdrop(ListT* _list, void* _element)  {
    if(_list->size)
        for(
            ListNodeT* cur = _list->first, *last = _list->last ;
            cur != _list->last ;
            last = cur , cur = cur->next
            ) {
                if(cur->element == _element)  {
                   --_list->size;

                    if(!_list->size) {
                        free(cur);
                        _list->first = NULL;
                        _list->last = NULL;
                        return 1;
                    }

                    last->next = cur->next;
                    cur->next->last = last;
                    
                    free(cur);

                    return 1;
                }
            }
    return 0;
}
static void plistdropLast(ListT* _list) {
    plistdrop(_list, _list->last->element);
}
static void plistdropFirst(ListT* _list) {
    plistdrop(_list, _list->first->element);
}
static void plistsort(ListT* _list, int (*compare)(void*, void*)) {

    if(!_list->size)
        return;
    
    ListNodeT* current = _list->first, *next = NULL;
    void* temp = NULL;

    while(current->next) {
        next = current->next;

        while(next) {

            if(compare(current->element, next->element) > 0) {
                temp = current->element;
                current->element = next->element;
                next->element = temp;        
            }
            next = next->next;
        }
        
        current = current->next;
    }
}
ListT* newList() {
    ListT* newlist = malloc(sizeof(ListT));
    
    newlist->first = NULL;
    newlist->last = NULL;

    newlist->size = 0;

    newlist->addFirst = &plistaddFirst;
    newlist->addLast = &plistaddLast;
    newlist->drop = &plistdrop;
    newlist->find = &plistfind;
    newlist->sort = &plistsort;

    return newlist;
}

/////////////////// --- list iterator

static void* plistiternext(ListIterT* _iter) {
    assert(_iter);

    void* element = NULL;

    if(!_iter->current) {
        _iter->current = _iter->first;

        element = _iter->current->element;

    } else if(_iter->current->next){
        _iter->current = _iter->current->next;

        element = _iter->current->element;
    }

    return element;
}
// TODO : fix else statement
static void* plistiterprev(ListIterT* _iter) {
    assert(_iter);

    if(!_iter->current)
        _iter->current = _iter->first;
    else
        _iter->current = _iter->current->last;

    return _iter->current->element;
}
ListIterT* newListIterator(ListT* _list) {
    puts("mallocing");
    ListIterT* iter = malloc(sizeof(ListIterT));
    puts("malloced iter");

    iter->first = _list->first;
    puts("first");
    iter->current = NULL;
    puts("current");
    iter->next = &plistiternext;
    puts("next");
    iter->prev = &plistiterprev;
    puts("prev");

    return iter;
}

int destroyListIterator(ListIterT** _iter) {
    (*_iter)->current = (*_iter)->first = NULL; 
    (*_iter)->next = (*_iter)->prev = NULL;

    free(*_iter);

    return (*_iter = NULL) ? 0 : 1;
}

#include "list.h"
#include <stdio.h>

// instantiates a new list and initializes memory

// instantiates a new list and initializes memory

void destroyList(list_t** _list) {

    list_node_t* node = (*_list)->first, *temp = NULL;

    while(node) {
        temp = node;
        node = node->next;
        free(temp);
    }
    free(node);
    free(*_list);

    *_list = NULL;
}

static int p_push_front(list_t* _list, const void* _element) {

    assert(_list);
    assert(_element);

    list_node_t* new = malloc(sizeof(list_node_t));
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

static int p_push_back(list_t* _list, const void* _element) {
    
    assert(_list);
    assert(_element);

    list_node_t* new = malloc(sizeof(list_node_t));
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

static void* plistfind(list_t* _list, const void* _element) {

    for(list_node_t* cur = _list->first ; cur != NULL; cur = cur->next) {
            if(cur->element == _element)
                return (void*) cur->element;
        }
    return NULL;
}
static int p_remove(list_t* _list, void* _element)  {
    if(_list->size)
        for(
            list_node_t* cur = _list->first, *last = _list->last ;
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
static void p_pop_back(list_t* _list) {
    plistdrop(_list, _list->last->element);
}
static void p_pop_front(list_t* _list) {
    plistdrop(_list, _list->first->element);
}
static void p_sort(list_t* _list, int (*compare)(void*, void*)) {

    if(!_list->size)
        return;
    
    list_node_t* current = _list->first, *next = NULL;
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
list_t* newList() {
    list_t* newlist = malloc(sizeof(list_t));
    
    newlist->first = NULL;
    newlist->last = NULL;

    newlist->size = 0;

    newlist->push_front = &p_push_front;
    newlist->push_back = &p_push_back;
    newlist->remove = &p_remove;
    newlist->find = &plistfind;
    newlist->sort = &p_sort;
    newlist->pop_back = &p_pop_back;
    newlist->pop_front = &p_pop_front;

    return newlist;
}

/////////////////// --- list iterator

static void* plistiternext(list_iter_t* _iter) {
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
static void* plistiterprev(list_iter_t* _iter) {
    assert(_iter);

    void* element = NULL;

    if(!_iter->current) {
        _iter->current = _iter->first;
        element = _iter->current->element;
    }
        
    else if(_iter->current->last) {
        _iter->current = _iter->current->last;

        element = _iter->current->element;
    }
        
    return element;
}
list_iter_t* newListIterator(list_t* _list) {
    list_iter_t* iter = malloc(sizeof(list_iter_t));

    iter->first = _list->first;
    iter->current = NULL;
    iter->next = &plistiternext;
    iter->prev = &plistiterprev;

    return iter;
}

int destroyListIterator(list_iter_t** _iter) {
    (*_iter)->current = (*_iter)->first = NULL; 
    (*_iter)->next = (*_iter)->prev = NULL;

    free(*_iter);

    return (*_iter = NULL) ? 0 : 1;
}

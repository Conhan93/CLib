#include "list.h"
#include <stdio.h>



void destroyList(list_t** _list) {

    list_node_t* node = (*_list)->first, *temp = NULL;

    while(node) {
        temp = node;
        node = node->next;
        free(temp);
        temp = NULL;
    }
    free(node);
    node = NULL;
    free(*_list);

    *_list = NULL;
}

static int p_push_front(list_t* _list, const void* _element) {

    assert(_list);
    assert(_element);

    list_node_t* new;
    if(!(new = malloc(sizeof(list_node_t))))
        return 0;

    new->element = _element;
    
    if(!_list->size) {
        _list->first = _list->last = new;
        _list->size++;
        return 1;
    }

    
    new->last = NULL;
    new->next = _list->first;
    _list->first->last = new;
    _list->first = new;

    _list->size++;

    return 1;
}

static int p_push_back(list_t* _list, const void* _element) {
    
    assert(_list);
    assert(_element);

    list_node_t* new;
    if(!(new = malloc(sizeof(list_node_t))))
        return 0;

    new->element = _element;

    if(!_list->size) {
        _list->first = _list->last = new;
        _list->size++;
        return 1;
        
    } else {
        new->next = NULL;
        new->last = _list->last;
        _list->last->next = new;
        _list->last = new;
    }
    
    _list->size++;

    return 1;
}

static void* p_find(list_t* _list, const void* _element) {

    for(list_node_t* cur = _list->first ; cur != NULL; cur = cur->next) {
            if(cur->element == _element)
                return cur->element;
        }
    return NULL;
}
static void* p_pop_back(list_t* _list) {
    if(!_list->size)
        return NULL;

    list_node_t* last = _list->last;
    void* element = last->element;

    _list->last = last->last;
    _list->last->next = NULL;

    last->element = NULL;
    last->last = NULL;
    last->next = NULL;

    free(last);
    last = NULL;

    return element;
}
static void* p_pop_front(list_t* _list) {
    if(!_list->size)
        return NULL;

    list_node_t* first = _list->first;
    void* element = first->element;

    _list->first = first->next;
    _list->first->last = NULL;

    first->element = NULL;
    first->last = NULL;
    first->next = NULL;

    free(first);
    first = NULL;

    return element;
}
static void* p_remove(list_t* _list, void* _element)  {

    void* element = NULL;

    if(_list->size)
        for(list_node_t* cur = _list->first; cur != NULL  ; cur = cur->next) {
            if(cur->element == _element)  {
                element = cur->element;
               --_list->size;

                if(!_list->size) { // empty list
                    free(cur);
                    _list->first = NULL;
                    _list->last = NULL;
                    return element;
                } 
                if(!cur->last) // remove first element
                    return p_pop_front(_list);
                if(!cur->next) // remove last element
                    return p_pop_back(_list);

                list_node_t* next = cur->next, *last = cur->last;
                last->next = next;
                next->last = last;

                cur->element = NULL;
                cur->last = NULL;
                cur->next = NULL;

                free(cur);
                cur = NULL;

                return element;
            }
        }
    return element;
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
static void p_reverse(list_t* _list) {
    if(!_list->size)
        return;
    list_node_t* first = _list->first, *last = _list->last;
    void* temp = NULL;
    while(first != last) {
        temp = first->element;
        first->element = last->element;
        last->element = temp;

        assert(first->next); assert(last->last);
        first = first->next;
        if(first == last) break;
        last = last->last;
    }
}
list_t* newList() { // instantiates a new list and initializes memory
    list_t* newlist;

    if(!(newlist = malloc(sizeof(list_t))))
        return NULL;
    
    newlist->first = NULL;
    newlist->last = NULL;

    newlist->size = 0;

    newlist->push_front = &p_push_front;
    newlist->push_back = &p_push_back;
    newlist->remove = &p_remove;
    newlist->find = &p_find;
    newlist->sort = &p_sort;
    newlist->reverse = &p_reverse;
    newlist->pop_back = &p_pop_back;
    newlist->pop_front = &p_pop_front;

    return newlist;
}

/////////////////// --- list iterator

static void* p_list_iter_next(list_iter_t* _iter) {
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
static void* p_list_iter_prev(list_iter_t* _iter) {
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
    list_iter_t* iter;
    if(!(iter = malloc(sizeof(list_iter_t))))
        return NULL;

    iter->first = _list->first;
    iter->current = NULL;
    iter->next = &p_list_iter_next;
    iter->prev = &p_list_iter_prev;

    return iter;
}

int destroyListIterator(list_iter_t** _iter) {
    (*_iter)->current = (*_iter)->first = NULL; 
    (*_iter)->next = (*_iter)->prev = NULL;

    free(*_iter);

    return (*_iter = NULL) ? 0 : 1;
}

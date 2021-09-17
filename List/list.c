#include "list.h"
#include <stdio.h>



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

static int push_front(list_t* _list, const void* _element) {

    if(!_list | !_element) return 0;

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

static int push_back(list_t* _list, const void* _element) {
    
    if(!_list | !_element) return 0;

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

static void* find(list_t* _list, const void* _element) {

    for(list_node_t* cur = _list->first ; cur != NULL; cur = cur->next) {
            if(cur->element == _element)
                return cur->element;
        }
    return NULL;
}
static void* pop_back(list_t* _list) {
    if(!_list->size)
        return NULL;

    list_node_t* last = _list->last;
    void* element = last->element;

    _list->last = last->last;
    _list->last->next = NULL;

    free(last);
    last = NULL;

    return element;
}
static void* pop_front(list_t* _list) {
    if(!_list->size)
        return NULL;

    list_node_t* first = _list->first;
    void* element = first->element;

    _list->first = first->next;
    _list->first->last = NULL;

    free(first);
    first = NULL;

    return element;
}
static void* remove(list_t* _list, void* _element)  {

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

                free(cur);
                cur = NULL;

                return element;
            }
        }
    return element;
}

static void sort(list_t* _list, int (*compare)(void*, void*)) {

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
static void reverse(list_t* _list) {
    if(!_list->size)
        return;
    list_node_t* first = _list->first, *last = _list->last;
    void* temp = NULL;
    while(first != last) {
        temp = first->element;
        first->element = last->element;
        last->element = temp;

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

    newlist->push_front = &push_front;
    newlist->push_back = &push_back;
    newlist->remove = &remove;
    newlist->find = &find;
    newlist->sort = &sort;
    newlist->reverse = &reverse;
    newlist->pop_back = &pop_back;
    newlist->pop_front = &pop_front;

    return newlist;
}

/////////////////// --- list iterator

static void* iter_next(list_iter_t* _iter) {
    if(!_iter) return NULL;

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
static void* iter_prev(list_iter_t* _iter) {
    if(!_iter) return NULL;

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
    iter->next = &iter_next;
    iter->prev = &iter_prev;

    return iter;
}

int destroyListIterator(list_iter_t** _iter) {

    free(*_iter);

    return (*_iter = NULL) ? 0 : 1;
}

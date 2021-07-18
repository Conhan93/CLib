#include "test_util.h"

// fill container with random int values
void fillContainerInt(void (*addToContainer)(void*, void*), void* _container, unsigned amount, unsigned range) {
    int* value = NULL;

    for(int index = 0 ; index < amount ; index++) {
        value = malloc(sizeof(int));
        *value = (rand() % range) + 1;
        addToContainer(_container, value);
    }
}
int compareInt(void* a, void* b) {
    int* first = a, *second = b;

    return *first > *second ? 1 : *first < *second ? -1 : 0; 
}
#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <stdlib.h>
#include <time.h>

void fillContainerInt(void (*addToContainer)(void*, void*),
                void* _container, unsigned amount, unsigned range);


int compareInt(void* a, void* b);

#endif
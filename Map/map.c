#include "map.h"
#include "stdlib.h"


///////// MAP
map_t* new_map() {
    map_t* _map = malloc(sizeof(map_t));
    if(!_map) return NULL;

    _map->head = NULL;

    return _map;
}

void destroy_map(map_t* _map) {

}


////////// MAP_NODE

static map_node_t* new_node(void* key, size_t key_size, void* value, size_t value_size) {
    if(key_size <= 0 || value_size <= 0)
        return NULL;

    map_node_t* new_node = malloc(sizeof(map_node_t));
    if(!new_node) return NULL;

    new_node->key = malloc(sizeof(key_size));
    if(!new_node->key) return NULL;
    new_node->key_size = key_size;

    new_node->value = malloc(sizeof(key_size));
    if(!new_node->value) return NULL;
    new_node->value_size = value_size;

    return new_node;
} 

static void destroy_node() {
    
}
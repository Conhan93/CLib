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

int map_add(map_t* _map, void* key, size_t k_size, void* value, size_t v_size) {

    map_node_t* _node = new_node(key, k_size, value, v_size);
    if(!_node) return 0;

    map_node_t* p = _map->head;

    while(p = p->next) ;

    p->next = _node;
    _node->prev = p;

    return 1;
}

void map_remove(map_t* _map, void* key, size_t k_size) {

    map_node_t* _node = _map->head;
    while(_node->next) {
        if(compare_key(_node, key, k_size))
            break;
    }

    if(_node) {
        // remove node from map
    }
}


static int compare_key(map_node_t* _node, void* key, size_t k_size) {

    if(_node->key_size != k_size) return 0;

    unsigned char* node_key = _node->key;
    unsigned char* cmp_key = key;
    const unsigned char* end_pos = node_key + k_size;

    while(node_key != end_pos)
        if(*node_key++ != *cmp_key++)
            return 0;
    
    return 1;
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

    new_node->next = NULL;
    new_node->prev = NULL;

    return new_node;
} 

static void destroy_node() {

}
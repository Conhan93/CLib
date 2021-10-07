#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED



typedef struct {
    void* key;
    void* value;

    size_t key_size;
    size_t value_size;
} map_node_t;

typedef struct {
    map_node_t* head;
} map_t;




map_t* new_map();
void destroy_map(map_t* _map);

#endif
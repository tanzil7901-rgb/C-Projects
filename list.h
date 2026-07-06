#ifndef LIST_H 
#define LIST_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define list_add(list, item_ptr, type) do { \
  typedef char type_check[__builtin_types_compatible_p(__typeof__(*(item_ptr)), type) ? 1 : -1]; \
  list_add_item((list), (item_ptr)); \
} while(0)

typedef struct{
  void *data;
  int items;
  size_t bytes;
  int capacity;
}List;

static inline void init_list(List *list, size_t bytes){
  list->items = 0;
  list->bytes = bytes;
  list->data = NULL;
  list->capacity = 1;
}     

static inline void list_add_item(List *list, void *new_item){
  if(list->items >= list->capacity){
    list->capacity *= 2;
    char *new = realloc(list->data, list->capacity * list->bytes);
    list->data = new;
  }
  char *address = (char *)list->data + (list->items * list->bytes);
  memcpy(address, new_item, list->bytes);
  list->items++;
}

static inline void list_remove(List *list, int index){
  if(index >= list->items|| index < -1){
    printf("invalid index %d for list_remove", index);
    return;
  }
  else if(index == -1){
    index = list->items - 1;
  }
  char *target = (char *)list->data + (index * list->bytes);
  while(index + 1 < list->items){
    char *next_item = target + list->bytes;
    memmove(next_item - list->bytes, next_item, list->bytes);
    index++;
    target += list->bytes;
  }
  list->items--; 
}

static inline void free_list(List *list){
  free(list->data);
}

#endif
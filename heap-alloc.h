#ifndef HEAP_ALLOC_H
#define HEAP_ALLOC_H
#include <sys/mman.h>
#include <stdio.h>
#include <stdbool.h>

#define HEAP_SIZE 4096
size_t *heap_ptr = NULL;
int allocations = 0;
typedef struct{
  int bytes;
  bool free;
  size_t *next_chunk;
}Chunk;

static inline size_t *heap_alloc(int bytes){
  bytes += 16;
  if(heap_ptr == NULL){
    heap_ptr = (size_t *)mmap(NULL, HEAP_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  }
  size_t *heap_end = (size_t *)((char *)heap_ptr + HEAP_SIZE);
  Chunk *chunk_ptr = (Chunk *)heap_ptr;

  for(int i = 0; i < allocations - 1; i++){
     if(chunk_ptr->free && ((Chunk *)chunk_ptr->next_chunk)->free){
      Chunk *next_chunk_ptr = (Chunk *)chunk_ptr->next_chunk;
      chunk_ptr->bytes += next_chunk_ptr->bytes;
      chunk_ptr->next_chunk = next_chunk_ptr->next_chunk;
      allocations--;
      i--;
      continue;
     }
     chunk_ptr = chunk_ptr->next_chunk;
  }

  chunk_ptr = (Chunk *)heap_ptr;

  if(allocations == 0){
    Chunk *chunk_0 = chunk_ptr;
    chunk_0->bytes = bytes;
    chunk_0->free = false;
    chunk_0->next_chunk = (size_t *)((char *)chunk_ptr + bytes);
    allocations = 1;
    return (size_t *)((char *)chunk_ptr + 16);
  }

  for(int i = 0; i < allocations; i++){
    if(chunk_ptr->free && chunk_ptr->bytes >= bytes){
      if(chunk_ptr->bytes - bytes > 0){
        int leftovers = chunk_ptr->bytes - bytes;
        chunk_ptr->bytes = bytes;
        chunk_ptr->free = false;
        chunk_ptr->next_chunk = (size_t *)((char *)chunk_ptr + bytes);

        Chunk *fragment_chunk = (Chunk *)chunk_ptr->next_chunk;
        fragment_chunk->bytes = leftovers;
        fragment_chunk->free = true;
        fragment_chunk->next_chunk = (size_t *)((char *)fragment_chunk + fragment_chunk->bytes);

        allocations++;
        return (size_t *)((char *)chunk_ptr + 16);
      }

      chunk_ptr->bytes = bytes;
      chunk_ptr->free = false;
      chunk_ptr->next_chunk = (size_t *)((char *)chunk_ptr + bytes);
      allocations++;
      return (size_t *)((char *)chunk_ptr + 16);
    }

    else if(i < allocations - 1){
      chunk_ptr = chunk_ptr->next_chunk;
    }
  }

  Chunk *new_chunk = (Chunk *)chunk_ptr->next_chunk;
  if((char *)new_chunk + bytes > heap_end){
    printf("4KB of heap memory used!");
    return NULL;
  }
  new_chunk->bytes = bytes;
  new_chunk->free = false;
  new_chunk->next_chunk = (size_t *)((char *)new_chunk + bytes);
  allocations++;
  return (size_t *)((char *)new_chunk + 16);
}

static inline void heap_free(size_t *ptr){
  if(ptr == NULL){
    return;
  }
  Chunk *chunk_ptr = (Chunk *)((char *)ptr - 16);
  chunk_ptr->free = true;
}

#endif
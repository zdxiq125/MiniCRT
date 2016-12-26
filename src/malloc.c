#include "minicrt.h"

typedef struct _heap_header {
  enum {
    HEAP_BLOCK_FREE = 0xABABABAB,
    HEAP_BLOCK_USED = 0xCDCDCDCD,
  } type;
  unsigned size;
  struct _heap_header* next;
  struct _heap_header* prev;
} heap_header;


#define ADDR_ADD(a,o) (((char*)(a)) + o)
#define HEADER_SIZE (sizeof(heap_header))

static heap_header* list_head = NULL;

void free(void* ptr) {
  heap_header* header = (heap_header*)ADDR_ADD(ptr, -HEADER_SIZE);
  if(header->type == HEAP_BLOCK_FREE)
    return;

  header->type = HEAP_BLOCK_FREE;
  // merge prev block
  if(header->prev && header->prev->type == HEAP_BLOCK_FREE) {
<<<<<<< HEAD
    // merge previous free block
=======
>>>>>>> origin/dev
    header->prev->next = header->next;
    header->prev->size += header->size;
    header->next->prev = header->prev;
    header = header->prev;
  }
  // merge next block
  if(header->next && header->next->type == HEAP_BLOCK_FREE) {
<<<<<<< HEAD
    // merge next free block
    header->next = header->next->next;
=======
>>>>>>> origin/dev
    header->size += header->next->size;
    header->next = header->next->next;
  }
}

void* malloc( unsigned size ) {
  void* ptr = NULL;
  heap_header* walker = list_head;
  while(walker) {
    if(walker->size >= size) {
      ptr = (void*)ADDR_ADD(walker, HEADER_SIZE);
      walker->type = HEAP_BLOCK_USED;
      // remain space management
      heap_header* next = walker->next;
      unsigned remain = walker->size - size;
      if(remain > HEADER_SIZE) {
        next = (*heap_header)ADDR_ADD(ptr, size);
        *next = heap_header()
      }
      walker->next = next;
      // end of management
      return ptr;
    }
    walker = walker->next;
  }
  return ptr;
}
<<<<<<< HEAD

void* malloc( unsigned size ) {
  void* ptr = NULL;
  if(size <= 0)
    return ptr;

  heap_header* walker = list_head;
  while(walker != NULL) {
    if(size + HEADER_SIZE > walker->size) {
      walker = walker->next;
      continue;                                                                                 j
    } else if(size + 2 * HEADER_SIZE >= walker->size) {
      // allocate whole block
      walker->type = HEAP_BLOCK_USED;
      return ADDR_ADD(walker, HEADER_SIZE);
    } else {
      // split the block
      heap_header* next = (heap_header*)ADDR_ADD(walker, HEADER_SIZE + size);
      next->next = walker->next;
      next->prev = walker;
      next->type = HEAP_BLOCK_FREE;
      next->size = walker->size - (size + HEADER_SIZE);
      walker->next = next;
      walker->type = HEAP_BLOCK_USED;
      walker->size = HEADER_SIZE + size;
      return ADDR_ADD(walker, HEADER_SIZE);
    }
    walker = walker->next;
  }

  return NULL;
}
=======
>>>>>>> origin/dev

#ifndef WIN32
static int brk(void* end_data_segment) {
  return 0;
}
#endif

#ifdef WIN32
#include <Windows.h>
#endif

int min_crt_heap_init() {
  return 1;
}

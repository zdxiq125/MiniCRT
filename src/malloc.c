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
  if(header->prev && header->prev->type == HEAP_BLOCK_FREE) {
    //if(header->next && head->next->type == HEAP_BLOCK_USED)
    header->prev->next = header->next;
    header->prev->size += header->size;
    header->next->prev = header->prev;
    header = header->prev;
  }

  if(header->next && header->next->type == HEAP_BLOCK_FREE) {
    header->next = header->next->next;
    header->size += header->next->size;
  }
}
void* malloc( unsigned size ) { return NULL; }

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

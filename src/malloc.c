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
  // merge previous free block
  if(header->prev && header->prev->type == HEAP_BLOCK_FREE) {
    header->prev->next = header->next;
    header->prev->size += header->size;
    header->next->prev = header->prev;
    header = header->prev;
  }
  // merge next free block
  if(header->next && header->next->type == HEAP_BLOCK_FREE) {
    header->next = header->next->next;
    header->size += header->next->size;
    header->next = header->next->next;
  }
}

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

#ifndef WIN32
static int brk(void* end_data_segment) {
  int ret;
  asm("movl $45, %%eax  \n\t"
      "movl %1, %%ebx   \n\t"
      "int 0x80         \n\t"
      "movl %%eax, %0   \n\t"
    : "=r"(ret): "m"(end_data_segment));
  return ret;
}
#endif

#ifdef WIN32
#include <Windows.h>
#endif

int min_crt_heap_init() {
  void* base = NULL;
  heap_header* header = NULL;
  // heap size = 32M
  int heap_size = 32 * 1024 * 1024;
#ifdef WIN32
  base = VirtualAlloc(0, heap_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
  if(base == NULL)
    return 1;
#else
  base = (void*)brk(0);
  void* end = ADDR_ADD(base, heap_size);
  end = (void*)brk(end);
  if(!end)
    return 1;
#endif
  header = (heap_header*)base;
  header->size = heap_size;
  header->type = HEAP_BLOCK_FREE;
  header->prev = header->next = NULL;

  list_head = header;
  return 0;
}

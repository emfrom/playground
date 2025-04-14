#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

size_t alignment = sizeof(void *);

void *xmalloc(size_t size) {
  void *return_value = malloc(size);

  if(NULL == return_value) {
    fprintf(stderr, "xmalloc() -> malloc() -> NULL\n");
    exit(EXIT_FAILURE);
  }

  return return_value;
}

void *xrealloc(void *pointer, size_t size) {
  void *return_value = realloc(pointer, size);
  
  if(NULL == return_value) {
    fprintf(stderr, "xrealloc() -> realloc() -> NULL\n");
    exit(EXIT_FAILURE);
  }
  return return_value;
}

void xfree(void *pointer) {
  free(pointer);
}

#ifdef USE_ALIGNED_MEMORY

//TODO: Finish aligned realloc (figure it out)
#error not finished

void *xrealloc(void *ptr, size_t new_size) {
  void *new_ptr;
  
    if (!ptr) {
        // Allocate new memory if ptr is NULL
        return aligned_alloc(alignment, new_size);
    }

    if (new_size == 0) {
        // Free the memory if new_size is 0
        free(ptr);
        return NULL;
    }

    new_ptr = realloc(ptr,new_size);
    if(new_ptr == NULL) {

    }
    
    if(((long)new_ptr % alignment) == 0) {
      return new_ptr;
    }
      

    new_ptr = aligned_alloc(alignment, new_size);
    if (!new_ptr) {
        return NULL;  // Allocation failed
    }

    // Copy data from old memory to new memory
    // (Make sure to not access beyond new_size or the size of the original allocation)
    memcpy(new_ptr, ptr, new_size > old_size ? old_size : new_size);

    // Free the old memory
    free(ptr);

    return new_ptr;
}
#endif

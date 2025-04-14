#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define TEST_SUFFIXARRAY

#if UINT64_MAX < SIZE_MAX
#error "size doesnt fit in uint64"
#endif

#if PTR_MAX > UINT64_T
#error "ptr bigger than u64"
#fi

#if 256 < UCHAR_MAX
#error "uchar size assumption failed"
#endif


//This is an initial naive sort

#include "xmalloc.c"

struct suffixarray_s {
  uint64_t length;
  uint64_t *memb;
};

typedef struct suffixarray_s suffixarray;

suffixarray suffixarray_create(char *data, size_t length) {

  //Create substrings
  unsigned char * temp_storage = xmalloc(sizeof(unsigned char) * length);
  for(uint64_t i = 0; i < length ; i++)
    temp_storage[i] = data + i;

  //Create suffixarray
  suffixarray sa = xmalloc(sizeof(struct suffixarray_s));
  
			   

  
  xfree(temp_storage);
  
}


#ifdef TEST_SUFFIXARRAY
int main(int arg_count, char **arg_vector) {
  return EXIT_FAILURE;
}
#endif

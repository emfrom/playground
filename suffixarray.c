#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define TEST_SUFFIXARRAY

#if UINT64_MAX < SIZE_MAX
#error "size doesnt fit in uint64"
#endif

#if PTR_MAX > UINT64_T
#error "ptr bigger than u64"
#endif

#if 256 < UCHAR_MAX
#error "uchar size assumption failed"
#endif


/**
   - This is an initial naive sort
   - Can only handle strings, not binary data
*/

#include "xmalloc.c"

struct suffixarray_s {
  uint64_t length;
  uint64_t *memb;
};

typedef struct suffixarray_s *suffixarray;

int string_compare(const void *pointer1, const void *pointer2) {
  char *string1 = *(char **) pointer1;
  char *string2 = *(char **) pointer2;

  return strcmp(string1, string2);
}

suffixarray suffixarray_create(char *data, size_t length) {

  //Create substrings
  char **temp_storage = xmalloc(sizeof(char *) * length);
  for(uint64_t i = 0; i < length ; i++)
    temp_storage[i] = data + i;

  //Sort substrings
  qsort(temp_storage, length, sizeof(char *), string_compare);
  
  //Create suffixarray
  suffixarray retval;
  retval = xmalloc(sizeof(struct suffixarray_s));
  retval->length = length;
  retval->memb = xmalloc(sizeof(uint64_t) * length);
  
  for(uint64_t i = 0; i < length ; i++)
    retval->memb[i] = (uint64_t) (temp_storage[i] - data);


  xfree(temp_storage);

  return retval; 
}

void suffixarray_destroy(suffixarray sa) {
  free(sa->memb);
  free(sa);
}



#ifdef TEST_SUFFIXARRAY
int main(int arg_count, char **arg_vector) {
  if(arg_count < 2) {
    printf("needs argument\n");
    return EXIT_SUCCESS;
  }


  char *string = (char *)arg_vector[1];
  int len = strlen(arg_vector[1]);

  suffixarray sa = suffixarray_create(string, len);

  for(int i = 0; i < sa->length ; i++)
    printf("%d: %s\n", i, string + sa->memb[i]);
				      

  suffixarray_destroy(sa);
  
  return EXIT_FAILURE;
}
#endif

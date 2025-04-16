#ifndef SUFFIXARRAY_C
#define SUFFIXARRAY_C
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef UNIT_TEST
#define SUFFIXARRAY_TEST
#undef UNIT_TEST
#endif 

#include "xmalloc.c"
#include "radixsort.c"

struct suffixarray_s {
  size_t length;
  uint8_t *data;
  size_t *indices;
};

typedef struct suffixarray_s *suffixarray;





suffixarray suffixarray_create(char *data, size_t length) {

  //Create suffixarray
  suffixarray sa;
  sa = xmalloc(sizeof(struct suffixarray_s));
  sa->length = length;
  sa->data = (uint8_t *)data;

  //Create substrings
  char **suffixes = xmalloc(sizeof(char *)*length);
  for(size_t i=0; i<length; i++)
    suffixes[i] = data + i;

  //Sort all the strings O(n)
  radixsort_asciz(suffixes, length, 0);

  //Store indices
  sa->indices = xmalloc(sizeof(size_t) * length);
  for(size_t i = 0; i < length; i++)
    sa->indices[i] = suffixes[i] - data;
    
  free(suffixes);
  
  return sa; 
}

void suffixarray_destroy(suffixarray sa) {
  free(sa->indices);
  free(sa);
}



#ifdef SUFFIXARRAY_TEST
int main(int arg_count, char **arg_vector) {
  if(arg_count < 2) {
    printf("needs argument\n");
    return EXIT_SUCCESS;
  }


  char *string = (char *)arg_vector[1];
  int len = strlen(arg_vector[1]);

  suffixarray sa = suffixarray_create(string, len);

  for(int i = 0; i < sa->length ; i++)
    printf("%lu: %s\n", sa->indices[i], string + sa->indices[i]);

  suffixarray_destroy(sa);
  
  return EXIT_FAILURE;
}
#endif
#endif /* SUFFIXARRAY_C */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define TEST_SUFFIXARRAY

#include "xmalloc.c"

struct suffixarray_s {
  int length;
  int *index;
};

typedef struct suffixarray_s *suffixarray;

suffixarray suffixarray_create(char *data, size_t length) {

  //Create suffixarray
  suffixarray retval;
  retval = xmalloc(sizeof(struct suffixarray_s));
  retval->length = length;
  retval->index = xmalloc(sizeof(int) * length);

  //Create substrings indices
  for(int i = 0; i < length ; i++)
    retval->index[i] = i;
  
  /**
   * Prefix sort O(n log n)
   *
   * Prefix sort exploints symmetries in the data sort faster
   *
   * Assumptions/definitions:
   *
   * - Let "rank" be value of char, same char has same rank. TODO: 0 and out of bounds ranks?
   * - 
   */

  // Initial ranking (with sort)

  // -- Loop over comparison lenghts (start 1)


  return retval; 
}

void suffixarray_destroy(suffixarray sa) {
  free(sa->index);
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
    printf("%d: %s\n", sa->index[i], string + sa->index[i]);

  suffixarray_destroy(sa);
  
  return EXIT_FAILURE;
}
#endif

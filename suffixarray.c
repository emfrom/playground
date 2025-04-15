#ifndef SUFFIXARRAY_C
#define SUFFIXARRAY_C

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define SUFFIXARRAY_TEST

#include "xmalloc.c"
#include "radixsort.c"

struct suffixarray_s {
  int length;
  int *index;
};

typedef struct suffixarray_s *suffixarray;



suffixarray suffixarray_create(char *data, size_t length) {

  //Create suffixarray
  suffixarray sa;
  sa = xmalloc(sizeof(struct suffixarray_s));
  sa->length = length;
  sa->index = xmalloc(sizeof(int) * length);

  //Create substrings indices
  for(int i = 0; i < length ; i++)
    sa->index[i] = i;
  
  /**
   * Prefix sort O(n log n), with radix or counting sort 
   *
   * Prefix sort exploints symmetries in the data sort faster
   *
   * Assumptions/definitions:
   *
   * - Let "rank" be value of char, same char has same rank. TODO: 0 and out of bounds ranks?
   * - 
   */

 
  
  //Sort index
  // qsort is used to reduce overall complexity (not sorting tuples)
  // the sort is done only on first char
  qsort_r(sa->index, length, sizeof(int), compare_first_char, data);
  
  // Initial ranking 
  int *ranks = xmalloc(sizeof(int) * length);
  int current_rank = 1; //Start at one, 0 is empty space
  ranks[0] = 1;
  for(int i = 1; i < length ; i++) {
    if(data[sa->index[i]] != data[sa->index[i - 1]])
       current_rank += 1;

    ranks[i] = current_rank;
  }

  //List is now sorted and ranked

  

  

  free(ranks);
  return sa; 
}

void suffixarray_destroy(suffixarray sa) {
  free(sa->index);
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
    printf("%d: %s\n", sa->index[i], string + sa->index[i]);

  suffixarray_destroy(sa);
  
  return EXIT_FAILURE;
}
#endif
#endif /* SUFFIXARRAY_C */

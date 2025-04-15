#ifndef SUFFIXARRAY_C
#define SUFFIXARRAY_C

/*
  TODO: Sorting by ranks do not work, needs differently written radix sort
  TODO: Generating ranks do not work, needs a new function
*/


#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define SUFFIXARRAY_TEST

#include "xmalloc.c"
#include "radixsort.c"

struct suffixarray_s {
  size_t length;
  uint8_t *data;
  uint64_t *indices;
};

typedef struct suffixarray_s *suffixarray;

//For the inital ranking of the suffixes
uint16_t get_initial(uint64_t index, void *base, void *userdata) {
  suffixarray sa = base;

  if(index + 1 == sa->length)
    return sa->data[index] << 8;

  return sa->data[index] << 8 | sa->data[index + 1];
}

//Data needed to generate sorting tuples (i.e. u16 from 2xu8)
struct rank_data_s {
  uint64_t *ranks;
  uint64_t prefix_length;
};

/*
  sa   = [0, 2, 4, 3, 5, 1]
  rank = [1, 3, 1, 2, 1, 2]

sa[i] suffix   rank[sa[i]]  rank[sa[i]+1]
 0   acabab       1         3    
 2     abab       1         2
 4       ab       1         2
 3      bab       2         1
 5        b       2         0
 1    cabab       3         1
 */
uint16_t get_rank_tuple(uint64_t i, void *base, void *userdata) {
  suffixarray sa = base;
  struct rank_data_s *rank_data = userdata;
  uint16_t tuple;

  //TODO: BUG: uint16 only works for up to 256 char lenght (max rank == length)
  tuple = (( rank_data->ranks[sa->indices[i]]) & 0x0F) << 8;
    
  uint64_t k = i + rank_data->prefix_length;

  if(k >= sa->length)
    return tuple;

  //TODO: BUG: uint16 only works for up to 256 char lenght (max rank == length)
  return tuple | (rank_data->ranks[sa->indices[i]] & 0x0F);
}


//Convert ordered indexes into ranks
uint64_t *rank_from_index(suffixarray sa, int *sorted) {
  uint64_t *ranks = xmalloc(sizeof(uint64_t)*sa->length);

  uint64_t current_rank = 1; //start at 1
  ranks[0] = 1;

  for(uint64_t i = 1; i < sa->length ; i++) {
    //TODO: BUG: This doesnt work (how did I come up with this?)
    if(sa->data[sa->indices[i]] != sa->data[sa->indices[i - 1]])
       current_rank += 1;

    ranks[i] = current_rank;
  }

  //All elements in the rank array have a unique rank
  // implies last rank == lenght
  *sorted = current_rank == sa->length ? 1 : 0;
    
  return ranks;
}

suffixarray suffixarray_create(char *data, size_t length) {

  //Create suffixarray
  suffixarray sa;
  sa = xmalloc(sizeof(struct suffixarray_s));
  sa->length = length;
  sa->data = (uint8_t *)data;
  
  /**
   * Prefix sort O(n log n), with radix or counting sort 
   *
   * Prefix sort exploints symmetries in the data sort faster
   *
   * Assumptions/definitions:
   */
  sa->indices = rsort_index(sa, length, get_initial, NULL);
  
  
  
  // Initial ranking 
     

  //List is now sorted and ranked

  

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

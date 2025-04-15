#ifndef RADIXSORT_C
#define RADIXSORT_C

/* I've botched this completely, DO NOT USE */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "xmalloc.c"

struct u64_bucket_s {
  size_t capacity;
  size_t nmembers;
  uint64_t *members;
};
typedef struct u64_bucket_s *u64_bucket;

typedef u64_bucket **bucketlist;

bucketlist bucketlist_add(bucketlist buckets, uint16_t bucket, uint64_t num) {
  int msb = bucket >> 8;
  int lsb = bucket & 0x00FF;
  
  if(NULL == buckets) {
    //First call, allocate MSB bucketlists
    buckets = xmalloc(sizeof(void *) * 256);

    for(int i=0; i < 256; i++)
      buckets[i] = NULL;
  }

  if(NULL == buckets[msb]) {
    //First entry for this MSB, allocate LSB bucketlist
    buckets[msb] = xmalloc(sizeof(void *) * 256);

    for(int i=0; i < 256; i++)
      buckets[msb][i] = NULL;
  }

  u64_bucket cur = buckets[msb][lsb];
    
  if(NULL == cur) {
    //First entry for this bucket
    buckets[msb][lsb] = xmalloc(sizeof(struct u64_bucket_s));
    
    cur = buckets[msb][lsb];
    cur->capacity = 0;
    cur->nmembers = 0;
    cur->members = NULL; 
  }

  if(cur->nmembers + 1 > cur->capacity) {
    //Need to increase size of bucket
    cur->capacity = cur->capacity ? cur->capacity * 4 : 4; 
    cur->members = xrealloc(cur->members, sizeof(uint64_t)*cur->capacity);
  }

  //Append this u64 number to bucket
  cur->members[cur->nmembers] = num;
  cur->nmembers += 1;
  
  return buckets;
}

void bucketlist_empty(bucketlist buckets, uint64_t *array) {
  uint64_t index = 0;
  
  for(int msb=0 ; msb < 256 ; msb++) {
    if(NULL == buckets[msb])
      continue;

    for(int lsb=0; lsb < 256; lsb++) {
      if(NULL == buckets[msb][lsb])
	continue;

      u64_bucket cur = buckets[msb][lsb];

      for(int i = 0; i < cur->nmembers; i++) {
	array[index] = cur->members[i];
	index += 1;
      }
      free(cur->members);
      free(cur); 
    }
    free(buckets[msb]);
  }
  
  free(buckets);
}


/**
 * Radix sort on uint16_t
 *
 * UINT16_MAX + 1 (2^16) number of buckets
 * Each bucket is an empty or expandable array
 * each element is represented by an index, plus a get function
 *
 * returns an array of indexes sorted
 */
uint64_t *rsort_index(void *base, size_t length,
			 uint16_t (*get_func)(uint64_t index, void *base, void *userdata),
			 void *userdata) {
  // Two tiers of buckets, one tier for MSB and one for LSB.
  //
  // MSB list is stored here, bucketlist buckets
  // MSB list is populated with LSB bucketlists when needed
  // LSB list is populated with 256 empty buckets when needed
  // Each LSB bucket is populated with a list of u64 indexes when needed 
  bucketlist buckets = NULL;

  for(uint64_t i=0; i < length; i++)
    buckets = bucketlist_add(buckets, get_func(i, base, userdata), i);

  //Retval
  uint64_t *indices = xmalloc(sizeof(uint64_t) * length);
  bucketlist_empty(buckets, indices);
  
  return indices;
}

#ifdef RADIXSORT_TEST

uint16_t get_value(uint64_t index, void *base, void *not_used) {
  char *string = ((char **)base)[index];

  if(string[0] == '\0')
    return 0;
  
  return string[0] << 8 | string[1]; 
}


int main(int argc, char **argv) {

  if(argc < 2)
    return EXIT_FAILURE;

  argc -= 1;

  uint64_t *indices = rsort_index(&argv[1], argc, get_value, NULL);

  for(int i=0; i < argc; i++)
    printf("%s\n", argv[indices[i] + 1]);

  free(indices);
}

#endif
#endif

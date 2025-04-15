#ifndef COUNTINGSORT_C
#define COUNTINGSORT_C

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "xmalloc.c"

/**
 * Counting sort, example
 *
 * Based on pseudo-code on the wikipedia page
 * - size_t is large enough to hold any array and count any members, TODO: prefix sums ?
 */
uint8_t *countingsort_example(uint8_t *array, size_t size) {

  //Store the counts
  // - 256 member array
  size_t counts[UINT8_MAX + 1] = { 0 };

  //Count pass
  for(size_t i=0; i < size; i++)
    counts[array[i]] += 1;

  //Prefix sum pass
  for(size_t i=1; i <= UINT8_MAX; i++)
    counts[i] += counts[i - 1];

  //Separate result
  uint8_t *result = xmalloc(size);
  
  //Insert pass
  for(size_t i=size;;) {
    i--;

    //Symbol 
    uint8_t j = array[i];

    //Decrement count
    counts[j] -= 1;

    result[counts[j]] = array[i]; 

    //When 0 is done, finished
    if(!i)
      break;
  }

  //Copy sorted array into original
  for(size_t i=0; i < size; i++) 
    array[i] = result[i];

  free(result);
  
  return array;
}


/**
 *  Unthreaaded counting sort
 *
 * Sorts data based on a u8 key
 */ 
void *countingsort_u8(void *data, size_t nmembers, size_t size,
		      uint8_t (*key_func)(void *element, void *userdata),
		      void *userdata) {
  //Store for counts
  size_t counts[UINT8_MAX + 1] = { 0 };

  //Keys
  // - Keys are cached
  uint8_t *keys = xmalloc(nmembers);
  
  //Counting pass 
  for (size_t i = 0; i < nmembers; i++) {
    void *element = (char *)data + i * size;
    keys[i] = key_func(element, userdata); 

    counts[keys[i]] += 1;
  }

  //Prefix sum pass
  for(size_t i=1; i <= UINT8_MAX; i++)
    counts[i] += counts[i - 1];

  //Result
  // not in place
  char *result = xmalloc(size*nmembers);

  //Insert pass
  // - unsigned safe decrement iteration
  for(size_t i = nmembers;;) {
    i--;

    //Readability
    uint8_t j = keys[i]; //Key value
    void *source = (char *)data + i * size;
    
    
    //Decrement count
    counts[j] -= 1;
    void *destination = result + counts[j]*size;

    //copy over
    memcpy(destination, source, size);

    //When 0 is done, finished
    if(!i)
      break;
  }

  //All done
  free(keys);

  return result;
}

#ifdef COUNTINGSORT_TEST

int main(int argc, char **argv) {

  if(2 > argc)
    return EXIT_FAILURE;

  //Sort argv[1] in place
  printf("%s: ", argv[1]);
  countingsort_example((uint8_t *) argv[1], strlen(argv[1]));
  printf("%s\n", argv[1]);

  return EXIT_SUCCESS;
}


#endif


#endif /* COUNTINGSORT_C */

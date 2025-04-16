#ifndef RADIXSORT_C
#define RADIXSORT_C

#ifdef UNIT_TEST
#define RADIXSORT_TEST
#undef UNIT_TEST
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "xmalloc.c"
#include "countingsort.c"

/**
 * General radix sort
 *
 * - lsb first
 * - any size works
 * - not threaded
 */

//Accessor function 
uint8_t radixsort_getplace(void *element, void *userdata) {
  uint8_t *value = element;
  uint8_t *digit_place = userdata;

  value += *digit_place;

  return *value;
}


void *radixsort(void *data, size_t nmembers, size_t size) {
  void *old = data;
  void *new;
  
  for(size_t digit_place = size;;) {
    digit_place -= 1;

    new = countingsort_u8(old, nmembers, size, radixsort_getplace, &digit_place);

    //Free unless it's original data
    if(old != data)
      free(old);

    old = new;
    
    if(!digit_place)
      break;
  }

  return new;
}

/**
 * String radix sort
 *
 * - sorts a prefix up to prefixlen
 */
struct string_s {
  char *text;
  size_t length;
};

//Accessor function
uint8_t radixsort_asciz_getchar(void *element, void *userdata) {
  struct string_s *string = element;
  size_t *position = userdata;

  if(string->length < *position)
    return '\0'; //Q: Should this be 0 and not '\0' ?

  return (uint8_t) string->text[*position];
}

void radixsort_asciz(char **ascizs, size_t nstrings, size_t prefixlen) {
  size_t max_length = 0;
  
  //Construct strings
  struct string_s *strings = malloc(nstrings * sizeof(struct string_s));
  for(size_t i = 0; i < nstrings; i++) {
    strings[i].text = ascizs[i];
    strings[i].length = strlen(ascizs[i]);

    if(max_length < strings[i].length)
      max_length = strings[i].length;
  }

  //Compare prefixlen or entire string
  if(!prefixlen)
    prefixlen = max_length;

  for(size_t i = prefixlen;;) {
    i -= 1;

    struct string_s *temp = countingsort_u8(strings, nstrings, sizeof(struct string_s), radixsort_asciz_getchar, &i);

    free(strings);
    strings = temp;
    
    if(!i)
      break;
  }

  //Save char * in original pointer array
  for(size_t i = 0; i < nstrings; i++)
    ascizs[i] = strings[i].text;

  //All done
  free(strings);
}

#ifdef RADIXSORT_TEST



int main(int argc, char **argv) {

  if(argc < 2)
    return EXIT_FAILURE;

  radixsort_asciz(argv, argc, 0);

  for(int i=0; i < argc; i++) 
    printf("%s\n", argv[i]);   
 
  return EXIT_SUCCESS;
}

#endif
#endif

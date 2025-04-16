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

  //Only works for little endian
  for(size_t digit_place = 0; digit_place < size ; digit_place++) {

    new = countingsort_u8(old, nmembers, size, radixsort_getplace, &digit_place);

    //Free unless it's original data
    if(old != data)
      free(old);

    old = new;
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

uint32_t convert(char *string) {
  uint32_t retval = 0;

  for(int i=0; i < 4; i++) {
    retval |= string[i] << (8*(3-i));

    if(!string[i])
      break;
  }

  return retval;  
}

#include "sorttest.c"

int main(int argc, char **argv) {

  //No args, use test data
  if(argc < 2) {
    size_t num_words;
    char **data = load_testdata(&num_words);

    radixsort_asciz(data, num_words,0);

    free(data);

    printf("All sorted");
  }
  else {
    //Just sort command line arguments
  argv++;
  argc--;
  
  uint32_t *ints = malloc(sizeof(uint32_t)*argc);
  for(int i=0;i<argc;i++)
    ints[i] = convert(argv[i]);

  //Sort numbersa
  uint32_t *sorted = radixsort(ints, argc, sizeof(uint32_t));

  //Sort text 
  radixsort_asciz(argv, argc, 0);



  for(int i=0; i < argc; i++) 
    printf("%8.8x : %s\n", sorted[i], argv[i]);
  
  
  free(ints);
  free(sorted);
  }
  
  return EXIT_SUCCESS;
}

#endif
#endif


#ifndef RADIXSORT_C
#define RADIXSORT_C

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "xmalloc.c"
#include "countingsort.c"

uint8_t radix_sort_getplace(void *element, void *userdata) {
  uint8_t *value = element;
  uint8_t *digit_place = userdata;

  value += *digit_place;

  return *value;
}

void *radix_sort(void *data, size_t nmembers, size_t size) {
  void *old = data;
  
  for(size_t digit_place = size;;) {
    digit_place -= 1;

    void *new = countingsort_u8(old, nmembers, size, radix_sort_getplace, &digit_place);

    //Free unless it's original data
    if(old != data)
      free(old);

    old = new;
    
    if(!digit_place)
      break;
  }

  //old points to latest sorted data here
  return old;
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

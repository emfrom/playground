#ifndef SORTTEST_C
#define SORTTEST_C

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#ifdef UNIT_TEST
#define SORTTEST_TEST
#undef UNIT_TEST
#endif

#include "xload.c"

char **load_testdata(size_t *num_strings) {
  size_t lines_loaded;

  //The collected works of shakespeare, one word per line.
  // Should sort in < 0.5s with qsort on a modern machine, but big enough n for asymptotic behavior
  // (go ahead, bubble sort it)
  char **lines = xload_lines("sorttest_data.txt", &lines_loaded);

  if(NULL != num_strings)
    *num_strings = lines_loaded;

  return lines;
}

#ifdef SORTTEST_TEST
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp_function(const void *a, const void *b) {
    const char *str_a = *(const char **)a;
    const char *str_b = *(const char **)b;
    
    return strcmp(str_a, str_b);
}


int main(int argc, char **argv) {
  size_t num_strings;
  char **to_sort = load_testdata(&num_strings);


  qsort(to_sort, num_strings, sizeof(char *), cmp_function);

  if(argc != 1) 
    for(int i=0; i < 25; i++)
      printf("%d: %s\n", i, to_sort[i]);

}

#endif

#endif /* SORTTEST_C */

#ifndef XLOAD_C
#define XLOAD_C

#ifdef UNIT_TEST
#define XLOAD_TEST
#undef UNIT_TEST
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "xmalloc.c"

char *xload_pipe(FILE *fp, size_t *size) {
  size_t capacity = 4096;
  size_t total_read = 0;
  char *buffer = xmalloc(capacity);

  while (1) {
    size_t bytes_read =
        fread(buffer + total_read, 1, capacity - total_read, fp);
    total_read += bytes_read;

    if (bytes_read < capacity - total_read) {
      if (feof(fp)) {
        break;
      } else if (ferror(stdin)) {
        fprintf(stderr, "xread() -> ferror() -> true\n");
        exit(EXIT_FAILURE);
      }
    }

    // Check if we need more space
    if (total_read == capacity) {
      capacity += 512;
      buffer = xrealloc(buffer, capacity);
    }
  }

  buffer = xrealloc(buffer, total_read + 1);
  buffer[total_read] = '\0';

  if (NULL != size)
    *size = total_read;
  
  return buffer;
}

char *xload_file(const char *filename, size_t *size) {
  FILE *f = fopen(filename, "rb");
  if (!f) {
    fprintf(stderr,"xload_file()->fopen");
    exit(EXIT_FAILURE);
  }

  if (fseek(f, 0, SEEK_END) != 0) {
    fprintf(stderr,"xload_file()->fseek");
    exit(EXIT_FAILURE);
  }

  //Get size, long implies max size 2^63
  long file_size = ftell(f);
  if (file_size < 0) {
    fprintf(stderr,"xload_file()->ftell");
    exit(EXIT_FAILURE);
  }

  //Prepare to load
  fseek(f, 0, SEEK_SET);
  char *buf = xmalloc(file_size + 1);

  //Load file
  if (fread(buf, 1, file_size, f) != (size_t)file_size) {
    fprintf(stderr,"xload_file()->fread");
    exit(EXIT_FAILURE);
  }

  fclose(f);
  
  //Terminate loaded data 
  buf[file_size] = '\0';

  
  if(NULL != size)
    *size = file_size;
  
  return buf;
}


/**
 * Loads an entire file and splits it into lines
 *
 * - Lines are stores as asciiz with pointers in a large array
 * - Pointers and actual data occupy the same memory so that the entire thing can be free()ed at the end
 * - Can load from file, or stdin (for convenience)
 */
char **xload_lines(const char *filename, size_t *number_lines) {

    char *file_content;
    size_t filesize;

    if(NULL != filename) {
      file_content = xload_file(filename, &filesize);
    }
    else {
      file_content = xload_pipe(stdin, &filesize);
    }

    //Lines information 
    char **lines = NULL;
    size_t num_lines = 0;
    size_t size = 0;

    //Set pointers to start and end of first line
    char *current_line = file_content;
    char *end_of_line = strchr(current_line, '\n');

    //Check if there are lines at all
    if (NULL == end_of_line) {
      // Entire file is one line
      int line_length = strlen(current_line);
      if (line_length) {
        end_of_line = current_line + line_length;
      }

      //See if there is anything at all
      if(end_of_line != current_line)
	num_lines = 1;
    }

    //Parse lines in file 
    while (NULL != end_of_line) {
      *end_of_line = '\0';
      num_lines++;

      if (size < (num_lines * sizeof(char *))) {
        size += 4096;
        lines = xrealloc(lines, size);
      }
      lines[num_lines - 1] = current_line;

      current_line = end_of_line + 1;
      if (current_line > (file_content + filesize))
        break;

      end_of_line = strchr(current_line, '\n');
      if (NULL == end_of_line) {
        // One last line?
        int line_length = strlen(current_line);
        if (line_length) {
          end_of_line = current_line + line_length;
        }
      }
    }

    
    if(0 == num_lines) {
      fprintf(stderr, "xload_lines() -> empty file\n");
      exit(EXIT_FAILURE);
    }

    
    //Terminate lines array
    size = (num_lines + 1)*sizeof(char *);
    lines[num_lines] = NULL;

    
    //One memory location to rule them all
    //TODO: Move pointers to front of filedata instead of copying the entire file to new location 
    size_t total_size = filesize + size;
    lines = xrealloc(lines, total_size);

    char *file_content2 = (char *) &(lines[num_lines + 1]);
    memcpy(file_content2, file_content, filesize);
    xfree(file_content);

    //Adjust pointers
    off_t adjustment = file_content2 - file_content;

    for(size_t i=0; i < num_lines; i++)
      lines[i] = lines[i] + adjustment;

    if(NULL != number_lines)
      *number_lines = num_lines;
    
    return lines; 
}



#ifdef XLOAD_TEST
int main(int argc, char **argv) {
  char *filename = "xload.c";

  if(argc > 1)
    filename = argv[1];
  
  //Load
  size_t num_lines;
  char **lines = xload_lines(filename, &num_lines);

  for(unsigned int i = 0; i < num_lines; i++) {
    printf("%d: %s\n", i, lines[i]);
  }

  free(lines);
  return EXIT_SUCCESS; 
}
#endif

#endif /* XLOAD_C */


#ifndef TRIEAZ_C
#define TRIEAZ_C

//Naive trie implementation using only a-z 

#ifdef UNIT_TEST
#define TRIEAZ_TEST
#undef UNIT_TEST
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct trieaz_s *trieaz;

struct trieaz_s {
  char character;
  char nchildren;
  trieaz children; 
};

// Use a root node marker
// Since this is "supposed to be" space efficient,
//  edges cannot store the char values
const char TRIE_ROOT = 0xFF;

//
const char CHILDLIST_MAX = 'z' - 'a';
const char CHILDLIST_SIZE = CHILDLIST_MAX + 1;
#define CHILDLIST_INDEX(c) ((c) - 'a')

// Will return position c is at
// OR position it should be at
//
// TODO: I'm not sure all the opts are any use for a 26 member alphabet
//
// This is one of those cases where something is O(n) in a very counter
// intuitive way. The bsearch is log n but on the alphabet so it's limited to 5
// which makes the overall algorithm O(n). Even for a 256 alphabet it would
// still be O(n) with n being the length of the word.
char trieaz_helper_getpos(trieaz node, char c) {

  // No children
  // TODO: Check with other logic if this is really necessary
  if (!node->nchildren)
    return 0;

  //Full list 
  if (node->nchildren == CHILDLIST_SIZE)
    return CHILDLIST_INDEX(c);

  // Check if it's first
  if (node->children[0].character > c)
    return 0;

  // Check if it's last
  if( node->children[node->nchildren - 1].character < c)
    return node->nchildren;

  // Search
  
  // No character is every stored at an index higher than it's index in a full array
  char high=CHILDLIST_INDEX(c);

  // No character is ever stored lower than it's index in a full array - (26 - nchildren) (or 0)
  char low = CHILDLIST_INDEX(c) - ( CHILDLIST_SIZE - node->nchildren);
  if(low < 0)
    low = 0;

  //Linear search faster for small arrays
  if(4 > (high - low)) {
    for(char i = low; i < high; i++)
      if(node->children[i].character < c)
	return i;

    return high;
  }

  //Ordinary bsearch 
  while (low < high) {
    char mid = (low + high) >> 1;

    if (node->children[mid].character < c)
      low = mid + 1;
    else
      high = mid;
  }
  
  return low;
}

// Takes a node and a char and returns the next node or char
// Adding if needed
trieaz trieaz_add_node(trieaz node, char c) {
  
}


trieaz trieaz_add(trieaz root, char *word) {
  

  
  return NULL;
}


#ifdef TRIEAZ_TEST

int main() {

  return EXIT_FAILURE;
}

#endif


#endif //TRIEAZ_C

#ifndef TRIEAZ_C
#define TRIEAZ_C

// Naive trie implementation using only a-z

// Realised that the naive implementation is braindead
// Wastes too much time and doesnt really save all that much space


// New design
//
// No human language using latin alphabet (i.e. <32 tokens) will ever have
// anywhere near 2^32 words.
//
// The following info is needed
// 1 bit - Is a node a word termination
// 5 bits - how many children does node have
// 26 bits - which letter children are present
// i.e. <32 bits
//
// This is the first 32 bit word in an array of 1 + nchildren 32 bit words
// Where each of the words are 32-bit pointers to other children
//
// Needs: 32-bit memarena for allocations
//
// Space is ~30-40% of naive implementation
// Speed is 2x naive (minimum)
//
// Also opens up the possibilty of having 8bit alphabets without to much
// overhead.
// Can look at things liek how a code generator "talks",
// what kinds of words it uses. A word being anything said up until a branch/jmp
// instruction.
//
// Fun stuff



#include <stddef.h>
#include <string.h>
#ifdef UNIT_TEST
#define TRIEAZ_TEST
#undef UNIT_TEST
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xmalloc.c"


typedef struct trieaz_s *trieaz;
struct trieaz_s {
  char character;
  char nchildren;
  char is_word; //A word ends on this node
  trieaz children; 
};

// Use a root node marker
// Since this is "supposed to be" space efficient,
//  edges cannot store the char values
const char TRIE_ROOT = 0x00;

//
const char CHILDLIST_MAX = 'z' - 'a';
const char CHILDLIST_SIZE = CHILDLIST_MAX + 1;
#define CHILDLIST_INDEX(c) ((c) - 'a')

// Make a new trie root
trieaz trieaz_create() {
  trieaz new = xmalloc(sizeof(struct trieaz_s));

  new->character = TRIE_ROOT;
  new->nchildren = 0;
  new->children = NULL;
  new->is_word = 0;

  return new;
}

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

  //
  // Search
  //
  
  // No character is ever stored at an index higher than it's index in a full array
  char high=CHILDLIST_INDEX(c);

  // No character is ever stored lower than it's index in a full array - free spaces in the array
  // max(index - ( 26 - nchildren), 0 ) 
  char low = CHILDLIST_INDEX(c) - ( CHILDLIST_SIZE - node->nchildren);
  if(low < 0)
    low = 0;

  //Ordinary bsearch, 5 steps ceiling
  while (low < high) {
    char mid = (low + high) >> 1;

    if (node->children[mid].character < c)
      low = mid + 1;
    else
      high = mid;
  }
  
  return low;
}

// Takes a node and a char and returns the corresponding node 
// Adding if needed
// Returns NULL if c == NUL
trieaz trieaz_add_node(trieaz node, char c) {
  if('\0' == c) {
    node->is_word = 1;
    return NULL;
  }
  
  char array_position = trieaz_helper_getpos(node, c);

  //Check if we found c
  if(array_position < node->nchildren && node->children[array_position].character == c)
    return node->children + array_position;

  
  //Grow array 
  node->children = xrealloc(node->children, sizeof(struct trieaz_s) * (node->nchildren + 1));

  // Array move needed ?
  if(array_position < node->nchildren)
    memmove(node->children + array_position + 1,
	    node->children + array_position,
	    (node->nchildren - array_position)*sizeof(struct trieaz_s));

  //Assign values   
  node->children[array_position].character = c;
  node->children[array_position].nchildren = 0;
  node->children[array_position].children = NULL;
  node->is_word = 0;

  node->nchildren += 1;
  
  return node->children + array_position;

}

// Takes a root node and add's a word
//
// Returns the root
trieaz trieaz_insert(trieaz root, char *word) {
  //New trie?
  if(NULL == root)
    root = trieaz_create();

  //Actually something to add?
  if('\0' == *word)
    return root;
  
  trieaz temp = root;

  while(NULL != (temp = trieaz_add_node(temp, *word)))
    word++;

  return root;
}

void trieaz_destroy(trieaz node) {
  if(node->nchildren == 0)
    return;
  
  for(int i=0; i < node->nchildren; i++)
    trieaz_destroy(node->children + i);

  free(node->children);

  return;

}

trieaz trieaz_remove(trieaz node, char *word) {


}


#ifdef TRIEAZ_TEST

int main() {

  return EXIT_FAILURE;
}

#endif


#endif //TRIEAZ_C

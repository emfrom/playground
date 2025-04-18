#ifndef TRIEAZ_C
#define TRIEAZ_C

#ifdef UNIT_TEST
#define TRIEAZ_TEST
#undef UNIT_TEST
#endif

typedef struct trieaz_s *trieaz;

struct trieaz_s {
  char character;
  char nchildren;
  trieaz *children; //struct trieaz ** !!  
};

trieaz trieaz 

//Use a root node to
const char TRIE_ROOT = 0xFF;

#ifdef TRIEAZ_TEST

int main() {

}

#endif


#endif //TRIEAZ_C

//Stefon Miller
//Cs449 recitation 11579
//Mymalloc.h

//Struct block given in lab
struct Block
{
  int occ;              // whether block is occupied
  int size;             // size of block (including header)
  struct Block *prev;   // pointer to previous block
  struct Block *next;   // pointer to next block
};

//Allocates size bytes and returns a pointer to the allocated space
void *my_malloc(int size);

//Frees memory pointed to by pointer and coallesces when necessary
void my_free(void *pointer);

//Dump heap function given in lab
void dump_heap();

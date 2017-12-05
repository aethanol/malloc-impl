/* Sang Ouk Kim 
 * Ethan Anderon
 * cse374 - hw6 - 05/18/17
 *
 * mem_impl.h is a header file that defines internal implementation details for a memory management
 * program built for a cse374 homework assignment. Includes free list data structures and check_heap
 */

#ifndef MEM_IMPL_H
#define MEM_IMPL_H

#include <stdio.h>
#include <inttypes.h>
// define a blocksize for calculating the offset in the freelist
#define BLOCK_SIZE sizeof(Block)
#define SPLIT_THRESHOLD 32

// define freelist structure 
typedef struct block Block; 
struct block {
    // amount of data free in this block
    uintptr_t size; // size_t is an unsinged type returned by sizeof()
    Block *next; // pointer to the next block in the free list
};

// define global head variable for the freelist
extern Block *head;
extern uintptr_t total_mem;
// check_heap checks for possible problems with the free list data structure.
void check_heap();
void _check_blocks(Block *prev, Block *curr);
void _check_size(Block *block);
Block* _new_block(uintptr_t size);
uintptr_t _size_align(uintptr_t size);
uintptr_t get_total_mem();

#endif

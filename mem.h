/* Sang Ouk Kim 
 * Ethan Anderon
 * cse374 - hw6 - 05/18/17
 *
 * mem.h is a header file defines an interface for a memory management program implemented 
 * by getmem and freemem functionality.
 */

#ifndef MEM_H
#define MEM_H

#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>

// getmem returns a pointer to a new block of storage with at least size bytes of memory.
// if size is negative or if getmem cannot satisfy the request it returns NULL.
void* getmem(uintptr_t size);

// freemem returns a block of storage at location p to the pool of available free storage.
// if p is NULL freemem returns immediately with no effect.
void freemem(void* p);

// get_mem_stats stores statistics about the current state of the memory manager
//  in three integer variables.
// total_size is the toal amount of storage in bytes requested from the system
// total_free is the total amount of storage in bytes that is currently stored 
// on the free list (including header info or links between free blocks)
// n_free_blocks is the total number of individual blocks current stored on the free list.
void get_mem_stats(uintptr_t* total_size, uintptr_t* total_free, uintptr_t* n_free_blocks);

// print_heap prints a formatted listing on file f showing the address and length of each 
// block on the free list on a single output line.
void print_heap(FILE* f);

#endif

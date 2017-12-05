/* Sang Ouk Kim 
 * Ethan Anderon
 * cse374 - hw6 - 05/18/17
 *
 * get_mem_stats.c is an implementation of a memory statistics storage function for
 * a memory management program.
 */

#include "mem.h"
#include "mem_impl.h"

int get_total_free_blocks();
// get_mem_stats stores statistics about the current state of the memory manager
//  in three integer variables.
// total_size is the toal amount of storage in bytes requested from the system
// total_free is the total amount of storage in bytes that is currently stored 
// on the free list (including header info or links between free blocks)
// n_free_blocks is the total number of individual blocks current stored on the free list.
void get_mem_stats(uintptr_t* total_size, uintptr_t* total_free, uintptr_t* n_free_blocks) {
    uintptr_t size = get_total_mem();
    //printf("%lu\n", size);
    uintptr_t count_size = 0;
    uintptr_t count_free = 0;
    *total_size = size;
    Block *curr = head;
    while (curr != NULL) {
        count_size += curr->size + BLOCK_SIZE;
        count_free++;
        curr = curr->next;
    }
    
    *total_free = count_size;
    *n_free_blocks = count_free;
}

/* Sang Ouk Kim 
 * Ethan Anderon
 * cse374 - hw6 - 05/18/17
 *
 * mem_utils.c is an implementation of a heap checking function for a
 * memory management program.
 */

#include "mem.h"
#include "mem_impl.h"

#include <stdlib.h>
#include <assert.h>

// initalize the head to NULL
Block *head = NULL;

// check_heap checks for possible problems with the free list data structure.
void check_heap() {
    // return and pass if the head is null
    if (head == NULL){
        return;
    }
    // check the size of the head (fencepost problem)
    //_check_size(head);
    //printf("CHECK head->size at: %" PRIuPTR "\n", (uintptr_t)head->size);
    assert(head->size >= SPLIT_THRESHOLD);
    Block *curr = head;
    while (curr->next != NULL) {
        //printf("CHECK curr at: %" PRIuPTR "\n", (uintptr_t)curr);
        //printf("CHECK curr->next at: %" PRIuPTR "\n", (uintptr_t)curr->next);
        
        //printf("CHECK curr->size: %" PRIuPTR "\n", curr->size);
        assert(curr->next->size >= SPLIT_THRESHOLD && "check that blocks are at least SPLIT_THRESHOLD");
        assert((uintptr_t)curr < (uintptr_t)curr->next && "check that block addresses are strictly increasing");
        
        // make sure that the blocks do not overlap
        assert((uintptr_t)curr + curr->size + BLOCK_SIZE < (uintptr_t)curr->next && "check that blocks don't overlap");
        //assert((intptr_t)curr->next - ((intptr_t)curr + curr->size + BLOCK_SIZE) > 32 && "check that blocks are adaquately far apart ");
        assert(((uintptr_t)curr + BLOCK_SIZE + curr->size) != (uintptr_t)curr->next && "check that blocks are not touching");
        // go to the next block in the freelist
        curr = curr->next;
    }
}

// _check_size asserts that the block size is at least as large as the threshold
void _check_size(Block *block) {
    printf("%" PRIuPTR "\n", block->size);
    assert(block->size >= SPLIT_THRESHOLD);
}

// _check_blocks asserts that two blocks are on the right boundary
void _check_blocks(Block *prev, Block *curr) {
    // check that blocks are ordered strictly increasing memory addresses
    assert((uintptr_t)prev < (uintptr_t)curr);
    //printf("prev: %d, curr: %d\n", &prev, &curr);
    // check the size of both blocks
    _check_size(prev);
    _check_size(curr);
}

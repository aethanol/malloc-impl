/* Sang Ouk Kim 
 * Ethan Anderon
 * cse374 - hw6 - 05/18/17
 *
 * freemem.c is the implementation of a memory freeing function 
 * for a memory managemnt 
 * program built as a cse374 assignment.
 */
#include "mem.h"
#include "mem_impl.h"
#include <assert.h>

Block* findBlock(Block* p);

// freemem returns a block of storage at location p to the
// pool of available free storage. If p is NULL freemem returns
// immediately with no effect.
void freemem(void* p) {
// check the heap for any errors
    check_heap();
    //print_heap(stdout);
    // if user gave us a null pointer do nothing (stupid user)
    if (p == NULL) {
        return;
    }

    uintptr_t pAd = (uintptr_t)p - BLOCK_SIZE;
    // turn the pointer back into a block
    Block* free_block = (Block*)pAd;
    // find the previous block
    Block *prev_block = findBlock(free_block); // null if no previous (or if there is no head)
    //assert((uintptr_t)prev_block < (uintptr_t)free_block && "check that the prev block is before free block");
    // case where there is no prev (needs to go before head)
    if ((uintptr_t)free_block < (uintptr_t)head) {
        // check if needs to be merged with head
        if ((uintptr_t)free_block + BLOCK_SIZE + free_block->size == (uintptr_t)head && head != NULL) {
            // link the next to where head next was pointing
            //if (head == NULL) {
            //head = free_block;
            // }else {
            uintptr_t prev_size = free_block->size;
            free_block->next = head->next;  // this could be NULL
            // add the size of the head block + header size
            free_block->size += BLOCK_SIZE + head->size;
            assert(free_block->size == prev_size + head->size + BLOCK_SIZE);
            // zero out the head to be safe
            head->next = NULL;
            head->size = 0;
            // set the head to be the block we are freeing
            head = free_block;
            //}
            
     
        } else { // otherwise just link up before the head (which could be null)
            free_block->next = head;
            head = free_block;
        }
        // if we aren't makeing free_block the head
    } else {
        // save the next block so we can merge with 
        Block* next = prev_block->next;
        
        // check PREVIOUS block to see if it needs to be merged or just linked
        // check if the end of the previous block is our free block
        if ((uintptr_t)prev_block + BLOCK_SIZE + prev_block->size == (uintptr_t)free_block) {
            // set the size and we don't need to set the pointer because it still points to next
            prev_block->size += BLOCK_SIZE + free_block->size;
            // null out to be safe
            free_block->next = NULL;
            free_block->size = 0;
            // MAKE SURE TO SET free_block to now point to prev which is now the merged block 
            free_block = prev_block;
          } else { // otherwise we just link up prev to free_block
            prev_block->next = free_block;
            //assert( (uintptr_t)free_block - ((uintptr_t)prev_block + BLOCK_SIZE + prev_block->size) > SPLIT_THRESHOLD );
        }
        
        // check NEXT block to see if it needs to be merged or just linked
        if ((uintptr_t)free_block + BLOCK_SIZE + free_block->size == (uintptr_t)next) {
            free_block->size += BLOCK_SIZE + next->size;
            free_block->next = next->next; // next->next could be NULL
            // null out to be safe
            next->next = NULL;
            next->size = 0;
        } else {
            free_block->next = next;
        }
    }
    check_heap();
}

// findBlock returns a pointer to the block that is previous to the passed in
// pointer. If p is front of the freelist, returns null
Block* findBlock(Block* p) {
    //if the block is before head or if head == NULL
    if ( (uintptr_t)head >= ((uintptr_t)p) || head == NULL) {
        return NULL;  // returns null if p is front of the freelist
    }
    
    Block* curr_node = head;
    while (curr_node->next != NULL && (uintptr_t)p > (uintptr_t)curr_node->next) {
        curr_node = curr_node->next;
    }    return curr_node;
}

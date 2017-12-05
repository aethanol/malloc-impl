/* Sang Ouk Kim 
 * Ethan Anderon
 * cse374 - hw6 - 05/18/17
 *
 * getmem.c is the implementation of getmem for a memory management program built for
 * a cse374 assignment.
 */

#include "mem_impl.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#define OVERHEAD 6080

Block* _new_block(uintptr_t size);
uintptr_t _size_align(uintptr_t size);
uintptr_t _size_align_neg(uintptr_t size);
uintptr_t _align_closest(uintptr_t size);
void _add_to_list(Block *new_block, Block *end);
void* _find_block(uintptr_t size);
Block* _check_block(Block *prev, Block *curr, uintptr_t size);
Block* _split(Block *new_block, uintptr_t size);

// store the total amount of memory requested from malloc
uintptr_t total_mem = 0;

// get_total_mem returns the total memory that has been allocated
// from the uderlying system
uintptr_t get_total_mem() {
    return total_mem;
}

// getmem returns a pointer to a new block of storage with at least size bytes of memory.
// if size is negative or if getmem cannot satisfy the request it returns NULL.
void* getmem(uintptr_t size) {
    // align the size so all future work will be aligned
    size = _size_align(size);
    // check if the head of the freelist is NULL and allocate a new node if so
    if (head == NULL) {
        head = _new_block(size);
    }

    check_heap();

    // go through the freelist and get a pointer to a block of size+x so %16 =0
    void *b = _find_block(size);
    check_heap();
    
    // assert that the pointer we're returning is a multiple of 16
    assert (((uintptr_t)b % 16) == 0);
    return b;
}

// _get_block returns a pointer to a block 
void* _find_block(uintptr_t size) {
    // make sure to align the size to 16 bytes
    size = _size_align(size);
    if (size < SPLIT_THRESHOLD) {
        size = SPLIT_THRESHOLD;
    }
    Block *prev = NULL;
    Block *temp = head;
    // check if the head block can be given to the user
    Block *return_block = _check_block(NULL, head, size);
    
    if (return_block != NULL) {
         // return to the user after the header
        return (void*)return_block + BLOCK_SIZE;
    }
    // find a valid block from the freelist
    while (temp->next != NULL) {
        // go to the next block first
        prev = temp;
        temp = temp->next;
        // check if we can return this block
        return_block = _check_block( prev, temp, size);
        //check_heap();
        if (return_block != NULL) {
            // unlink the block from the freelist
            //prev->next = return_block->next;
            // return to the user after the header
            return (void*)return_block + BLOCK_SIZE;
        }
    }

    assert(temp->next == NULL && "check that the program didn't find a block");
    // if we got to the end of the list without finding a block
    // get a new block from malloc and put it to the
    // front or end of the list
    Block *new_block = _new_block(size);
    _add_to_list(new_block, temp);
    temp->next = new_block;
    // then find a block from the freelist to give to the user
    return _find_block(size);
}

Block* _check_block(Block *prev, Block *curr, uintptr_t size) {
// check if the current block can be given to the user
    if (curr->size > size + BLOCK_SIZE) {
        // check that if we split the block it won't leave a block < THRESHOLD 
        // case where we DO want to split the block
        if ( ((intptr_t)curr->size - ((intptr_t)size + (intptr_t)BLOCK_SIZE) ) > SPLIT_THRESHOLD) { 
            return _split(curr, size);
        } else { // we should just return the block to the user
            // unlink the block if it's not the head
            if (curr != head) {
                prev->next = curr->next;
            } else {
                head = curr->next;
            }
            return curr;
        }
    } else {
        return NULL;
    }
}

// _split splits a block in two, (size and left-over) and returns a pointer to the user
Block* _split(Block *block, uintptr_t size) {
    // create a new block from the end of the block
    uintptr_t end = ((uintptr_t)block + BLOCK_SIZE + (uintptr_t)block->size);
    uintptr_t newAd = end - ((uintptr_t)size + BLOCK_SIZE);
    assert( newAd % 16 == 0 );
    // set the size of the block and the new block
    Block *newBlock = (Block*)newAd;
    // calculate the new block size
    // which is new address 
    //newBlock->size = end - newAd - BLOCK_SIZE;
    newBlock->size = size;
    newBlock->next = NULL;
    assert( newBlock->size % 16 == 0);
    
    // set the size of the old block
    block->size = (block->size - newBlock->size - BLOCK_SIZE);
    assert(block->size > SPLIT_THRESHOLD);
    
    assert((uintptr_t)block + block->size + BLOCK_SIZE == (uintptr_t)newBlock && "check that when we split the blocks are right next to each other");
    assert( block->size % 16 == 0);
    // return a pointer to the new block
    return newBlock;
}

// _add_to_list adds malloced memory to the front or end of the list
void _add_to_list(Block *new_block, Block *end) {
    assert(end->next == NULL);
    // check if the block address
    // should go at the front or end of the list
    // check incredibally rare case where merging needs to happen
    if ((uintptr_t)new_block < (uintptr_t)head) {
        new_block->next = head;
        head = new_block;
    } else {
        end->next = new_block;
    }
}

// _new_block gets a block from the system
// will always get an overhead amount
Block* _new_block(uintptr_t size) {
    // get a block from the system and check error
    Block *retBlock = (Block*)malloc(size + BLOCK_SIZE + OVERHEAD);
    if (retBlock == NULL) {
        exit(0);
    }

    // increment the total_mem of the system
    total_mem += size + BLOCK_SIZE + OVERHEAD;

    retBlock->size = size + OVERHEAD;
    retBlock->next = NULL;
    return retBlock;
}

uintptr_t _size_align_neg(uintptr_t size) {
    if (size % 16)
        size = size - size % 16;
    return size;
}

uintptr_t _size_align(uintptr_t size) {
    if (size % 16) {
        size = size + (16 - size % 16);
    }
    return size;
}

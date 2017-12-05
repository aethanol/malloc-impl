/* Sang Ouk Kim 
 * Ethan Anderon
 * cse374 - hw6 - 05/18/17
 *
 * print_heap.c is the implementation of a fucntion that prints a formatted listing 
 * of the free list memory blocks for a memory managemnt program built for a cse374
 * homework assignment.
 */

#include "mem_impl.h"
#include <inttypes.h>
// print_heap prints a formatted listing on file f showing the address and length of each 
// block on the free list on a single output line.
void print_heap(FILE * f) {
    
    fprintf(f, "=== PRINTING HEAP ===\n\n");
    
    Block *curr = head;
    while (curr != NULL) {
        // fprint to the file and memory 
        fprintf(f,"address: 0x%" PRIuPTR " length: 0x%" PRIuPTR " address end: 0x%" PRIuPTR "\n", (uintptr_t)curr, curr->size + BLOCK_SIZE, (uintptr_t)curr + curr->size + BLOCK_SIZE);
        curr = curr->next;
    }
}

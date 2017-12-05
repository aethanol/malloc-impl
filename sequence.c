// this is a testfile


#include "mem.h"
#include "mem_impl.h"
#include <time.h>


void print_stats(uintptr_t* total_size, uintptr_t* total_free, uintptr_t* n_free_blocks, clock_t* start){
    // get the current time
    clock_t curr = clock() - *start;
    double time_taken = ((double)curr)/ CLOCKS_PER_SEC;
    printf("Total CPU time so far: %f seconds\n", time_taken);
    get_mem_stats(total_size, total_free, n_free_blocks);
    printf("Total number of bytes acquired so far: %" PRIuPTR "\n", *total_size);
    printf("Total number of bytes on freelist: %" PRIuPTR "\n", *total_free);
    printf("Total number of free blocks: %" PRIuPTR "\n", *n_free_blocks);
    printf("Average number of bytes per block in freelist: %f\n\n", (double)*total_free / (double)*n_free_blocks);
}


int main (int argc, char *argv[]) {
    // start the clock
    clock_t start = clock();
    //int interval = (int)((((double)ntrials/10)) + 0.5);
    uintptr_t total_size = 0;
    uintptr_t total_free = 0;
    uintptr_t n_free_blocks = 0;
    
// call a sequence of known memory calls
    void *allocated[5];
    printf("SEQ: allocating\n");
    for (int i = 0; i < 5; i++) {
        allocated[i] = getmem(50);
    }
    for (int i = 0; i < 5; i++) {
        
        Block * b = (Block*)((uintptr_t)allocated[i] - BLOCK_SIZE);
        printf("address: %" PRIuPTR " address len: %" PRIuPTR "\n", (uintptr_t)allocated[i] - BLOCK_SIZE, (uintptr_t)b + b->size + BLOCK_SIZE);
    }

    printf("SEQ: freeing\n");
    // free each of them in order
    for (int j = 0; j < 5; j++) {
        freemem(allocated[j]);
    }

    print_heap(stdout);
    
    print_stats(&total_size, &total_free, &n_free_blocks, &start);
}

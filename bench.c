/* Sang Ouk Kim 
 * Ethan Anderon
 * cse374 - hw6 - 05/18/17
 *
 * bench.c is a benchmarking tool that executes a large number of calls to free and get memory
 * from a memory management program implemented for cse374.
 *
 * usage: bench [ntrials [pctget [pctlarge [small_limit [large_limit [random_seed ]]]]]]
 */
#include "mem.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>

#define NTRIALS 10000
#define PCTGET 50
#define PCTLARGE 10
#define SMALL_LIMIT 200
#define LARGE_LIMIT 20000
void print_stats(uintptr_t* total_size, uintptr_t* total_free, uintptr_t* n_free_blocks, clock_t* start);
bool nextBool(double probability);
int random_range(unsigned int min, unsigned int max);

// main is the entrypoint to bench
// usage: bench [ntrials [pctget [pctlarge [small_limit [large_limit [random_seed ]]]]]]
// ntrials is the total number of getmem plus freemem calls to randomly perform during this test. Default 10000.
// pctget is the percent of the total getmem/freemem calls that should be getmem. Default 50.
// pctlarge is the percent of the getmem calls that should request "large" blocks with a size greater than small_limit. Default 10.
// small_limit is the largest size in bytes of a "small" block. Default 200.
// large_limit is the largest size in bytes of a "large" block. Default 20000.
// random_seed is the initial seed value for the random number generator. Default: bytes read from /dev/urandom
int main(int argc, char *argv[]) {
    // print usage and exit if the arguments is greater than 7
    if (argc > 7) {
        fprintf(stderr,"usage: bench [ntrials [pctget [pctlarge [small_limit [large_limit [random_seed ]]]]]]\n");
        exit(0);
    }
    
    // get the default arguments
    long int ntrials = NTRIALS;
    long int pctget = PCTGET;
    long int pctlarge = PCTLARGE;
    long int small_limit = SMALL_LIMIT;
    long int large_limit = LARGE_LIMIT;
    long int random_seed = time(NULL);
    long int *arguments[] = {&ntrials, &pctget, &pctlarge, &small_limit, &large_limit, &random_seed};
    // iterate through the args and set the pointers
    for (int i = 1; i < argc; i++) {
        // use strtol to cast to int and check
        char *endptr;
        long int arg = strtol(argv[i], &endptr, 10);
        if (!*argv[i] || *endptr) {
            fprintf(stderr,"usage: bench [ntrials [pctget [pctlarge [small_limit [large_limit [random_seed ]]]]]]\n");
            exit(0);
        }
        // dereference the pointer at the index and set it to the arg
        *arguments[i - 1] = arg;
    }
    
    // start the clock
    clock_t start = clock();
    int interval = (int)((((double)ntrials/10)) + 0.5);
    uintptr_t total_size = 0;
    uintptr_t total_free = 0;
    uintptr_t n_free_blocks = 0;
    
    // keep track of all the allocated pointers
    void **allocated = (void*)malloc(sizeof(void*) * ntrials);
    int size = 0;

    // seed the random number generator
    srand(random_seed);
    for (int n = 0; n < ntrials; n++) {
        // check all the pointers
        for (int j = 0; j < size; j++) {
            assert(allocated[j] != NULL);
        }
        // if nextBool returns true call getmem and store the pointer
        if (nextBool((double)pctget/100)) {
            // if nextBool returns true getmem a random sized block between small and large
            if (nextBool((double)pctlarge/100)) {
                void *mem = getmem(random_range(small_limit, large_limit));
                if (mem != NULL) {
                    allocated[size] = mem;
                    assert(allocated[size] != NULL && "check that getting small block didn't return NULL");
                    size++;
                }
                // otherwise getmem a "small" block
            } else {
                int amount = random_range(small_limit, large_limit);
                
                void *mem =  getmem(random_range(1, small_limit));
                if (mem != NULL) {
                    allocated[size] = mem;
                    assert(allocated[size] != NULL && "check that getting small block didn't return NULL");
                    size++;
                }
            }
            // else free mem randomly between 0 and size
        } else {
            if (size > 0) {
                int ri = random_range(0, size - 1);
                assert(allocated[ri] != NULL);
                 
                freemem(allocated[ri]);
                allocated[ri] = NULL;
                // swap in the end to the removed item
                // and decrease the size
                if (ri != size - 1) {
                    allocated[ri] = allocated[size - 1];
                    assert(allocated[ri] != NULL && "check that the pointer is not null when swap");
                    allocated[size] = NULL;
                } else {
                    allocated[size] = NULL;
                }
                size--;
            }
            
        }
        
        // print the stats every 10% interval
        if (n != 0 && n % interval == 0) {
            print_stats(&total_size, &total_free, &n_free_blocks, &start);
        }
        
    }
    
    print_stats(&total_size, &total_free, &n_free_blocks, &start);
    printf("number of blocks allocated: %d\n", size);
    //print_heap(stdout);
    return 0;
}

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

int random_range(unsigned int min, unsigned int max) {
    double scaled = (double)rand()/RAND_MAX;
    return (int)(max - min +1)*scaled + min;
}

bool nextBool(double probability) {
    return (rand() / (double)RAND_MAX) < probability;
}

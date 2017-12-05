// this is a testfile


#include "mem.h"
#include "mem_impl.h"



int main (int argc, char *argv[]) {
    // call a sequence of known memory calls
    void *allocated[10];
    for (int i = 0; i < 10; i++) {
        allocated[i] = getmem(32);
    }

    // free each of them in order
    for (int j = 0; j < 10; j++) {
        freemem(allocated[j]);
    }

    print_heap(stdout);
}

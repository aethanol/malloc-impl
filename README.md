# malloc-impl
C Malloc Implementation

Ethan Anderson

The heap is designed as a linked list of ‘Block’. Each Block has a size field containing the size of the data that object holds and a next field containing the pointer to the next Block node in the linked list. Each Block of memory is assigned by malloc in the first place. We created another test case with fixed number of trials so that we can test and debug our code better.


-- bench stats --
The bench program allocates and frees memory randomly and in random order. It uses default values if the user doesn't specify parameters, with current time to be the random seed value.

The average runtime was between 0.00001 and 0.0000 with around 10k trials
The average block size is generally 2-10k

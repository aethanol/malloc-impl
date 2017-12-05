# Sang Ouk Kim 
# Ethan Anderon
# cse374 - hw6 - 05/18/17
# This is a Makefile for a memory management program and benchmarking tool that was implemented
# as a homework assignment for cse374.

all: bench

bench: bench.o getmem.o freemem.o get_mem_stats.o print_heap.o mem_utils.o
	gcc bench.o getmem.o freemem.o get_mem_stats.o print_heap.o mem_utils.o -o bench -std=c11 -g -Wall

bench.o: bench.c mem.h
	gcc -c bench.c

getmem.o: getmem.c mem.h mem_impl.h
	gcc -c getmem.c -o getmem.o -std=c11 -g -Wall

freemem.o: freemem.c mem.h mem_impl.h
	gcc -c freemem.c -o freemem.o -std=c11 -g -Wall

get_mem_stats.o: get_mem_stats.c mem.h mem_impl.h
	gcc -c get_mem_stats.c -o get_mem_stats.o -std=c11 -g -Wall

print_heap.o: print_heap.c mem.h mem_impl.h
	gcc -c print_heap.c -o print_heap.o -std=c11 -g -Wall

mem_utils.o: mem_utils.c mem.h mem_impl.h
	gcc -c mem_utils.c -o mem_utils.o -std=c11 -g -Wall

test: bench 
	./bench 100 50 100 200 20000 9

seq: sequence.o getmem.o freemem.o get_mem_stats.o print_heap.o mem_utils.o
	gcc sequence.o getmem.o freemem.o get_mem_stats.o print_heap.o mem_utils.o -o seq -std=c11 -g -Wall

sequence.o: sequence.c mem.h mem_impl.h
	gcc -c sequence.c -o sequence.o -std=c11 -g -Wall

clean:
	rm -f *.o bench seq

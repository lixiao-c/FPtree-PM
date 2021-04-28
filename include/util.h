#ifndef util_h
#define util_h

#include <iostream>
#include <fstream>
#include <map>
#include <mutex>
#include <cstdio>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <jemalloc/jemalloc.h>

#include <bits/stdc++.h>

#include <libvmem.h>
#include <libpmem.h>
#define VMEM_MIN_POOL1 1024*1024*1024*40L
VMEM *vmp;

void init_pmem_pool(){
	if ((vmp = vmem_create("/pmem6/btree_pool",
                    VMEM_MIN_POOL1)) == NULL) {
        perror("vmem_create");
        exit(1);
    }
}

void * pmalloc(size_t size)
{
    void* ret;
    if ((ret = vmem_aligned_alloc(vmp,64, size)) == NULL) {
      perror("vmem_malloc");
      exit(1);
    }
    return ret;
}

static __always_inline uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

#define asm_clwb(addr)\
	asm volatile(".byte 0x66; xsaveopt %0" : "+m" (*(volatile char *)addr));


#define asm_clflush(addr)					\
({								\
	__asm__ __volatile__ ("clwb %0" : : "m"(*addr));	\
})

// static inline void asm_mfence(void)
#define asm_mfence()				\
({									\
	__asm__ __volatile__ ("mfence");	\
})

//static inline void asm_sfence(void)
#define asm_sfence()				\
({									\
	__asm__ __volatile__ ("sfence");	\
})



#define CACHE_ALIGN 64

// #define NO_CACHELINE_FLUSH
/*
 * TODO: Now our cpu only support clflush, which is less effcient than clflushopt;
 * If clflushopt is available, we should add an automatic choice between these two instructions.
 */
static void flush_data(void* addr, size_t len){
#ifndef NO_CACHELINE_FLUSH
	char* end = (char*)(addr) + len;
	char* ptr = (char *)((unsigned long)addr &~(CACHE_ALIGN-1));
	for (; ptr < end; ptr += CACHE_ALIGN)
		asm_clflush(ptr);
	asm_mfence();
#endif
}


// prefetch instruction
//
#define CACHE_LINE_SIZE 64
inline void prefetch(const void *ptr) {
#ifdef NOPREFETCH
    (void) ptr;
#else
    typedef struct { char x[CACHE_LINE_SIZE]; } cacheline_t;
    asm volatile("prefetcht0 %0" : : "m" (*(const cacheline_t *)ptr));
#endif
}

/**
 * @brief find first zero bit in a word
 * @details
 *
 * @param long the word to find
 * @return the location of the first zero bit
 */

static __always_inline unsigned long ffz(unsigned long word)
{
	asm("rep; bsf %1,%0"
		: "=r" (word)
		: "r" (~word));
	return word;
}

#endif

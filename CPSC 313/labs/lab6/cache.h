/*
 * cache.h
 *
 * Definition of the structure used to represent a cache.
 */
#ifndef CACHE_H
#define CACHE_H

#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

/*
 * Replacement policies. The MASK defines which bits are used to
 * represent policies. We  define multiple policies, although we
 * will never ask to implement all of them.
 *
 * Therefore, you can check for a specific policy using:
 * if (policy & CACHE_REPLACEMENTPOLICY_MASK == CACHE_REPLACEMENTPOLICY_LRU) { ... }
 */
#define CACHE_REPLACEMENTPOLICY_MASK               0b00001110

#define CACHE_REPLACEMENTPOLICY_RANDOM             0b00000010
#define CACHE_REPLACEMENTPOLICY_LRU                0b00000100
#define CACHE_REPLACEMENTPOLICY_MRU                0b00000110
#define CACHE_REPLACEMENTPOLICY_LFU                0b00001000
#define CACHE_REPLACEMENTPOLICY_MFU                0b00001010
#define CACHE_REPLACEMENTPOLICY_RANDOMIZED_MARKING 0b00001100

/*
 * Write policies: We use two bits to indicate the write policy.
 * one bit represents either writethrough/writeback; the other
 * represents writeallocate/writenoallocate.
 */
#define CACHE_WRITEPOLICY_MASK   0b00110000

#define CACHE_WRITEPOLICY_WRITETHROUGH       0b00000000
#define CACHE_WRITEPOLICY_WRITEBACK          0b00010000

#define CACHE_WRITEPOLICY_WRITEALLOCATE      0b00100000
#define CACHE_WRITEPOLICY_WRITENOALLOCATE    0b00110000

/*
 * Other policies: Do we want to use cache tracing.
 */
#define CACHE_TRACE_MASK  0b00000001
#define CACHE_TRACEPOLICY 0b00000001

/*
 * Structure used to store a single cache line.
 */
typedef struct cache_line_s {

    /* The valid bit. */
    bool is_valid;

    /* The dirty bit. */
    bool is_dirty;

    /* The use_count value; also used as a 0/1 value for randomized marking. */
    uint8_t use_count;
    
    /* The tag. */
    uintptr_t tag;
  
    /* The cache block as bytes */
    uint8_t *block;
  
} cache_line_t;

/*
 * Structure used to store a cache set: a cache set contains a size
 * and a reference to the lines of the set.
 */
typedef struct cache_set_s {
    cache_line_t *lines;
    size_t first_index;
    size_t *lru_list;
    size_t num_marked;
} cache_set_t;

/*
 * Structure used to store a cache.
 */
typedef struct simcache_s { 
    /* Number of sets in the cache. */
    size_t num_sets;
  
    /* Number of lines in cache. */
    size_t num_lines;
  
    /* Number of bytes in a line. */
    size_t line_size;
  
    /* Associativity of the cache */
    size_t associativity;

    /* Mask for block offset. */
    uintptr_t block_offset_mask;
  
    /* Mask for cache index. */
    uintptr_t cache_index_mask;
  
    /* Shift for cache index. */
    unsigned int cache_index_shift;
  
    /* Mask for tag. */
    uintptr_t tag_mask;
  
    /* Shift for tag. */
    uint8_t tag_shift;
  
    /* Replacement and write policies. */
    uint8_t policies;
  
    /* All the memory in the cache */
    uint8_t *memory;

    /* Array of lines, each of which is an array of bytes. */
    cache_line_t *lines;
  
    /* Array of sets, each of which refers to its lines */
    cache_set_t *sets;
  
    /* Statistics about cache usage. */
    uint32_t access_count, miss_count;
} simcache_t;

typedef int (*func_t)(void);

/* Public functions */

/*
 * Create a new cache that contains a total of num_bytes line, each of which is block_size
 * bytes long, with the given associativity and policies.
 */
simcache_t *cache_new(size_t num_bytes, size_t associativity, size_t block_size, uint8_t policies);

/*
 * Frees all memory allocated for the given cache.
 */
void cache_free(simcache_t *cache);

/*
 * Read a single long integer from the cache.
 */
uint64_t cache_read(simcache_t *cache, uintptr_t address, func_t generate_random_number);

/*
 * Write a single long integer to memory and/or the cache.
 */
void cache_write(simcache_t *cache, uintptr_t address, uint64_t value, func_t generate_random_number);

/*
 * Return the number of cache misses since the cache was created.
 */
uint32_t cache_miss_count(simcache_t *cache);

/*
 * Return the number of cache accesses since the cache was created.
 */
uint32_t cache_access_count(simcache_t *cache);

/*
 *  Helpers
 */
bool cache_line_is_valid_tag_matches(cache_line_t *cache_line, uintptr_t tag);
uint64_t cache_line_retrieve_data(cache_line_t *cache_line, size_t offset);
cache_line_t *cache_set_find_line_matching(simcache_t *cache, cache_set_t *cache_set, uintptr_t tag);
size_t choose_unmarked_cache_line(simcache_t *cache, cache_set_t *cache_set, func_t generate_random_number);
cache_line_t *find_available_cache_line(simcache_t *cache, cache_set_t *cache_set, func_t generate_random_number);

#endif

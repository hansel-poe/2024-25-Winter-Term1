#include "catch.hpp"
extern "C"
{
#include "cache.h"
}

TEST_CASE("cache_line_check_validity_and_tag", "[weight=1][part=test]")
{
    cache_line_t cache_line;
    cache_line.is_valid = true;
    cache_line.tag = 10;

    bool actual = !!cache_line_is_valid_tag_matches(&cache_line, 10);
    REQUIRE(actual == true);

    actual = !!cache_line_is_valid_tag_matches(&cache_line, 9);
    REQUIRE(actual == false);

    cache_line.is_valid = false;
    cache_line.tag = 10;

    actual = !!cache_line_is_valid_tag_matches(&cache_line, 10);
    REQUIRE(actual == false);

    actual = !!cache_line_is_valid_tag_matches(&cache_line, 9);
    REQUIRE(actual == false);
}

TEST_CASE("cache_line_retrieve_data", "[weight=1][part=test]")
{
    uint64_t block[] = {100, 101};
    cache_line_t cache_line;
    cache_line.block = (uint8_t *)block;

    long data = cache_line_retrieve_data(&cache_line, 0);
    REQUIRE(data == 100);

    data = cache_line_retrieve_data(&cache_line, 8);
    REQUIRE(data == 101);
}

#define ASSERT_EQUAL(A, B) REQUIRE((A) == (B))

TEST_CASE("cache_set_find_line_matching::RANDOMIZED_MARKING", "[weight=1][part=test]")
{
    simcache_t cache;
    cache.policies = CACHE_REPLACEMENTPOLICY_RANDOMIZED_MARKING;
    cache.num_lines = 16;
    cache.num_sets = 4;
    cache.associativity = cache.num_lines / cache.num_sets;

    cache_set_t cache_set;
    cache_line_t lines[] = {{true, false, 0, 0, 10}, {true, false, 0, 0, 11}, {true, false, 0, 0, 12}, {false, false, 0, 0, 13}};
    size_t lru_list[] = {3, 2, 1, 0};
    cache_set.lines = lines;
    cache_set.lru_list = lru_list;

    cache_line_t * actual = cache_set_find_line_matching(&cache, &cache_set, 11);
    ASSERT_EQUAL(actual, &lines[1]);
    ASSERT_EQUAL(lru_list[0], 3);
    ASSERT_EQUAL(lru_list[1], 2);
    ASSERT_EQUAL(lru_list[2], 1);
    ASSERT_EQUAL(lru_list[3], 0);

    actual = cache_set_find_line_matching(&cache, &cache_set, 12);
    ASSERT_EQUAL(actual, &lines[2]);
    ASSERT_EQUAL(lru_list[0], 3);
    ASSERT_EQUAL(lru_list[1], 2);
    ASSERT_EQUAL(lru_list[2], 1);
    ASSERT_EQUAL(lru_list[3], 0);

    actual = cache_set_find_line_matching(&cache, &cache_set, 10);
    ASSERT_EQUAL(actual, &lines[0]);
    ASSERT_EQUAL(lru_list[0], 3);
    ASSERT_EQUAL(lru_list[1], 2);
    ASSERT_EQUAL(lru_list[2], 1);
    ASSERT_EQUAL(lru_list[3], 0);
    
    actual = cache_set_find_line_matching(&cache, &cache_set, 13);
    ASSERT_EQUAL(actual, NULL);
    ASSERT_EQUAL(lru_list[0], 3);
    ASSERT_EQUAL(lru_list[1], 2);
    ASSERT_EQUAL(lru_list[2], 1);
    ASSERT_EQUAL(lru_list[3], 0);

    
    actual = cache_set_find_line_matching(&cache, &cache_set, 14);
    ASSERT_EQUAL(actual, NULL);
    ASSERT_EQUAL(lru_list[0], 3);
    ASSERT_EQUAL(lru_list[1], 2);
    ASSERT_EQUAL(lru_list[2], 1);
    ASSERT_EQUAL(lru_list[3], 0);
}

TEST_CASE("cache_set_find_line_matching::LFU", "[weight=1][part=test]")
{
    simcache_t cache;
    cache.policies = CACHE_REPLACEMENTPOLICY_LFU;
    cache.num_lines = 16;
    cache.num_sets = 4;
    cache.age = 6;
    cache.associativity = cache.num_lines / cache.num_sets;

    cache_set_t cache_set;
    cache_line_t lines[] = {{true, false, 6, 1, 10}, {false, false, 0, 0, 13}, {true, false, 5, 1, 11}, {true, false, 5, 1, 12}};
    size_t lru_list[] = {0, 3, 2, 1};
    cache_set.lines = lines;
    cache_set.lru_list = lru_list;

    cache_line_t * actual = cache_set_find_line_matching(&cache, &cache_set, 11);
    ASSERT_EQUAL(actual, &lines[2]);
    ASSERT_EQUAL(cache.age, 6);
    ASSERT_EQUAL(lru_list[0], 2);
    ASSERT_EQUAL(lru_list[1], 0);
    ASSERT_EQUAL(lru_list[2], 3);
    ASSERT_EQUAL(lru_list[3], 1);
    ASSERT_EQUAL(lines[0].age, 6);
    ASSERT_EQUAL(lines[1].age, 0);
    ASSERT_EQUAL(lines[2].age, 6);
    ASSERT_EQUAL(lines[3].age, 5);
    ASSERT_EQUAL(lines[0].use_count, 1);
    ASSERT_EQUAL(lines[1].use_count, 0);
    ASSERT_EQUAL(lines[2].use_count, 2);
    ASSERT_EQUAL(lines[3].use_count, 1);

    actual = cache_set_find_line_matching(&cache, &cache_set, 11);
    ASSERT_EQUAL(actual, &lines[2]);
    ASSERT_EQUAL(cache.age, 6);
    ASSERT_EQUAL(lru_list[0], 2);
    ASSERT_EQUAL(lru_list[1], 0);
    ASSERT_EQUAL(lru_list[2], 3);
    ASSERT_EQUAL(lru_list[3], 1);
    ASSERT_EQUAL(lines[0].age, 6);
    ASSERT_EQUAL(lines[1].age, 0);
    ASSERT_EQUAL(lines[2].age, 6);
    ASSERT_EQUAL(lines[3].age, 5);
    ASSERT_EQUAL(lines[0].use_count, 1);
    ASSERT_EQUAL(lines[1].use_count, 0);
    ASSERT_EQUAL(lines[2].use_count, 3);
    ASSERT_EQUAL(lines[3].use_count, 1);

    actual = cache_set_find_line_matching(&cache, &cache_set, 12);
    ASSERT_EQUAL(actual, &lines[3]);
    ASSERT_EQUAL(cache.age, 6);
    ASSERT_EQUAL(lru_list[0], 2);
    ASSERT_EQUAL(lru_list[1], 3);
    ASSERT_EQUAL(lru_list[2], 0);
    ASSERT_EQUAL(lru_list[3], 1);
    ASSERT_EQUAL(lines[0].age, 6);
    ASSERT_EQUAL(lines[1].age, 0);
    ASSERT_EQUAL(lines[2].age, 6);
    ASSERT_EQUAL(lines[3].age, 6);
    ASSERT_EQUAL(lines[0].use_count, 1);
    ASSERT_EQUAL(lines[1].use_count, 0);
    ASSERT_EQUAL(lines[2].use_count, 3);
    ASSERT_EQUAL(lines[3].use_count, 2);
    
    actual = cache_set_find_line_matching(&cache, &cache_set, 13);
    ASSERT_EQUAL(actual, NULL);
    ASSERT_EQUAL(cache.age, 6);
    ASSERT_EQUAL(lru_list[0], 2);
    ASSERT_EQUAL(lru_list[1], 3);
    ASSERT_EQUAL(lru_list[2], 0);
    ASSERT_EQUAL(lru_list[3], 1);
    ASSERT_EQUAL(lines[0].age, 6);
    ASSERT_EQUAL(lines[1].age, 0);
    ASSERT_EQUAL(lines[2].age, 6);
    ASSERT_EQUAL(lines[3].age, 6);
    ASSERT_EQUAL(lines[0].use_count, 1);
    ASSERT_EQUAL(lines[1].use_count, 0);
    ASSERT_EQUAL(lines[2].use_count, 3);
    ASSERT_EQUAL(lines[3].use_count, 2);

    actual = cache_set_find_line_matching(&cache, &cache_set, 14);
    ASSERT_EQUAL(actual, NULL);
    ASSERT_EQUAL(cache.age, 6);
    ASSERT_EQUAL(lru_list[0], 2);
    ASSERT_EQUAL(lru_list[1], 3);
    ASSERT_EQUAL(lru_list[2], 0);
    ASSERT_EQUAL(lru_list[3], 1);
    ASSERT_EQUAL(lines[0].age, 6);
    ASSERT_EQUAL(lines[1].age, 0);
    ASSERT_EQUAL(lines[2].age, 6);
    ASSERT_EQUAL(lines[3].age, 6);
    ASSERT_EQUAL(lines[0].use_count, 1);
    ASSERT_EQUAL(lines[1].use_count, 0);
    ASSERT_EQUAL(lines[2].use_count, 3);
    ASSERT_EQUAL(lines[3].use_count, 2);
    
    actual = cache_set_find_line_matching(&cache, &cache_set, 10);
    ASSERT_EQUAL(actual, &lines[0]);
    ASSERT_EQUAL(cache.age, 6);
    ASSERT_EQUAL(lru_list[0], 2);
    ASSERT_EQUAL(lru_list[1], 0);
    ASSERT_EQUAL(lru_list[2], 3);
    ASSERT_EQUAL(lru_list[3], 1);
    ASSERT_EQUAL(lines[0].age, 6);
    ASSERT_EQUAL(lines[1].age, 0);
    ASSERT_EQUAL(lines[2].age, 6);
    ASSERT_EQUAL(lines[3].age, 6);
    ASSERT_EQUAL(lines[0].use_count, 2);
    ASSERT_EQUAL(lines[1].use_count, 0);
    ASSERT_EQUAL(lines[2].use_count, 3);
    ASSERT_EQUAL(lines[3].use_count, 2);
}

TEST_CASE("find_available_cache_line::RANDOMIZED_MARKING", "[weight=1][part=test]")
{
    simcache_t cache;
    cache.policies = CACHE_REPLACEMENTPOLICY_RANDOMIZED_MARKING;
    cache.num_lines = 16;
    cache.num_sets = 4;
    cache.associativity = cache.num_lines / cache.num_sets;

    cache_set_t cache_set;
    cache_line_t lines[] = {{true, false, 0, 0, 10}, {true, false, 0, 0, 11}, {false, false, 0, 0, 12}, {false, false, 0, 0, 13}};
    size_t lru_list[] = {3, 2, 1, 0};
    cache_set.lines = lines;
    cache_set.num_marked = 0;
    cache_set.lru_list = lru_list;
    
    // For testing purposes, we are mocking "generate_random_number" to always return 1
    // If you are really measuring the miss rate of your cache, use "rand" (from stdlib.h) instead of "[](){ return 1; }"
    cache_line_t *actual = find_available_cache_line(&cache, &cache_set, [](){ return 1; });
    ASSERT_EQUAL(actual, &lines[2]);
    ASSERT_EQUAL(lru_list[0], 3);
    ASSERT_EQUAL(lru_list[1], 2);
    ASSERT_EQUAL(lru_list[2], 1);
    ASSERT_EQUAL(lru_list[3], 0);
    ASSERT_EQUAL(cache_set.num_marked, 1);
    ASSERT_EQUAL(lines[0].use_count, 0);
    ASSERT_EQUAL(lines[1].use_count, 0);
    ASSERT_EQUAL(lines[2].use_count, 1);
    ASSERT_EQUAL(lines[3].use_count, 0);

    lines[2].is_valid = true;
    actual = find_available_cache_line(&cache, &cache_set, [](){ return 1; });
    ASSERT_EQUAL(actual, &lines[3]);
    ASSERT_EQUAL(lru_list[0], 3);
    ASSERT_EQUAL(lru_list[1], 2);
    ASSERT_EQUAL(lru_list[2], 1);
    ASSERT_EQUAL(lru_list[3], 0);
    ASSERT_EQUAL(cache_set.num_marked, 2);
    ASSERT_EQUAL(lines[0].use_count, 0);
    ASSERT_EQUAL(lines[1].use_count, 0);
    ASSERT_EQUAL(lines[2].use_count, 1);
    ASSERT_EQUAL(lines[3].use_count, 1);

    lines[3].is_valid = true;
    actual = find_available_cache_line(&cache, &cache_set, [](){ return 0; });
    ASSERT_EQUAL(actual, &lines[0]);
    ASSERT_EQUAL(lru_list[0], 3);
    ASSERT_EQUAL(lru_list[1], 2);
    ASSERT_EQUAL(lru_list[2], 1);
    ASSERT_EQUAL(lru_list[3], 0);
    ASSERT_EQUAL(cache_set.num_marked, 3);
    ASSERT_EQUAL(lines[0].use_count, 1);
    ASSERT_EQUAL(lines[1].use_count, 0);
    ASSERT_EQUAL(lines[2].use_count, 1);
    ASSERT_EQUAL(lines[3].use_count, 1);

    actual = find_available_cache_line(&cache, &cache_set, [](){ return 1; });
    ASSERT_EQUAL(actual, &lines[1]);
    ASSERT_EQUAL(lru_list[0], 3);
    ASSERT_EQUAL(lru_list[1], 2);
    ASSERT_EQUAL(lru_list[2], 1);
    ASSERT_EQUAL(lru_list[3], 0);
    ASSERT_EQUAL(cache_set.num_marked, 4);
    ASSERT_EQUAL(lines[0].use_count, 1);
    ASSERT_EQUAL(lines[1].use_count, 1);
    ASSERT_EQUAL(lines[2].use_count, 1);
    ASSERT_EQUAL(lines[3].use_count, 1);

    actual = find_available_cache_line(&cache, &cache_set, [](){ return 2; });
    ASSERT_EQUAL(actual, &lines[2]);
    ASSERT_EQUAL(lru_list[0], 3);
    ASSERT_EQUAL(lru_list[1], 2);
    ASSERT_EQUAL(lru_list[2], 1);
    ASSERT_EQUAL(lru_list[3], 0);
    ASSERT_EQUAL(cache_set.num_marked, 1);
    ASSERT_EQUAL(lines[0].use_count, 0);
    ASSERT_EQUAL(lines[1].use_count, 0);
    ASSERT_EQUAL(lines[2].use_count, 1);
    ASSERT_EQUAL(lines[3].use_count, 0);

    actual = find_available_cache_line(&cache, &cache_set, [](){ return 5; });
    ASSERT_EQUAL(actual, &lines[3]);
    ASSERT_EQUAL(lru_list[0], 3);
    ASSERT_EQUAL(lru_list[1], 2);
    ASSERT_EQUAL(lru_list[2], 1);
    ASSERT_EQUAL(lru_list[3], 0);
    ASSERT_EQUAL(cache_set.num_marked, 2);
    ASSERT_EQUAL(lines[0].use_count, 0);
    ASSERT_EQUAL(lines[1].use_count, 0);
    ASSERT_EQUAL(lines[2].use_count, 1);
    ASSERT_EQUAL(lines[3].use_count, 1);

    actual = find_available_cache_line(&cache, &cache_set, [](){ return 6; });
    ASSERT_EQUAL(actual, &lines[0]);
    ASSERT_EQUAL(lru_list[0], 3);
    ASSERT_EQUAL(lru_list[1], 2);
    ASSERT_EQUAL(lru_list[2], 1);
    ASSERT_EQUAL(lru_list[3], 0);
    ASSERT_EQUAL(cache_set.num_marked, 3);
    ASSERT_EQUAL(lines[0].use_count, 1);
    ASSERT_EQUAL(lines[1].use_count, 0);
    ASSERT_EQUAL(lines[2].use_count, 1);
    ASSERT_EQUAL(lines[3].use_count, 1);
}

TEST_CASE("find_available_cache_line::LFU", "[weight=1][part=test]")
{
    simcache_t cache;
    cache.policies = CACHE_REPLACEMENTPOLICY_LFU;
    cache.num_lines = 16;
    cache.num_sets = 4;
    cache.age = 6;
    cache.associativity = cache.num_lines / cache.num_sets;

    cache_set_t cache_set;
    cache_line_t lines[] = {{true, false, 6, 1, 10}, {false, false, 0, 0, 13}, {true, false, 5, 1, 11}, {true, false, 5, 1, 12}};
    size_t lru_list[] = {0, 3, 2, 1};
    cache_set.lines = lines;
    cache_set.lru_list = lru_list;

    /* Mock finding 11 without checking things are working */
    cache_set_find_line_matching(&cache, &cache_set, 11);

    /* Now find a cache line to store value 14. */
    cache_line_t *actual = find_available_cache_line(&cache, &cache_set, [](){ return 1; });
    ASSERT_EQUAL(actual, &lines[1]);
    ASSERT_EQUAL(cache.age, 6);
    ASSERT_EQUAL(lru_list[0], 2);
    ASSERT_EQUAL(lru_list[1], 1);
    ASSERT_EQUAL(lru_list[2], 0); 
    ASSERT_EQUAL(lru_list[3], 3);
    ASSERT_EQUAL(lines[0].age, 6);
    ASSERT_EQUAL(lines[1].age, 6);
    ASSERT_EQUAL(lines[2].age, 6);
    ASSERT_EQUAL(lines[3].age, 5);
    ASSERT_EQUAL(lines[0].use_count, 1);
    ASSERT_EQUAL(lines[1].use_count, 1);
    ASSERT_EQUAL(lines[2].use_count, 2);
    ASSERT_EQUAL(lines[3].use_count, 1);

    /* Fake adding data. */
    lines[1].tag = 14;

    /* Mock finding 12 without checking things are working */
    actual = cache_set_find_line_matching(&cache, &cache_set, 12);
    ASSERT_EQUAL(actual, &lines[3]);
    ASSERT_EQUAL(cache.age, 6);
    ASSERT_EQUAL(lru_list[0], 3);
    ASSERT_EQUAL(lru_list[1], 2);
    ASSERT_EQUAL(lru_list[2], 1);
    ASSERT_EQUAL(lru_list[3], 0);
    ASSERT_EQUAL(lines[0].age, 6);
    ASSERT_EQUAL(lines[1].age, 6);
    ASSERT_EQUAL(lines[2].age, 6);
    ASSERT_EQUAL(lines[3].age, 6);
    ASSERT_EQUAL(lines[0].use_count, 1);
    ASSERT_EQUAL(lines[1].use_count, 1);
    ASSERT_EQUAL(lines[2].use_count, 2);
    ASSERT_EQUAL(lines[3].use_count, 2);

    /* Now find a cache line to store value 15. */
    actual = find_available_cache_line(&cache, &cache_set, [](){ return 1; });
    ASSERT_EQUAL(actual, &lines[0]);
    ASSERT_EQUAL(cache.age, 7);
    ASSERT_EQUAL(lru_list[0], 0);
    ASSERT_EQUAL(lru_list[1], 3);
    ASSERT_EQUAL(lru_list[2], 2);
    ASSERT_EQUAL(lru_list[3], 1);
    ASSERT_EQUAL(lines[0].age, 7);
    ASSERT_EQUAL(lines[1].age, 6);
    ASSERT_EQUAL(lines[2].age, 6);
    ASSERT_EQUAL(lines[3].age, 6);
    ASSERT_EQUAL(lines[0].use_count, 1);
    ASSERT_EQUAL(lines[1].use_count, 1);
    ASSERT_EQUAL(lines[2].use_count, 2);
    ASSERT_EQUAL(lines[3].use_count, 2);
    
    /* Fake adding data. */
    lines[0].tag = 15;

    /* Mock finding 11 twice without checking things are working. */
    cache_set_find_line_matching(&cache, &cache_set, 11);
    cache_set_find_line_matching(&cache, &cache_set, 11);

    /* Now find a cache line to store 10 (which was evicted earlier). */
    actual = find_available_cache_line(&cache, &cache_set, [](){ return 1; });
    ASSERT_EQUAL(actual, &lines[1]);
    ASSERT_EQUAL(cache.age, 7);
    ASSERT_EQUAL(lru_list[0], 2);
    ASSERT_EQUAL(lru_list[1], 1);
    ASSERT_EQUAL(lru_list[2], 0);
    ASSERT_EQUAL(lru_list[3], 3);
    ASSERT_EQUAL(lines[0].age, 7);
    ASSERT_EQUAL(lines[1].age, 7);
    ASSERT_EQUAL(lines[2].age, 7);
    ASSERT_EQUAL(lines[3].age, 6);
    ASSERT_EQUAL(lines[0].use_count, 1);
    ASSERT_EQUAL(lines[1].use_count, 1);
    ASSERT_EQUAL(lines[2].use_count, 4);
    ASSERT_EQUAL(lines[3].use_count, 2);
    
    /* Fake adding data. */
    lines[0].tag = 10;
}

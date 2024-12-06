#pragma once
#include <assert.h>
#include <stdio.h>

#ifdef NDEBUG
#define muse_assert_valid_ptr(ptr, msg) ((void)0)
#else

#define muse_assert_valid_ptr(ptr, msg) do { \
    if (ptr == NULL) { \
        printf(msg);    \
        assert(ptr != NULL); \
    } \
} while (0)

#endif
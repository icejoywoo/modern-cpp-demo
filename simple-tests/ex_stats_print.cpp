// https://github.com/jemalloc/jemalloc/wiki/Getting-Started
// https://jemalloc.net/mailman/jemalloc-discuss/2013-July/000617.html
#include <stdlib.h>
#include <jemalloc/jemalloc.h>

void
do_something(size_t i) {
    // Leak some memory.
    malloc(i * 100);
}

int
main(int argc, char **argv) {
    for (size_t i = 0; i < 1000; i++) {
            do_something(i);
    }

    // Dump allocator statistics to stderr.
    malloc_stats_print(NULL, NULL, NULL);

    return 0;
}

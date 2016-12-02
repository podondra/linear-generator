#include <stdio.h>
#include <unistd.h>

int main() {
    printf("_SC_LEVEL1_DCACHE_SIZE\t%ld\n", sysconf(_SC_LEVEL1_DCACHE_SIZE));
    printf("_SC_LEVEL1_DCACHE_ASSOC\t%ld\n", sysconf(_SC_LEVEL1_DCACHE_ASSOC));
    printf("_SC_LEVEL1_DCACHE_LINESIZE\t%ld\n", sysconf(_SC_LEVEL1_DCACHE_LINESIZE));

    printf("_SC_LEVEL2_CACHE_SIZE\t%ld\n", sysconf(_SC_LEVEL2_CACHE_SIZE));
    printf("_SC_LEVEL2_CACHE_ASSOC\t%ld\n", sysconf(_SC_LEVEL2_CACHE_ASSOC));
    printf("_SC_LEVEL2_CACHE_LINESIZE\t%ld\n", sysconf(_SC_LEVEL2_CACHE_LINESIZE));

    printf("_SC_LEVEL3_CACHE_SIZE\t%ld\n", sysconf(_SC_LEVEL3_CACHE_SIZE));
    printf("_SC_LEVEL3_CACHE_ASSOC\t%ld\n", sysconf(_SC_LEVEL3_CACHE_ASSOC));
    printf("_SC_LEVEL3_CACHE_LINESIZE\t%ld\n", sysconf(_SC_LEVEL3_CACHE_LINESIZE));

    return 0;
}

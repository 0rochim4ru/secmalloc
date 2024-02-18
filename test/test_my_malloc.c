#include "my_secmalloc.h"
#include <stdio.h>
#include <string.h>

int main() {
    init_memory_pool();

    size_t allocation_size1 = 10;
    size_t allocation_size2 = 20;
    size_t allocation_size3 = 30;

    void* allocated_memory1 = my_malloc(allocation_size1);
    void* allocated_memory2 = my_malloc(allocation_size2);
    void* allocated_memory3 = my_malloc(allocation_size3);

    if (allocated_memory1 != NULL && allocated_memory2 != NULL && allocated_memory3 != NULL) {
        printf("Les allocations ont réussi\n");
        
        my_free(allocated_memory1);
        my_free(allocated_memory1);

        my_free(allocated_memory2);

        memset(allocated_memory3, 0, allocation_size3 + 1);
        my_free(allocated_memory3);
    } else {
        printf("Échec de l'allocation.\n");
    }

    return 0;
}

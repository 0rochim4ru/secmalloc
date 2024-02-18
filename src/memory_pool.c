#include "../include/my_secmalloc.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>

void* data_pool = NULL; // Le pool de données global
BlockMeta* meta_pool = NULL;

void init_memory_pool() {
    if (data_pool == NULL) {
        data_pool = mmap(NULL, POOL_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	printf("L'adresse du data_pool est la suivante :%p\n",data_pool);
        if (data_pool == MAP_FAILED) {
            perror("Erreur lors de l'initialisation du pool de données");
            exit(EXIT_FAILURE);
        }
    }
    if (meta_pool == NULL)
    {
         size_t meta_pool_size = sizeof(BlockMeta) * MAX_BLOCKS;
         meta_pool = (BlockMeta*)mmap(NULL, meta_pool_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
         printf("L'adresse du meta_pool est la suivante%p\n\n", meta_pool);
         if (meta_pool == MAP_FAILED)
	 {
             perror("Erreur lors de l'initialisation du pool de métadonnées");
             exit(EXIT_FAILURE);
         }
    }
    size_t default_block_size = POOL_SIZE / MAX_BLOCKS;
    for (int i = 0; i < MAX_BLOCKS; i++) {
        meta_pool[i].start = NULL;
        meta_pool[i].size = default_block_size;
        meta_pool[i].is_free = 1;
    }
}



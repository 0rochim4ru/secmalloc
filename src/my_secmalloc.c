#include "../include/my_secmalloc.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void* my_malloc(size_t requested_size) {
    if (requested_size == 0) {
        printf("Echec de l'allocation : taille demandée 0 Octet\n");
        return NULL;
    }
    if (data_pool == NULL || meta_pool == NULL) {
        init_memory_pool();
    }

    size_t total_size = requested_size + sizeof(unsigned long) * 2; // Total size including canaries
    int needed_blocks = (total_size + 8191) / 8192;
    int consecutive_free_blocks = 0;
    int start_block_index = -1;

    for (int i = 0; i < MAX_BLOCKS; i++) {
        if (meta_pool[i].is_free) {
            if (start_block_index == -1) {
                start_block_index = i;
            }
            consecutive_free_blocks++;
            if (consecutive_free_blocks == needed_blocks) {
                unsigned long canary_value = 0xDEADBEEF;
                meta_pool[start_block_index].start = (char*)data_pool + (start_block_index * 8192) + sizeof(canary_value);
                meta_pool[start_block_index].is_free = 0;
                meta_pool[start_block_index].size = requested_size;
                meta_pool[start_block_index].allocation_size = total_size;
                meta_pool[start_block_index].blocks = needed_blocks;
                meta_pool[start_block_index].canary_start = canary_value;
                meta_pool[start_block_index].canary_end = canary_value;

                *(unsigned long*)((char*)meta_pool[start_block_index].start - sizeof(canary_value)) = canary_value;
                *(unsigned long*)((char*)meta_pool[start_block_index].start + requested_size) = canary_value;

                printf("Allocation réussie: Bloc %d, taille=%zu, adresse=%p\n", start_block_index, requested_size, meta_pool[start_block_index].start);
                return meta_pool[start_block_index].start;
            }
        } else {
            consecutive_free_blocks = 0;
            start_block_index = -1;
        }
    }

    printf("Échec de l'allocation: Pas de séquence suffisante de blocs libres trouvée pour la taille demandée de %zu octets.\n", requested_size);
    return NULL;
}



void my_free(void* ptr) {
    if (ptr == NULL) return;

    size_t offset = (char*)ptr - (char*)data_pool - sizeof(unsigned long);
    int block_index = offset / 8192;

    if (block_index < 0 || block_index >= MAX_BLOCKS) {
        printf("Erreur : Adresse à libérer non valide.\n");
        return;
    }

    if (meta_pool[block_index].is_free) {
        printf("Erreur Double Free : Tentative de double libération.\n");
        return;
    }

    unsigned long canary_start = *(unsigned long*)((char*)meta_pool[block_index].start - sizeof(unsigned long));
    unsigned long canary_end = *(unsigned long*)((char*)meta_pool[block_index].start + meta_pool[block_index].size);

    if (canary_start != 0xDEADBEEF || canary_end != 0xDEADBEEF) {
        printf("Heap Overflow : Erreur de corruption de mémoire détectée.\n");
        return;
    }

    meta_pool[block_index].is_free = 1;
    printf("Bloc %d libéré à partir de l'adresse %p\n", block_index, ptr);
}

#define _GNU_SOURCE
#include <link.h>
#include <stdio.h>
#include <stdlib.h>
#include "xxhash.h"

static int
callback(struct dl_phdr_info *info, size_t size, void *data)
{
    XXH64_state_t* state = (XXH64_state_t*)data;
    for (int header_id = 0; header_id < info->dlpi_phnum; header_id++) {
        const ElfW(Phdr) *hdr = &info->dlpi_phdr[header_id];
        // Skip over headers that can not include read-only data that
        // possibly is also executable code:
        if (hdr->p_memsz == 0) {
            continue;
        }
        if ((hdr->p_flags & (PF_R | PF_X)) != (PF_R | PF_X)) {
            continue;
        }
        if (hdr->p_flags & PF_W) {
            continue;
        }

        XXH_errorcode result = XXH64_update(
            state,
            (void*)(info->dlpi_addr + hdr->p_vaddr),
            hdr->p_memsz);
        if (result != XXH_OK) {
            abort();
        }
    }
    return 0;
}

int main(void)
{
    XXH64_state_t* state = XXH64_createState();
    if (state == NULL) {
        fprintf(stderr, "Unable to allocate xxHash state!\n");
        return EXIT_FAILURE;
    }
    if (XXH64_reset(state, 0) != XXH_OK) {
        fprintf(stderr, "Failed to zero xxHash state!\n");
        return EXIT_FAILURE;
    }
    dl_iterate_phdr(callback, state);
    XXH64_hash_t digest = XXH64_digest (state);
    XXH64_canonical_t canonical;
    XXH64_canonicalFromHash(&canonical, digest);
    for (size_t byte_id = 0; byte_id < sizeof(canonical.digest); byte_id++) {
        printf("%02x", canonical.digest[byte_id]);
    }
    printf("\n");
    XXH64_freeState(state);
    return EXIT_SUCCESS;
}

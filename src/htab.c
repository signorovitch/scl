#include "include/htab.h"
#include "include/fnv1a.h"
#include "include/util.h"
#include "include/util.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

HTab* htab_init() {
    HTab* htab = calloc(1, sizeof(HTab));

    return htab;
}

void htab_destroy(HTab *htab) {
    free(htab);
}

// Get the index of a key.
size_t geti(char* key) {
    uint64_t hash = fnv1a_hash(key, strlen(key));
    size_t i = hash & (HTAB_SPACE - 1);
    return i;
}

void* htab_get(HTab* htab, char* key) {
    size_t i = geti(key);
    log_dbgf("Getting something from hash table @ index %lu", i);
    return (*htab)[i];
}

void htab_ins(HTab* htab, char* key, void* data) {
    size_t i = geti(key);
    assert((*htab)[i] == NULL);
    (*htab)[i] = data;
    log_dbgf("Inserted something to hash table @ index %lu", i);
}

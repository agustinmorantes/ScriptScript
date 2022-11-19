#include <stdlib.h>
#include <string.h>
#include "symtable.h"
#include "map.h"

static hashmap* m;

void symtable_init() {
    m = hashmap_create();
}

static void free_entry(void* key, size_t ksize, uintptr_t val, void* usr) {
    free(key);
    free((void*)val);
}

void symtable_free() {
    hashmap_iterate(m, free_entry, NULL);
    hashmap_free(m);
}

bool symtable_exists(const char* id) {
    char* key = strdup(id);
    bool ret = hashmap_get(m, key, strlen(key), NULL);
    free(key);
    return ret;
}

SymtableEntry* symtable_add(const char* id) {
    if (symtable_exists(id)) return NULL;

    char* key = strdup(id);
    SymtableEntry* entry = calloc(1,sizeof(SymtableEntry));
    hashmap_set(m, key, strlen(key), (uintptr_t)entry);

    return entry;
}

SymtableEntry* symtable_get(const char* id) {
    char* key = strdup(id);
    
    uintptr_t val = (uintptr_t)NULL;
    hashmap_get(m, key, strlen(key), &val);

    free(key);
    return (SymtableEntry*)val;
}

void symtable_set_type(const char* id, SymtableEntryType type) {
    SymtableEntry* entry = symtable_get(id);
    if (entry) entry->type = type;
    else {
        entry = symtable_add(id);
        entry->type = type;
    }
}

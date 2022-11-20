#include <stdlib.h>
#include <string.h>
#include "symtable.h"
#include "map.h"
#include "shared.h"

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
    char* key = cstrdup(id);
    uintptr_t val;
    bool ret = hashmap_get(m, key, strlen(key), &val);
    free(key);
    return ret;
}

SymtableEntry* symtable_add(const char* id) {
    if (symtable_exists(id)) return symtable_get(id);

    char* key = cstrdup(id);
    SymtableEntry* entry = calloc(1,sizeof(SymtableEntry));
    hashmap_set(m, key, strlen(key), (uintptr_t)entry);

    return entry;
}

SymtableEntry* symtable_add_with_type(const char* id, SymtableEntryType type) {
    SymtableEntry* entry = symtable_add(id);
    entry->type = type;
    return entry;
}

SymtableEntry* symtable_get(const char* id) {
    char* key = cstrdup(id);
    
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

static void check_entry(void* key, size_t ksize, uintptr_t val, void* usr) {
    bool* err = (bool*)usr;

    SymtableEntry* entry = (SymtableEntry*)val;
    if (entry->type == STT_ENTRYTYPE_TBD) {
        printf("Error: El ID '%s' se utiliza sin ser definido (¿ID de bloque inválido?).\n", (char*)key);
        *err = true;
    }
}

bool symtable_check() {
    bool err = false;
    hashmap_iterate(m, check_entry, &err);
    return !err;
}

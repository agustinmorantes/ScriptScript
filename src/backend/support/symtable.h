#pragma once

#include <stdbool.h>

typedef enum {
    ST_ENTRYTYPE_TBD = 0,
    ST_ENTRYTYPE_VAR,
    ST_ENTRYTYPE_BLOCK,
    ST_ENTRYTYPE_TRIGGER
} SymtableEntryType;

typedef struct {
    SymtableEntryType type;
} SymtableEntry;

void symtable_init();
void symtable_free();

bool symtable_exists(const char* id);
SymtableEntry* symtable_add(const char* id);
SymtableEntry* symtable_get(const char* id);
void symtable_set_type(const char* id, SymtableEntryType type);

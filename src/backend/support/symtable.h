#pragma once

#include <stdbool.h>

typedef enum {
    STT_ENTRYTYPE_TBD = 0,
    STT_ENTRYTYPE_VAR,
    STT_ENTRYTYPE_BLOCK,
    STT_ENTRYTYPE_TRIGGER
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

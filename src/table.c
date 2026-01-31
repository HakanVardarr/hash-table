#include "table.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint64_t fnv1a_hash(const void *data, size_t len)
{
    const uint8_t *bytes = data;
    uint64_t hash = 14695981039346656037ULL;

    for (size_t i = 0; i < len; i++)
    {
        hash ^= bytes[i];
        hash *= 1099511628211ULL;
    }

    return hash;
}

Entry entry_create(const char *key, int value)
{
    Entry e = {0};
    e.key = key;
    e.value = value;
    e.used = 1;
    return e;
}

Table table_create(size_t capacity)
{
    Table t = {0};
    t.capacity = capacity;
    t.length = 0;
    t.entires = calloc(capacity, sizeof(Entry));

    return t;
};

void table_insert(Table *t, const char *key, const int value)
{
    uint64_t hash = fnv1a_hash(key, strlen(key));
    size_t index = hash & (t->capacity - 1);

    Entry new_entry = entry_create(key, value);
    size_t len = 0;
    while (1)
    {
        Entry *cur = &t->entires[index];
        if (!cur->used)
        {
            *cur = new_entry;
            t->length++;
            return;
        }

        if (strcmp(cur->key, key) == 0)
        {
            cur->value = value;
        }

        if (len >= t->capacity)
        {
            return;
        }

        index = (index + 1) & (t->capacity - 1);
    }
}

Entry *table_get(Table *t, const char *key)
{
    uint64_t hash = fnv1a_hash(key, strlen(key));
    size_t index = hash & (t->capacity - 1);

    size_t len = 0;
    while (1)
    {
        Entry *e = &t->entires[index];
        if (!e->used)
            return NULL;
        if (strcmp(e->key, key) == 0)
            return e;
        if (len++ >= t->capacity)
            return NULL;

        index = (index + 1) & (t->capacity - 1);
    }
}
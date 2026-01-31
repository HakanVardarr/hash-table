#include "table.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

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

Entry entry_create(const void *key, size_t key_size, const void *value, size_t value_size)
{
    Entry e = {0};
    e.key = malloc(key_size);
    e.key_size = key_size;
    memcpy(e.key, key, key_size);
    e.value = malloc(value_size);
    e.value_size = value_size;
    memcpy(e.value, value, value_size);
    e.used = 1;
    e.probe_len = 0;
    return e;
}

Table table_create(size_t capacity)
{
    assert((capacity & (capacity - 1)) == 0 && "capacity must be power of two");

    Table t = {0};
    t.capacity = capacity;
    t.length = 0;
    t.entires = calloc(capacity, sizeof(Entry));

    return t;
};

int key_eq_mem(const void *a, size_t a_size,
               const void *b, size_t b_size)
{
    if (a_size != b_size)
        return 0;

    return memcmp(a, b, a_size) == 0;
}

void table_insert(Table *t, const void *key, size_t key_size, const void *value, size_t value_size)
{
    uint64_t hash = fnv1a_hash(key, key_size);
    size_t index = hash & (t->capacity - 1);

    Entry new_entry = entry_create(key, key_size, value, value_size);
    while (1)
    {
        Entry *cur = &t->entires[index];
        if (!cur->used)
        {
            *cur = new_entry;
            t->length++;
            return;
        }

        if (key_eq_mem(key, key_size, cur->key, cur->key_size))
        {
            void *new_val = malloc(value_size);
            memcpy(new_val, value, value_size);

            free(cur->value);
            cur->value = new_val;
            cur->value_size = value_size;
            return;
        }

        if (new_entry.probe_len > cur->probe_len)
        {
            Entry tmp = *cur;
            *cur = new_entry;
            new_entry = tmp;
        }

        if (new_entry.probe_len > t->capacity)
        {
            assert(0 && "hash table full");
        }

        index = (index + 1) & (t->capacity - 1);
        new_entry.probe_len++;
    }
}

Entry *table_get(Table *t, const char *key, size_t key_size)
{
    uint64_t hash = fnv1a_hash(key, key_size);
    size_t index = hash & (t->capacity - 1);

    size_t probe = 0;
    while (1)
    {
        Entry *e = &t->entires[index];

        if (!e->used)
            return NULL;

        if (key_eq_mem(key, key_size, e->key, e->key_size))
            return e;

        if (probe > e->probe_len)
            return NULL;

        index = (index + 1) & (t->capacity - 1);
        probe++;
    }
}
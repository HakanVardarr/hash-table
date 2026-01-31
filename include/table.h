#ifndef __TABLE_H
#define __TABLE_H

typedef unsigned long size_t;

typedef struct
{
    void *key;
    size_t key_size;

    void *value;
    size_t value_size;

    int used;
    size_t probe_len;
} Entry;

Entry entry_create(const void *key, size_t key_size, const void *value, size_t value_size);

typedef struct
{
    Entry *entires;
    size_t capacity;
    size_t length;

} Table;

Table table_create(size_t capacity);
void table_insert(Table *t, const void *key, size_t key_size, const void *value, size_t value_size);
Entry *table_get(Table *t, const char *key, size_t key_size);

#endif // __TABLE_H
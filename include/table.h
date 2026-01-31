#ifndef __TABLE_H
#define __TABLE_H

typedef unsigned long size_t;

typedef struct
{
    const char *key;
    int value;
    int used;
} Entry;

Entry entry_create(const char *key, int value);

typedef struct
{
    Entry *entires;
    size_t capacity;
    size_t length;

} Table;

Table table_create(size_t capacity);
void table_insert(Table *t, const char *key, const int value);
Entry *table_get(Table *t, const char *key);

#endif // __TABLE_H
#include "table.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    Table t = table_create(1024);

    const char *key = "Hello";
    int value = 10;
    table_insert(&t, key, strlen(key), &value, sizeof(int));

    const char *needle = "Hello";
    Entry *e = table_get(&t, needle, strlen(needle));
    if (e)
    {
        printf("%d\n", *(int *)e->value);
    }

    return 0;
}
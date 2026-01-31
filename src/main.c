#include "table.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    Table t = table_create(16);

    const char *key = "Hello";
    const int value = 10;
    table_insert(&t, key, value);

    const char *needle = "Hello";
    Entry *e = table_get(&t, needle);
    if (e)
    {
        printf("%d\n", e->value);
    }

    return 0;
}
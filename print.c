#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "chash.h"
#include "print.h"

// Insert a new record. Prints result to stdout.
void print(){
    printf("Current Database:\n");
    hashRecord *cur = hash_table;
    while (cur != NULL) {
        printf("%u,%s,%u\n", cur->hash, cur->name, cur->salary);
        cur = cur->next;
    }
    return;
}
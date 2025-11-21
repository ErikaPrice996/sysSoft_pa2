#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "chash.h"

void search(Command *cmd, uint32_t hashedName){
    // search
    hashRecord *cur = hash_table;
    int nameFound = 0;

    // Check if list is empty
    if (cur == NULL) {
        printf("%s not found.\n", cmd->name);
        return;
    }

    while (cur != NULL) {
        if (cur->hash == hashedName) {
            // found
            nameFound = 1;
            printf("Found: %u,%s,%u\n", cur->hash, cur->name, cur->salary);
            break;
        }
        cur = cur->next;
    }

    // if not found
    if (!nameFound)
    {
        printf("Entry %u not found. Not in database.\n", hashedName);
    }
}
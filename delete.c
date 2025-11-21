#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "chash.h"
#include "delete.h"

// Delete a record. Prints result to stdout.
void delete_hash(Command *cmd, int thread_id, uint32_t hashedName){
    // search
    hashRecord *cur = hash_table;
    int nameFound = 0;
    hashRecord *toDelete = NULL;

    if (cur->hash == hashedName) {
        // found at the head of the list
        nameFound = 2;
        // get it ready to free
        toDelete = hash_table;
        // remove it
        hash_table = cur->next;
    }

    while (cur->next != NULL && !nameFound) {
        if (cur->next->hash == hashedName) {
            // found (cur->next = the found one to delete)
            nameFound = 1;
            // get ready to delete it
            toDelete = cur->next;
            // remove it from the list
            cur->next = cur->next->next;
        }
        cur = cur->next;
    }

    // if not found
    if (!nameFound)
    {
        printf("Entry %u not deleted. Not in database.\n", hashedName);
        return;
    }

    // if found, free memory
    free(toDelete);

    printf("Deleted record for %u,%s,%u\n", hashedName, cmd->name, cmd->salary);
}
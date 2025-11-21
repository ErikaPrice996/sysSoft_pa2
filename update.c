#include <stdio.h>
#include <stdlib.h>
#include "chash.h"
#include "update.h"

// Update an existing record. Prints result to stdout.
void update(Command *cmd, uint32_t hashedName){
    // Check for hash in the bucket's linked list
    hashRecord *cur = hash_table;
    int found = 0;

    while (cur != NULL) {
        if(cur->hash == hashedName) {
            found = 1;
            uint32_t oldSalary = cur->salary;

            // Perform the update
            cur->salary = cmd->salary;
            
            // Print output
            printf("Updated record %u from %u,%s,%u to %u,%s,%u\n", hashedName, hashedName, cur->name, oldSalary, hashedName, cur->name, cur->salary);

            break;
        }
        cur = cur->next;
    }

    // If entry not found, print failure output
    if (!found){
        printf("Update failed.  Entry %u not found.\n", hashedName);
    }

}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "chash.h"
#include "insert.h"

// Insert a new record. Prints result to stdout.
void insert(Command *cmd, int thread_id, uint32_t hashedName){
    //Check for duplicate name in the bucket's linked list
    hashRecord *cur = hash_table;
    while (cur != NULL) {
        if(cur->hash == hashedName) {
            printf("Duplicate entry for name %s. Insertion aborted.\n", cmd->name);
            return;
        }
        cur = cur->next;
    }
    
    //Create and populate new node 
    hashRecord *newRecord = (hashRecord *)malloc(sizeof(hashRecord));
    newRecord->hash = hashedName;
    strcpy(newRecord->name, cmd->name);
    newRecord->salary = cmd->salary;

    // Insert at head of list 
    newRecord->next = hash_table;
    hash_table = newRecord;

    printf("Inserted %u,%s,%u\n", hashedName, cmd->name, cmd->salary);
}
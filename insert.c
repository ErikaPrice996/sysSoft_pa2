#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "chash.h"

// Insert a new record. Prints result to stdout.
void insert(Command *cmd, int thread_id, unsigned int hashedName)
{
    printf("%lld: THREAD %d INSERT,%s,%u\n", current_timestamp(), thread_id, cmd->name, cmd->salary);

    //Check for duplicate name in the bucket's linked list

    //Create and populate new node 

    // Insert at head of list 

    // Release lock 

    printf("Inserted %d,%s,%.2f\n", hashedName, cmd->name, cmd->salary);
}
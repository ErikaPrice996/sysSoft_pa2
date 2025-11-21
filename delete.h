#ifndef DELETE_H
#define DELETE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "chash.h"

/* Function prototypes */
void delete_hash(Command *cmd, int thread_id, uint32_t hashedName);

#endif
#ifndef INSERT_H
#define INSERT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "chash.h"

/* Function prototypes */
void insert(Command *cmd, int thread_id, unsigned int hashedName);

#endif
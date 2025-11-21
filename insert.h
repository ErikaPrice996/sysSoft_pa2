#ifndef INSERT_H
#define INSERT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "chash.h"

/* Function prototypes */
void insert(Command *cmd, uint32_t hashedName);

#endif
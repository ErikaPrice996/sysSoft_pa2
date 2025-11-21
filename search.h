#ifndef SEARCH_H
#define SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "chash.h"

/* Function prototypes */
void search(Command *cmd, uint32_t hashedName);

#endif
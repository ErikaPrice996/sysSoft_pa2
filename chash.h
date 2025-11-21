#ifndef CHASH_H
#define CHASH_H

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

#define MAX_NAME_LENGTH 50
#define MAX_COMMAND_LENGTH 100
#define MAX_HASH_SIZE 1024

extern FILE *logfile;

typedef struct {
    char command[MAX_COMMAND_LENGTH];
    char name[MAX_NAME_LENGTH];
    uint32_t salary;
    int priority;
} Command;

typedef struct hash_struct {
    uint32_t hash;
    char name[50];
    uint32_t salary;
    struct hash_struct *next;
} hashRecord;

extern hashRecord *hash_table;

typedef struct {
    Command *commands;
    int command_count;
    pthread_mutex_t turn_mutex;      /* Protects current_turn and ordering */
    pthread_cond_t cond;             /* Condition variable for thread ordering */
    pthread_rwlock_t hash_lock;      /* Reader-writer lock for hash table */
    int current_turn;                /* Tracks which thread should execute next */
} CommandList;

typedef struct {
    CommandList *cmd_list;
    int thread_id;
} ThreadArgs;

/* Function prototypes */
long long current_timestamp(void);

uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length);

CommandList parse_commands(const char *filename);

void *execute_command(void *arg);

#endif
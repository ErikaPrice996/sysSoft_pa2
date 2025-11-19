#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>

#define MAX_NAME_LENGTH 50
#define MAX_COMMAND_LENGTH 100

FILE *logfile;

typedef struct {
    char command[MAX_COMMAND_LENGTH];
    char name[MAX_NAME_LENGTH];
    uint32_t salary;
    int priority;
} Command;

typedef struct {
    Command *commands;
    int command_count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int current_turn;
} CommandList;

typedef struct {
    CommandList *cmd_list;
    int thread_id;
} ThreadArgs;

typedef struct hash_struct
{
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *next;
} hashRecord;

long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long microseconds = (te.tv_sec * 1000000LL) + te.tv_usec; // calculate microseconds
    return microseconds;
}

void *execute_command(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    CommandList *cmd_list = args->cmd_list;
    int thread_id = args->thread_id;
    free(args);

    Command cmd;

    // Lock before waiting
    pthread_mutex_lock(&cmd_list->mutex);

    // Wait for this thread's turn
    while (cmd_list->current_turn < thread_id) {
        fprintf(logfile, "%lld: THREAD %d WAITING FOR MY TURN\n", current_timestamp(), thread_id);
        fflush(logfile);
        pthread_cond_wait(&cmd_list->cond, &cmd_list->mutex);
    }

    fprintf(logfile, "%lld: THREAD %d AWAKENED FOR WORK\n", current_timestamp(), thread_id);
    fflush(logfile);
    
    // Get the command for this thread
    if (thread_id < cmd_list->command_count) {
        cmd = cmd_list->commands[thread_id];
    }
    
    // Increment turn for next thread and broadcast
    cmd_list->current_turn++;
    pthread_cond_broadcast(&cmd_list->cond);
    
    // Unlock before executing command
    pthread_mutex_unlock(&cmd_list->mutex);

    if (strncmp(cmd.command, "insert", 6) == 0) {
        //get lock
        printf("%lld: THREAD %d INSERT,%s,%u\n", current_timestamp(), thread_id, cmd.name, cmd.salary);
        fprintf(logfile, "%lld: THREAD %d WRITE LOCK ACQUIRED\n", current_timestamp(), thread_id);
        fflush(logfile);

        // Perform insert operation on hash table here
        
        
        fprintf(logfile, "%lld: THREAD %d WRITE LOCK RELEASED\n", current_timestamp(), thread_id);
        fflush(logfile);
        //release lock
    } 
    else if (strncmp(cmd.command, "delete", 6) == 0) {
        //get lock
        printf("%lld: THREAD %d DELETE,%s\n", current_timestamp(), thread_id, cmd.name);
        fprintf(logfile, "%lld: THREAD %d WRITE LOCK ACQUIRED\n", current_timestamp(), thread_id);
        fflush(logfile);

        // Perform delete operation on hash table here
        
        fprintf(logfile, "%lld: THREAD %d WRITE LOCK RELEASED\n", current_timestamp(), thread_id);
        fflush(logfile);
        //release lock
    } 
    else if (strncmp(cmd.command, "search", 6) == 0) {
        //get lock
        printf("%lld: THREAD %d SEARCH,%s\n", current_timestamp(), thread_id, cmd.name);
        fprintf(logfile, "%lld: THREAD %d READ LOCK ACQUIRED\n", current_timestamp(), thread_id);
        fflush(logfile);

        // Perform search operation on hash table here
        
        fprintf(logfile, "%lld: THREAD %d READ LOCK RELEASED\n", current_timestamp(), thread_id);
        fflush(logfile);
        //release lock
    } 
    else if (strncmp(cmd.command, "print", 5) == 0) {
        //get lock
        printf("%lld: THREAD %d PRINT\n", current_timestamp(), thread_id);
        fprintf(logfile, "%lld: THREAD %d READ LOCK ACQUIRED\n", current_timestamp(), thread_id);
        fflush(logfile);

        // Perform print operation on hash table here
        
        fprintf(logfile, "%lld: THREAD %d READ LOCK RELEASED\n", current_timestamp(), thread_id);
        fflush(logfile);
        //release lock
    } 
    else if (strncmp(cmd.command, "update", 6) == 0) {
        //get lock
        printf("%lld: THREAD %d UPDATE,%s,%u\n", current_timestamp(), thread_id, cmd.name, cmd.salary);
        fprintf(logfile, "%lld: THREAD %d WRITE LOCK ACQUIRED\n", current_timestamp(), thread_id);
        fflush(logfile);

        // Perform update operation on hash table here

        fprintf(logfile, "%lld: THREAD %d WRITE LOCK RELEASED\n", current_timestamp(), thread_id);
        fflush(logfile);
        //release lock
    }
    
    return NULL;
}

CommandList parse_commands(const char *filename) {
    CommandList cmd_list;
    cmd_list.command_count = 0;
    cmd_list.commands = NULL;
    cmd_list.current_turn = 0;
    pthread_mutex_init(&cmd_list.mutex, NULL);
    pthread_cond_init(&cmd_list.cond, NULL);

    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open commands.txt");
        exit(EXIT_FAILURE);
    }

    char line[MAX_COMMAND_LENGTH];
    // Read the first line to get the number of threads
    if (fgets(line, sizeof(line), file)) {
        sscanf(line, "threads,%d", &cmd_list.command_count);
        cmd_list.commands = malloc(cmd_list.command_count * sizeof(Command));
    }

    // Read the rest of the commands
    int index = 0;
    while (fgets(line, sizeof(line), file) && index < cmd_list.command_count) {
        char *token = strtok(line, ",");
        if (token) {
            strcpy(cmd_list.commands[index].command, token);
            token = strtok(NULL, ",");
            if (token) {
                strcpy(cmd_list.commands[index].name, token);
                token = strtok(NULL, ",");
                if (token) {
                    cmd_list.commands[index].salary = atoi(token);
                    token = strtok(NULL, ",");
                    if (token) {
                        cmd_list.commands[index].priority = atoi(token);
                    }
                } 
            }
            index++;
        }
    }

    fclose(file);
    return cmd_list;
}

uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length) {
  size_t i = 0;
  uint32_t hash = 0;
  while (i != length) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

int main() {
    logfile = fopen("hash.log", "w");
    if (!logfile) {
        perror("Failed to open hash.log");
        exit(EXIT_FAILURE);
    }

    CommandList cmd_list = parse_commands("commands.txt");
    pthread_t *threads = malloc(cmd_list.command_count * sizeof(pthread_t));

    // Create threads for each command
    for (int i = 0; i < cmd_list.command_count; i++) {
        ThreadArgs *args = malloc(sizeof(ThreadArgs));
        args->cmd_list = &cmd_list;
        args->thread_id = i;
        pthread_create(&threads[i], NULL, execute_command, (void *)args);
    }

    // Join threads
    for (int i = 0; i < cmd_list.command_count; i++) {
        pthread_join(threads[i], NULL);
    }

    // Clean up
    free(cmd_list.commands);
    free(threads);
    pthread_mutex_destroy(&cmd_list.mutex);
    pthread_cond_destroy(&cmd_list.cond);
    fclose(logfile);
    
    return 0;
}
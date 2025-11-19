#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>

#define MAX_NAME_LENGTH 50
#define MAX_COMMAND_LENGTH 100

typedef struct {
    char command[MAX_COMMAND_LENGTH];
    char name[MAX_NAME_LENGTH];
    uint32_t salary;
} Command;

typedef struct {
    Command *commands;
    int command_count;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int inserts_done;
} CommandList;

long long current_timestamp() {
    struct timeval te;
    gettimeofday(&te, NULL); // get current time
    long long microseconds = (te.tv_sec * 1000000LL) + te.tv_usec; // calculate microseconds
    return microseconds;
}

void *execute_command(void *arg) {
    CommandList *cmd_list = (CommandList *)arg;
    Command cmd;
    
    // Lock the mutex to safely access the command list
    pthread_mutex_lock(&cmd_list->mutex);
    
    // Get the command to execute
    cmd = cmd_list->commands[cmd_list->inserts_done];
    cmd_list->inserts_done++;
    
    // Unlock the mutex after accessing the command
    pthread_mutex_unlock(&cmd_list->mutex);
    
    long long timestamp = current_timestamp();
    
    if (strncmp(cmd.command, "insert", 6) == 0) {
        printf("%lld: INSERT,%s,%u\n", timestamp, cmd.name, cmd.salary);
        // Call the insert function here
    } else if (strncmp(cmd.command, "delete", 6) == 0) {
        // Wait until all inserts are done before executing delete
        pthread_mutex_lock(&cmd_list->mutex);
        while (cmd_list->inserts_done < cmd_list->command_count) {
            pthread_cond_wait(&cmd_list->cond, &cmd_list->mutex);
        }
        pthread_mutex_unlock(&cmd_list->mutex);
        
        printf("%lld: DELETE,%s\n", timestamp, cmd.name);
        // Call the delete function here
    } else if (strncmp(cmd.command, "search", 6) == 0) {
        printf("%lld: SEARCH,%s\n", timestamp, cmd.name);
        // Call the search function here
    } else if (strncmp(cmd.command, "print", 5) == 0) {
        printf("%lld: PRINT\n", timestamp);
        // Call the print function here
    }
    
    return NULL;
}

CommandList parse_commands(const char *filename) {
    CommandList cmd_list;
    cmd_list.command_count = 0;
    cmd_list.commands = NULL;
    cmd_list.inserts_done = 0;
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
                } else {
                    cmd_list.commands[index].salary = 0; // Default salary for delete and print
                }
            }
            index++;
        }
    }

    fclose(file);
    return cmd_list;
}

int main() {
    CommandList cmd_list = parse_commands("commands.txt");
    pthread_t *threads = malloc(cmd_list.command_count * sizeof(pthread_t));

    // Create threads for each command
    for (int i = 0; i < cmd_list.command_count; i++) {
        pthread_create(&threads[i], NULL, execute_command, (void *)&cmd_list);
    }

    // Join threads
    for (int i = 0; i < cmd_list.command_count; i++) {
        pthread_join(threads[i], NULL);
    }

    // Signal that all inserts are done
    pthread_mutex_lock(&cmd_list.mutex);
    cmd_list.inserts_done = cmd_list.command_count; // All inserts are done
    pthread_cond_broadcast(&cmd_list.cond); // Wake up any waiting deletes
    pthread_mutex_unlock(&cmd_list.mutex);

    // Clean up
    free(cmd_list.commands);
    free(threads);
    pthread_mutex_destroy(&cmd_list.mutex);
    pthread_cond_destroy(&cmd_list.cond);
    
    return 0;
}
// chash.c
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "hashdb.h"
#include "rwlocks.h"
#include "common.h"
#include "common_threads.h"

extern hashRecord *hashTable[1000];

int main() {
    // Initialize the hash table
    for (int i = 0; i < 1000; i++) {
        hashTable[i] = NULL;
    }

    // Read the commands.txt file and process the commands
    FILE *file = fopen("sample_input.txt", "r");
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        char *command = strtok(line, ",");
        char *name = strtok(NULL, ",");
        char *salary = strtok(NULL, ",");

        if (strcmp(command, "insert") == 0) {
            insert(name, atoi(salary));
        } else if (strcmp(command, "delete") == 0) {
            delete(name);
        } else if (strcmp(command, "search") == 0) {
            hashRecord *record = search(name);
            if (record != NULL) {
                printf("Found %s with salary %d\n", record->name, record->salary);
            } else {
                printf("%s not found\n", name);
            }
        } else if (strcmp(command, "print") == 0) {
            print();
        }
    }

    fclose(file);
    return 0;
}

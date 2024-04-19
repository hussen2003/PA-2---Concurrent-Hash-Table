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
    // Open the output.txt file
    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Initialize the hash table
    for (int i = 0; i < 1000; i++) {
        hashTable[i] = NULL;
    }

    // Read the commands.txt file and process the commands
    FILE *file = fopen("sample_input.txt", "r");
    char line[256];
    int lockAcquisitions = 0;
    int lockReleases = 0;

    while (fgets(line, sizeof(line), file)) {
        char *command = strtok(line, ",");
        char *name = strtok(NULL, ",");
        char *salary = strtok(NULL, ",");
        int num_threads = 0;

        if (strcmp(command, "threads") == 0) {
            num_threads = atoi(name); 
            fprintf(outputFile, "Running %d threads\n", num_threads);
        }

        if (strcmp(command, "insert") == 0) {
            fprintf(outputFile, "INSERT,%s,%s", name, salary);
            insert(name, atoi(salary), outputFile);
            lockAcquisitions++;
        } else if (strcmp(command, "delete") == 0) {
            fprintf(outputFile, "DELETE,%s\n", name);
            delete(name, outputFile);
            lockReleases++;
        } else if (strcmp(command, "search") == 0) {
            fprintf(outputFile, "SEARCH,%s\n", name);
            hashRecord *record = search(name, outputFile);
            if (record != NULL) {
                fprintf(outputFile, "%d,%s,%d\n", record->hash, record->name, record->salary);
            } else {
                fprintf(outputFile, "%s not found\n", name);
            }
        } else if (strcmp(command, "print") == 0) {
            fprintf(outputFile, "PRINT\n");
            print(outputFile);
        }
    }

    //fprintf(outputFile, "\nNumber of lock acquisitions: %d\n", lockAcquisitions);
    //fprintf(outputFile, "Number of lock releases: %d\n", lockReleases);
    fprintf(outputFile, "Final Table:\n");
    printFinal(outputFile);

    fclose(file);
    fclose(outputFile);  // Close the output.txt file
    return 0;
}

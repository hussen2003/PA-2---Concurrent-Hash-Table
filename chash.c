// chash.c
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "hashdb.h"
#include "rwlocks.h"
#include "common.h"
#include "common_threads.h"

extern hashRecord *hashTable;  

int main() {
    FILE *outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    hashTable = NULL;  

    //Read the commands.txt file and process the commands
    FILE *file = fopen("commands.txt", "r");
    char line[256];
    int lockAcquistions = 0;
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
            uint32_t hash = hash_function(name);
            fprintf(outputFile, "INSERT,%u,%s,%s",hash, name, salary);
            insert(name, atoi(salary), outputFile);
            lockAcquistions++;
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
            print(outputFile);
        }
    }

    //fprintf(outputFile, "\nNumber of lock acquisitions: %d\n", lockAcquistions);
    //fprintf(outputFile, "Number of lock releases: %d\n", lockReleases);
    fprintf(outputFile, "\n");
    printFinal(outputFile);

    fclose(file);
    fclose(outputFile);  // Close the output.txt file
    return 0;
}

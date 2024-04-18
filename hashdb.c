//including your Jenkins function and all linked list operations
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "common_threads.h"
#include "rwlocks.h"
#include <string.h>
#include "hashdb.h"

typedef struct hash_struct
{
  uint32_t hash;
  char name[50];
  uint32_t salary;
  struct hash_struct *next;
} hashRecord;

rwlock_t lock;
hashRecord *hashTable;
int hashSize;
int numThread;
int numAcquire;
int numRelease;


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

void insert(char *name, uint32_t salary)
{
    uint32_t hashValue = jenkins_one_at_a_time_hash((const uint8_t *)name, strlen(name));
    rwlock_acquire_writelock(&lock);
    numAcquire++;
    
    // Asked Claude 2, Can you finish writing the insert implementation for the concurrent hash table
    // Check if the key already exists in the hash table
    hashRecord *curr = hashTable[hashValue % hashSize];
    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) {
            // Key already exists, update the salary
            curr->salary = salary;
            rwlock_release_writelock(&lock);
            numRelease++;
            return;
        }
        curr = curr->next;
    }
    
    // Key does not exist, create a new record and insert it
    hashRecord *newRecord = (hashRecord *)malloc(sizeof(hashRecord));
    strncpy(newRecord->name, name, 49);
    newRecord->name[49] = '\0'; // Ensure null termination
    newRecord->salary = salary;
    newRecord->hash = hashValue;
    newRecord->next = hashTable[hashValue % hashSize];
    hashTable[hashValue % hashSize] = newRecord;
    

    rwlock_release_writelock(&lock);
    numRelease++;
}


void delete(char *name)
{
    uint32_t hashValue = jenkins_one_at_a_time_hash((const uint8_t *)name, strlen(name));
    rwlock_acquire_writelock(&lock);
    numAcquire++;

    // Asked Claude 2, now can you write the delete implementation
    // Find the record in the linked list
    hashRecord *prev = NULL;
    hashRecord *curr = hashTable[hashValue % hashSize];
    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) {
            // Remove the record from the linked list
            if (prev == NULL) {
                hashTable[hashValue % hashSize] = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            rwlock_release_writelock(&lock);
            numRelease++;
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    rwlock_release_writelock(&lock);
    numRelease++;
}

void search(char *name)
{
    uint32_t hashValue = jenkins_one_at_a_time_hash((const uint8_t *)name, strlen(name));
    rwlock_acquire_writelock(&lock);
    numAcquire++;
    
    //Asked Claude 2, "Now can you write the search implementation"
    // Search for the record in the linked list
    hashRecord *curr = hashTable[hashValue % hashSize];
    while (curr != NULL) {
        if (strcmp(curr->name, name) == 0) {
            printf("Found record: Hash: %d Name: %s, Salary: %u\n", curr->hash, curr->name, curr->salary);
            rwlock_release_readlock(&lock);
            numRelease++;
            return;
        }
        curr = curr->next;
    }

    rwlock_release_writelock(&lock);
    numRelease++;
}

void print() 
{
    uint32_t hashValue = jenkins_one_at_a_time_hash((const uint8_t *)name, strlen(name));
    rwlock_acquire_writelock(&lock);
    numAcquire++;
    //Asked Claude 2 now can you write the print implementation, it should be in this format, 36474636,John Doe,45000 (hash value, name, salary)

    for (int i = 0; i < hashSize; i++) {
        hashRecord *curr = hashTable[i];
        while (curr != NULL) {
            printf("%u,%s,%u\n", curr->hash, curr->name, curr->salary);
            curr = curr->next;
        }
    }

    rwlock_release_writelock(&lock);
    numRelease++;
}

//Asked Claude 2: 
//Can you also write a final print function that prints something like the following: (final print example given)
void final_print() {
    rwlock_acquire_readlock(&lock);
    numAcquire++;

    printf("Number of lock acquisitions: %d\n", numAcquire);
    printf("Number of lock releases: %d\n", numRelease);
    printf("Final Table:\n");

    for (int i = 0; i < hashSize; i++) {
        hashRecord *curr = hashTable[i];
        while (curr != NULL) {
            printf("%u,%s,%u\n", curr->hash, curr->name, curr->salary);
            curr = curr->next;
        }
    }

    rwlock_release_readlock(&lock);
    numRelease++;
}

//Using Claude, can you write C code that reads in sample file with the following input and seperates all the values into three seperate variables (Commands, Name, and Salary): (sample input given to us)
int main() {
    FILE *file = fopen("sample_input.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    char line[100];
    char command[20], name[50];
    int salary;

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        // Remove the newline character
        line[strcspn(line, "\n")] = '\0';

        if (sscanf(line, "%[^,],%[^,],%d", command, name, &salary) == 3) {
            if (strcmp(command, "insert") == 0) {
                insert(name, salary);
            } else if (strcmp(command, "delete") == 0) {
                delete(name);
            } else if (strcmp(command, "search") == 0) {
                search(name);
            } else if (strcmp(command, "print") == 0) {
                print();
            } else {
                printf("Invalid command: %s\n", command);
            }
        } else {
            printf("Invalid line: %s\n", line);
        }
    }

    fclose(file);
    final_print();
    return 0;
}
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "hashdb.h"


hashRecord *hashTable[1000]; // Assuming the size of the hash table is 1000
pthread_rwlock_t rwlock;

int num_acquistions = 0;
int num_releases = 0;

uint32_t hash_function(char *str) {
    const uint8_t* key = (const uint8_t *)str;
    size_t length = strlen(str);
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
    return hash % 1000;  // Modulo operation to fit the hash table size
}


void insert(char *name, uint32_t salary, FILE *file) {
    fprintf(file, "WRITE LOCK ACQUIRED\n");
    pthread_rwlock_wrlock(&rwlock);  // Lock the mutex before inserting
    num_acquistions++;
    uint32_t index = hash_function(name);
    hashRecord *newnode = malloc(sizeof(hashRecord));
    strcpy(newnode->name, name);
    newnode->salary = salary;
    newnode->next = NULL;

    if (hashTable[index] == NULL) {
        hashTable[index] = newnode;
    } else {
        // Add the node at the end of the chain
        hashRecord *current = hashTable[index];
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newnode;
    }

    pthread_rwlock_unlock(&rwlock);  // Unlock the mutex after inserting
    num_releases++;
    fprintf(file, "WRITE LOCK RELEASED\n");
}

void delete(char *name, FILE *file) {
    fprintf(file, "WRITE LOCK ACQUIRED\n");
    pthread_rwlock_wrlock(&rwlock);  // Acquire a write lock
    num_acquistions++;
    uint32_t index = hash_function(name);
    hashRecord *temp = hashTable[index], *prev;

    if (temp != NULL && strcmp(temp->name, name) == 0) {
        hashTable[index] = temp->next;
        free(temp);
    } else {
        while (temp != NULL && strcmp(temp->name, name) != 0) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == NULL) {
            fprintf(file, "%s not found\n", name);
        } else {
            prev->next = temp->next;
            free(temp);
        }
    }

    pthread_rwlock_unlock(&rwlock);  // Release the write lock
    num_releases++;
    fprintf(file, "WRITE LOCK RELEASED\n");
}


hashRecord* search(char *name, FILE *file) {
    fprintf(file, "READ LOCK ACQUIRED\n");
    num_acquistions++;
    pthread_rwlock_rdlock(&rwlock);
    int index = hash_function(name);
    hashRecord *current = hashTable[index];

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            break;
        }
        current = current->next;
    }
    pthread_rwlock_unlock(&rwlock);  // Release the read lock
    fprintf(file, "READ LOCK RELEASED\n");
    num_releases++;
    return current;
}


void print(FILE *file) {
    fprintf(file, "READ LOCK ACQUIRED\n");
    num_acquistions++;
    pthread_rwlock_rdlock(&rwlock);
    for (int i = 0; i < 1000; i++) {
        hashRecord *current = hashTable[i];
        while (current != NULL) {
            fprintf(file, "%d, %s, %d\n", current->hash, current->name, current->salary);
            current = current->next;
        }
    }
    pthread_rwlock_unlock(&rwlock);  // Release the read lock
    fprintf(file, "READ LOCK RELEASED\n");
    num_releases++;
}


void printFinal(FILE *file) {
    fprintf(file, "READ LOCK ACQUIRED\n");
    num_acquistions++;
    pthread_rwlock_rdlock(&rwlock);
    num_releases++;
    fprintf(file, "Number of lock acquisitions: %d\n", num_acquistions);
    fprintf(file, "Number of lock releases: %d\n", num_releases);
    for (int i = 0; i < 1000; i++) {
        hashRecord *current = hashTable[i];
        while (current != NULL) {
            fprintf(file, "%u, %s, %d\n", current->hash,current->name, current->salary);
            current = current->next;
        }
    }
    pthread_rwlock_unlock(&rwlock);  // Release the read lock
    fprintf(file, "READ LOCK RELEASED\n");
}


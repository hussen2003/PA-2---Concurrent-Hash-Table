#include "hashdb.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



hashRecord *hashTable[1000]; // Assuming the size of the hash table is 1000
pthread_rwlock_t rwlock;

int num_acquistions = 0;
int num_releases = 0;

uint32_t hash_function(char *str) {
    uint32_t hash = 0;
    int c;

    while (c = *str++)
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash % 1000;
}


void insert(char *name, uint32_t salary) {
    printf("WRITE LOCK ACQUIRED\n");
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
    printf("WRITE LOCK RELEASED\n");
    num_releases++;
}



void delete(char *name) {
    printf("WRITE LOCK ACQUIRED\n");
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
            printf("%s not found\n", name);
        } else {
            prev->next = temp->next;
            free(temp);
        }
    }

    pthread_rwlock_unlock(&rwlock);  // Release the write lock
    printf("WRITE LOCK RELEASED\n");
    num_releases++;
}


hashRecord* search(char *name) {
    printf("READ LOCK ACQUIRED\n");
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
    printf("READ LOCK RELEASED\n");
    num_releases++;
    return current;
}


void print() {
    printf("READ LOCK ACQUIRED\n");
    num_acquistions++;
    for (int i = 0; i < 1000; i++) {
        hashRecord *current = hashTable[i];
        while (current != NULL) {
            printf("%d, %s, %d\n", current->hash, current->name, current->salary);
            current = current->next;
        }
    }
    printf("READ LOCK RELEASED\n");
    num_releases++;
}


void printfinal() {
    printf("Number of lock acqusitions: %d \n", num_acquistions);
    printf("Number of lock releases: %d \n", num_releases);
    for (int i = 0; i < 1000; i++) {
        hashRecord *current = hashTable[i];
        while (current != NULL) {
            printf("%d, %s, %d\n", current->hash, current->name, current->salary);
            current = current->next;
        }
    }
}

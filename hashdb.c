#include "hashdb.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



hashRecord *hashTable[1000]; // Assuming the size of the hash table is 1000
pthread_mutex_t lock;

uint32_t hash_function(char *str) {
    uint32_t hash = 0;
    int c;

    while (c = *str++)
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash % 1000;
}

void insert(char *name, uint32_t salary) {
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
}

void delete(char *name) {
    int index = hash_function(name);
    hashRecord *temp = hashTable[index], *prev;

    if (temp != NULL && strcmp(temp->name, name) == 0) {
        hashTable[index] = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && strcmp(temp->name, name) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

    prev->next = temp->next;
    free(temp);
}

hashRecord* search(char *name) {
    int index = hash_function(name);
    hashRecord *current = hashTable[index];

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void print() {
    for (int i = 0; i < 1000; i++) {
        hashRecord *current = hashTable[i];
        while (current != NULL) {
            printf("%s, %d\n", current->name, current->salary);
            current = current->next;
        }
    }
}

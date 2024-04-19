#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "hashdb.h"


hashRecord *hashTable = NULL; 
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
    //printf("\n  %u --> %s", hash, str);
    return hash;  
}

void insert(char *name, uint32_t salary, FILE *file) {
    fprintf(file, "WRITE LOCK ACQUIRED\n");
    pthread_rwlock_wrlock(&rwlock); 
    num_acquistions++;
    uint32_t hash = hash_function(name);
    hashRecord* temp = hashTable;
    hashRecord *newnode = malloc(sizeof(hashRecord));
    newnode->hash = hash;
    strcpy(newnode->name, name);
    newnode->salary = salary;
    newnode->next = NULL;
    if(temp == NULL)
    {
        hashTable = newnode;
    }
    else
    {
        newnode->next = hashTable; 
        hashTable = newnode; 
    }
    pthread_rwlock_unlock(&rwlock); 
    num_releases++;
    fprintf(file, "WRITE LOCK RELEASED\n");
}


void delete(char *name, FILE *file) {
    fprintf(file, "WRITE LOCK ACQUIRED\n");
    pthread_rwlock_wrlock(&rwlock);  
    num_acquistions++;
    hashRecord *temp = hashTable, *prev = NULL;

    while (temp != NULL && strcmp(temp->name, name) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        fprintf(file, "%s not found\n", name);
    } else {
        if (prev == NULL) {
            hashTable = temp->next;
        } else {
            prev->next = temp->next;
        }
        free(temp);
    }

    pthread_rwlock_unlock(&rwlock); 
    num_releases++;
    fprintf(file, "WRITE LOCK RELEASED\n");
}

hashRecord* search(char *name, FILE *file) {
    fprintf(file, "READ LOCK ACQUIRED\n");
    num_acquistions++;
    pthread_rwlock_rdlock(&rwlock);
    hashRecord *current = hashTable;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            break;
        }
        current = current->next;
    }

    pthread_rwlock_unlock(&rwlock);  
    fprintf(file, "READ LOCK RELEASED\n");
    num_releases++;
    return current;
}

void print(FILE *file) {
    fprintf(file, "READ LOCK ACQUIRED\n");
    num_acquistions++;
    pthread_rwlock_rdlock(&rwlock);
    hashRecord *current = hashTable;
    while (current != NULL) {
        fprintf(file, "%u, %s, %d\n", current->hash, current->name, current->salary);
        current = current->next;
    }
    pthread_rwlock_unlock(&rwlock);  
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
    hashRecord *current = hashTable;
    while (current != NULL) {
        fprintf(file, "%u, %s, %d\n", current->hash,current->name, current->salary);
        current = current->next;
    }
    pthread_rwlock_unlock(&rwlock);  
    fprintf(file, "READ LOCK RELEASED\n");
}



//Concurrent Hash Table struct definitions
//Concurrent Hash Table implementation

#ifndef HASHDB_H
#define HASHDB_H

#include <stdint.h>

typedef struct hash_struct {
    uint32_t hash;
    char name[50];
    uint32_t salary;
    struct hash_struct *next;
} hashRecord;

uint32_t hash_function(char *str);
void insert(char *name, uint32_t salary);
void delete(char *name);
hashRecord* search(char *name);
void print();
void printfinal();

#endif /* HASHDB_H */

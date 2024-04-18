//Concurrent Hash Table struct definitions
//Concurrent Hash Table implementation
#ifndef HASHDB_H
#define HASHDB_H

#include <stdint.h>

typedef struct hash_record {
    uint32_t hash;
    char name[50];
    uint32_t salary;
    struct hash_record* next;
} hash_record_t;

extern pthread rwlock_t lock;
extern hashRecord* hashTable;
extern int hashSize;
extern int numThread;



void insert(char *name, uint32_t salary);
void delete(char* name);
void search(char* name);
void print();
void final_print();
uint32_t jenkins_one_at_a_time_hash(const uint8_t* key, size_t length);

#endif // HASHDB_H
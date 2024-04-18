//including your Jenkins function and all linked list operations
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "common_threads.h"
#include "rwlock.h"
#include <string.h>

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

    //insert programming stuff here

    rwlock_release_writelock(&lock);
    numRelease++;
}


void delete(char *name)
{
    uint32_t hashValue = jenkins_one_at_a_time_hash((const uint8_t *)name, strlen(name));
    rwlock_acquire_writelock(&lock);
    numAcquire++;

    //delete programming stuff here

    rwlock_release_writelock(&lock);
    numRelease++;
}

void search(char *name)
{
    uint32_t hashValue = jenkins_one_at_a_time_hash((const uint8_t *)name, strlen(name));
    rwlock_acquire_writelock(&lock);
    numAcquire++;

    //search programming stuff here

    rwlock_release_writelock(&lock);
    numRelease++;
}

void print() 
{
    uint32_t hashValue = jenkins_one_at_a_time_hash((const uint8_t *)name, strlen(name));
    rwlock_acquire_writelock(&lock);
    numAcquire++;

    //print programming stuff here

    rwlock_release_writelock(&lock);
    numRelease++;
}
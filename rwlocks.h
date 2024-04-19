#include <pthread.h>
#include <semaphore.h>

typedef struct {
    sem_t read_lock;
    sem_t write_lock;
    int readers;
} rwlock_t;

void init_rwlock(rwlock_t *rw);
void rlock(rwlock_t *rw);
void runlock(rwlock_t *rw);
void wlock(rwlock_t *rw);
void wunlock(rwlock_t *rw);

// rwlocks.c
#include "rwlocks.h"

void init_rwlock(rwlock_t *rw) {
    sem_init(&rw->read_lock, 0, 1);
    sem_init(&rw->write_lock, 0, 1);
    rw->readers = 0;
}

void rlock(rwlock_t *rw) {
    sem_wait(&rw->read_lock);
    rw->readers++;
    if (rw->readers == 1) {
        sem_wait(&rw->write_lock);
    }
    sem_post(&rw->read_lock);
}

void runlock(rwlock_t *rw) {
    sem_wait(&rw->read_lock);
    rw->readers--;
    if (rw->readers == 0) {
        sem_post(&rw->write_lock);
    }
    sem_post(&rw->read_lock);
}

void wlock(rwlock_t *rw) {
    sem_wait(&rw->write_lock);
}

void wunlock(rwlock_t *rw) {
    sem_post(&rw->write_lock);
}

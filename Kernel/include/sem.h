#ifndef SEM_H
#define SEM_H

#define SEM_NAME_MAX_LEN 200


#include <processManager.h>

typedef struct sem_t{
    char name[SEM_NAME_MAX_LEN];
    uint16_t value;
    uint8_t lock;
    uint16_t openings;
    uint8_t blocked_processes[MAX_PROCESS_COUNT];
    uint16_t blocked_size;
    struct sem_t *next;
} sem_t;

typedef struct sem_list{
    uint16_t size;
    uint8_t lock;
    struct sem_t* first;
    struct sem_t* last;
} sem_list;

/**
*@brief     If existent, opens the desired semaphore. If not it creates one.
*@param[in] name Constant null terminated string of the desired semaphore.
*@return    Desired semaphore.
*@note      If the name is longer than 200 chars, returns NULL.
*/
sem_t *sem_open(const char *name, uint16_t value);

/**
*@brief     Closes the desired semaphore.
*@param[in] sem Semaphore to close.
*@return    0 if successful, -1 if semaphore not existent.
*/
int sem_close(sem_t *sem);

/**
*@brief     Increments/unlocks the desired semaphore.
*@param[in] sem Semaphore.
*@return    0 if successful, -1 if semaphore not existent.
*/
int sem_post(sem_t *sem);

/**
*@brief     Decrements/locks the desired semaphore.
*@param[in] sem Semaphore to close.
*@return    0 if successful, -1 if semaphore not existent.
*/
int sem_wait(sem_t *sem);

/**
*@brief     Looks for the target PID in all semaphores, and erases any participation.  To be used
*           when deleting processes.
*@param[in] pid_to_delete Process ID to delete.
*/
void delete_pid_from_sems(pid_t pid_to_delete);

/**
*@brief     Wakes all sleeping processes of a process.
*@param[in] sem Target semaphore.
*/
void release_pids(sem_t * sem);

/*----------------------
  | ASM functions
  -----------------------*/

/**
*@brief     Locks the desired semaphore.
*@param[in] lock Semaphore's lock.
*/
void sem_lock_wait(uint8_t * lock);

/**
*@brief     Unlocks the desired semaphore.
*@param[in] lock Semaphore's lock.
*/
void sem_lock_post(uint8_t * lock);


#endif //SEM_H

#include <mman.h>
#include <sem.h>
#include <lib.h>

sem_list list = {0, 0, NULL, NULL};

sem_t *search_by_name(sem_t* current, const char *name);
sem_t *delete_sem(sem_t *current, sem_t *sem);
int check_valid_sem(sem_t *sem);
pid_t get_pid_to_unblock(sem_t *sem);

sem_t *sem_open(const char *name, uint16_t value){
    if(k_strlen(name)>SEM_NAME_MAX_LEN) return NULL;

    sem_t *aux_sem;
    sem_lock_wait(&(list.lock));
    if(list.size){
        aux_sem = search_by_name(list.first, name);
        if(aux_sem){
            sem_lock_wait(&(aux_sem->lock));
            (aux_sem->openings)++;
            sem_lock_post(&(aux_sem->lock));
            sem_lock_post(&(list.lock));
            return aux_sem;
        }
    }
    aux_sem = (sem_t *) my_malloc(sizeof(sem_t));
    if(!aux_sem){
        sem_lock_post(&(list.lock));
        return NULL;
    }
    k_strcpy(aux_sem->name, name);
    aux_sem->value=value;
    aux_sem->lock=0;
    aux_sem->next=NULL;
    aux_sem->openings=1;
    for (int i = 0; i < MAX_PROCESS_COUNT; i++) {
        aux_sem->blocked_processes[i] = 0;
    }
    aux_sem->blocked_size=0;
    if(list.last) list.last->next = aux_sem;
    list.last = aux_sem;
    if(!list.first) list.first=list.last;
    list.size++;
    sem_lock_post(&(list.lock));
    return aux_sem;
}

int sem_close(sem_t *sem){
    if(check_valid_sem(sem)) return -1;
    sem_lock_wait(&(list.lock));
    sem_lock_wait(&(sem->lock));
    sem->openings--;
    if(!sem->openings){
        list.first = delete_sem(list.first, sem);
    }
    else sem_lock_post(&(sem->lock));
    sem_lock_post(&(list.lock));
    return 0;
}

int sem_post(sem_t *sem){
    int unblocked = 0;
    pid_t pid_to_unblock;
    if(!check_valid_sem(sem)) return -1;
    sem_lock_wait(&(sem->lock));
    if(!sem->value && sem->blocked_size){
            (sem->blocked_size)--;
            pid_to_unblock = get_pid_to_unblock(sem);
            sem->blocked_processes[pid_to_unblock] = 0;
            unblocked = 1;
    }
    else sem->value++;
    sem_lock_post(&(sem->lock));
    if (unblocked) unblock_process(pid_to_unblock);
    return 0;
}

int sem_wait(sem_t *sem){
    int blocked = 0;
    if(!check_valid_sem(sem)) return -1;
    sem_lock_wait(&(sem->lock));
    if(sem->value) sem->value--;
    else{
        sem->blocked_processes[get_pid()] = 1;
        (sem->blocked_size)++;
        blocked = 1;
        sem_lock_post(&(sem->lock));
    }
    sem_lock_post(&(sem->lock));
    if (blocked) block_process(get_pid());
    return 0;
}


void delete_pid_from_sems(pid_t pid_to_delete){
    sem_lock_wait(&(list.lock));
    if(!list.first){
        sem_lock_post(&(list.lock));
        return;
    }
    sem_t aux_placeholder = {"", 1, 0, 0, {0}, 0, list.first};
    sem_t *current = &aux_placeholder;
    while(current->next){
        sem_lock_wait(&(current->next->lock));
        if((current->next)->blocked_size && (current->next)->blocked_processes[pid_to_delete]){
            (current->next)->blocked_processes[pid_to_delete]=0;
            current->next->openings--;//a blocked process had the semaphore opened
            if(!current->next->openings){
                list.first = delete_sem(list.first, current->next);
            }
        }
        sem_lock_post(&(current->next->lock));
        current = current->next;
    }
    sem_lock_post(&(list.lock));
}

/*----------------------
  | Local functions
  -----------------------*/
/**
*@brief     Checks that the semaphore exists.
*@param[in] sem Desired name to look for.
*@return    1 if successful, 0 upon failure.
*/
int check_valid_sem(sem_t *sem){
    sem_lock_wait(&(list.lock));
    if(!list.size){
        sem_lock_post(&(list.lock));
        return 0;
    }
    sem_t *aux = search_by_name(list.first, sem->name);
    sem_lock_post(&(list.lock));
    return sem == aux;
}

/**
*@brief     Runs through the semaphores looking for a matching name to return that semaphore.
*@param[in] current First semaphore of a list.
*@param[in] name Desired name to look for.
*@return    Desired semaphore.
*@note      If not found, returns NULL.
*/
sem_t *search_by_name(sem_t *current, const char *name){
    if(k_strcmp(current->name, name)==0) return current;
    if(current->next) return search_by_name(current->next, name);
    return NULL;
}

/**
*@brief     Runs through the semaphores looking for a matching name to delete that semaphore.
*@param[in] current First semaphore of a list.
*@param[in] name Desired name to look for.
*@return    First semaphore of a list.
*/
sem_t *delete_sem(sem_t *current, sem_t *sem){
    if(current==sem){
        sem_t *aux_sem = current->next;
        my_free(current);
        (list.size)--;
        if(!list.size) list.last = NULL;
        return aux_sem;
    }
    if(current->next){
        if(!current->next->next){
            if(current->next==sem){
                my_free(current->next);
                (list.size)--;
                current->next = NULL;
                list.last = current;
                return current;
            }
        }
        current->next = delete_sem(current->next, sem);
    }
    return current;
}

/**
*@brief     Runs through the blocked processes by the semaphore.
*@param[in] sem Current semaphore.
*@return    Process ID to unblock.
*@note      Shouldn't happen, but if returns 0 it didnt found any blocked pid.
*/
pid_t get_pid_to_unblock(sem_t *sem){
    pid_t found = 0;
    int i = 0;
    while(!found && i<MAX_PROCESS_COUNT){
        if(sem->blocked_processes[i]) found = i;
        i++;
    }
    return found;
}


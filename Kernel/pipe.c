#include <mman.h>
#include <pipe.h>
#include <sem.h>
#include <lib.h>

#define MAX_FD 1000
#define PIPE_BUFF 65535

#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef struct pipe_t{
    uint16_t read_fd;
    uint16_t write_fd;
    char buffer[PIPE_BUFF];
    uint16_t to_read_idx;
    uint16_t to_write_idx;
    sem_t *sem_to_read;
    sem_t *sem_to_write;
    sem_t *sem_mutex;
} pipe_t;

typedef enum pipe_action{
    READ,
    WRITE
}pipe_action;

uint16_t highest_fd = STDERR+1;

pipe_t * pipes[MAX_FD/2]={NULL};//tODO los primeros 2 naonao

pipe_t *create_pipe(int read_fd, int write_fd);
int delete_pipe(int fd);
pipe_t * check_valid_fd(uint16_t fd, pipe_action action);
pipe_t *search_pipe(uint16_t fd);
void set_pipe_name(int num, char str[]);

int pipe(int pipefd[2]){
    if(highest_fd+2 > MAX_FD) return -1;
    pipe_t *aux_pipe = create_pipe(highest_fd+1, highest_fd+2);
    if(!aux_pipe) return -1;
    pipes[(highest_fd+2)/2] = aux_pipe;
    pipefd[0]=++highest_fd;
    pipefd[1]=++highest_fd;
    return 0;
}

int read_pipe(int fd, char *buffer, uint16_t amount){
    pipe_t *aux_pipe = check_valid_fd(fd, READ);
    if(!aux_pipe) return -1;
    if(!aux_pipe->write_fd && aux_pipe->to_read_idx==aux_pipe->to_write_idx) return 0;

    int amount_read = 0;
    int done = 0;

    while(amount_read < amount && !done){
        sem_wait(aux_pipe->sem_to_read);
        sem_wait(aux_pipe->sem_mutex);
        buffer[amount_read++] = aux_pipe->buffer[aux_pipe->to_read_idx];
        aux_pipe->to_read_idx = (aux_pipe->to_read_idx+1)%PIPE_BUFF;

        done = aux_pipe->to_read_idx == aux_pipe->to_write_idx;

        sem_post(aux_pipe->sem_mutex);
        sem_post(aux_pipe->sem_to_write);
    }
    return amount_read;
}

int write_pipe(int fd, char *message, uint16_t length){
    pipe_t *aux_pipe = check_valid_fd(fd, WRITE);
    if(!aux_pipe) return -1;
    if(!aux_pipe->read_fd) return 0;

    int amount_written = 0;
    while(amount_written < length){
        sem_wait(aux_pipe->sem_to_write);
        sem_wait(aux_pipe->sem_mutex);
        aux_pipe->buffer[aux_pipe->to_write_idx] = message[amount_written++];
        aux_pipe->to_write_idx = (aux_pipe->to_write_idx+1)%PIPE_BUFF;
        sem_post(aux_pipe->sem_mutex);
        sem_post(aux_pipe->sem_to_read);
    }

    return amount_written;
}

int pclose(int fd){
    pipe_t * aux_pipe = search_pipe(fd);
    if(!aux_pipe) return -1;
    if(aux_pipe->read_fd==fd){
        if(!aux_pipe->write_fd){
            if(delete_pipe(fd)==-1) return -1;
        }
        else aux_pipe->read_fd=0;
    }
    else{
        if(!aux_pipe->read_fd){
            if(delete_pipe(fd)==-1) return -1;
        }
        else aux_pipe->write_fd=0;
    }
    return 0;
}

/*----------------------
  | Local functions
  -----------------------*/

/**
*@brief     Creates a new pipe.
*@param[in] read_fd Read-end for the new pipe.
*@param[in] write_fd Write-end for the new pipe.
*@return    Pointer to the created pipe.
*/
pipe_t *create_pipe(int read_fd, int write_fd){
    pipe_t *aux_pipe = (pipe_t *) my_malloc(sizeof(pipe_t));
    if(!aux_pipe) return NULL;
    aux_pipe->read_fd = read_fd;
    aux_pipe->write_fd = write_fd;
    aux_pipe->to_read_idx = 0;
    aux_pipe->to_write_idx = 0;
    char aux_pipe_name[20] = "pipe_0000";
    set_pipe_name(read_fd/2, aux_pipe_name);
    k_strcpy(aux_pipe_name + 8, "_to_read");
    aux_pipe->sem_to_read = sem_open(aux_pipe_name, 0);
    if(!aux_pipe->sem_to_read) return NULL;
    k_strcpy(aux_pipe_name + 8, "_to_write");
    aux_pipe->sem_to_write = sem_open(aux_pipe_name, PIPE_BUFF);
    if(!aux_pipe->sem_to_write) return NULL;
    k_strcpy(aux_pipe_name + 8, "_mutex");
    aux_pipe->sem_mutex = sem_open(aux_pipe_name, 1);
    if(!aux_pipe->sem_mutex) return NULL;
    return aux_pipe;
}

/**
*@brief     Deletes a pipe.
*@param[in] fd One of the file descriptors of the desired pipe.
*@return    0 on success, -1 upon failure.
*/
int delete_pipe(int fd){
    pipe_t *aux_pipe = pipes[fd/2];
    if(sem_close(aux_pipe->sem_to_read)==-1) return -1;
    if(sem_close(aux_pipe->sem_to_write)==-1) return -1;
    if(sem_close(aux_pipe->sem_mutex)==-1) return -1;
    my_free(aux_pipe);
    pipes[fd/2] = NULL;
    return 0;
}

/**
*@brief     Verifies that a pipe exists and the file descriptor is for the action.
*@param[in] fd Desired file descriptor.
*@param[in] action Action to be made with this fd.
*@return    Pointer to the desired pipe, NULL upon failure.
*/
pipe_t * check_valid_fd(uint16_t fd, pipe_action action){
    pipe_t * aux_pipe = search_pipe(fd);
    if(!aux_pipe) return NULL;

    switch(action){
        case READ:
            if(fd==aux_pipe->read_fd) return aux_pipe;
            return NULL;

        case WRITE:
            if(fd==aux_pipe->write_fd) return aux_pipe;
            return NULL;

        default: return NULL;
    }
}

/**
*@brief     Looks for the desired pipe.
*@param[in] fd One of the file descriptors of the desired pipe.
*@return    Pointer to the desired pipe, NULL upon failure.
*/
pipe_t *search_pipe(uint16_t fd){
    if(fd > highest_fd) return NULL;
    return pipes[fd/2];
}

/**
*@brief     Creates names for the semaphores.
*@param[in] num ID for the pipe.
*@param[in] str String to change.
*/
void set_pipe_name(int num, char str[]){
    int digit1 = num/1000;
    int digit2 = (num/100) % 10;
    int digit3 = (num/10) % 100;
    int digit4 = num % 1000;

    str[5] = '0' + digit1;
    str[6] = '0' + digit2;
    str[7] = '0' + digit3;
    str[8] = '0' + digit4;
}

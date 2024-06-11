#include "include/userlibasm.h"
#include "include/userlib.h"
#include "include/phylo.h"
#include "include/rand.h"
#include <stddef.h>

#define PHYLO_MAX   30
#define PHYLO_MIN   3
#define PHYLO_INIT  5

#define THINKING    0
#define HUNGRY      1
#define EATING      2

typedef struct Phylo{
    int state;
    sem_t * sem;
    pid_t pid;
}Phylo;

int phylo_amount = 0;

boolean new_state = FALSE;

Phylo phylo_table[PHYLO_MAX]={0};

sem_t *mutex = NULL;

void init_phylos();
void show_phylo_table();
void remove_phylo();
int add_phylo();
void test(int phylo);
void put_forks(int phylo);
void take_forks(int phylo);
void phylos(int argc, char *argv[]);
void eat();
void think();

void set_phylo_name(int num, char str[]);
int get_phylo_num(char str[]);

void think(){
    //sleep(rand()%8, 0);
    sleep(1,0);
}

void eat(){
    //sleep(rand()%5, 0);
    sleep(1,0);
}

void phylos(int argc, char *argv[]){
    pid_t my_pid = get_pid();
    int num = get_phylo_num(argv[0] + 6);
    phylo_table[num].pid = my_pid;
    while(1){
        think();
        take_forks(num);
        eat();
        put_forks(num);
    }
}

void take_forks(int phylo){
    sem_down(mutex);
    phylo_table[phylo].state = HUNGRY;
    test(phylo);
    sem_up(mutex);
    sem_down(phylo_table[phylo].sem);
}

void put_forks(int phylo){
    sem_down(mutex);
    phylo_table[phylo].state = THINKING;
    test((phylo+phylo_amount-1)%phylo_amount);
    test((phylo+1)%phylo_amount);
    sem_up(mutex);
}

void test(int phylo){
    if ((phylo_table[phylo].state == HUNGRY) && (phylo_table[(phylo+phylo_amount-1)%phylo_amount].state != EATING) && (phylo_table[(phylo+1)%phylo_amount].state != EATING)) {
        phylo_table[phylo].state = EATING;
        new_state=TRUE;
        sem_up(phylo_table[phylo].sem);
    }
}

int add_phylo(){
    int aux_phylo_num = 0;
    char aux_phylo_name[20] = {0};
    if(phylo_amount<PHYLO_MAX){
        aux_phylo_num = phylo_amount;
        phylo_table[aux_phylo_num].state = THINKING;
        strcpy(aux_phylo_name, "phylo_00");
        set_phylo_name(aux_phylo_num, aux_phylo_name);
        phylo_table[aux_phylo_num].sem = sem_open(aux_phylo_name, 1);
        if(!phylo_table[aux_phylo_num].sem){
            print("Error opening semaphore in add_phylo\n");
            return -1;
        }
    }
    else print("Max amount of phylos reached\n");
    
    int aux_argc = 1;
    char * name = strdup(aux_phylo_name);
    if(!name){
        print("Error allocating mem add_phylo\n");
        sem_close(phylo_table[aux_phylo_num].sem);
        phylo_table[aux_phylo_num].sem=NULL;
        return -1;
    }
    char ** aux_argv = (char **) my_malloc(sizeof(char *) * aux_argc);
    if(!aux_argv){
        print("Error allocating mem add_phylo\n");
        my_free(name);
        sem_close(phylo_table[aux_phylo_num].sem);
        phylo_table[aux_phylo_num].sem=NULL;
        return -1;
    }
    aux_argv[0] = name;
    print("new phylo!\n");
    create_shiny_process(&phylos, aux_argc, aux_argv, 4, FALSE, FALSE, 0, 0);
    phylo_amount++;
    
    return 0;
}

void remove_phylo(){
    uint8_t removed = 0;
    if(phylo_amount>PHYLO_MIN){
        sem_close(phylo_table[phylo_amount].sem);
        phylo_table[phylo_amount].sem=NULL;
        phylo_amount--;
        removed = 1;
        kill(phylo_table[phylo_amount+1].pid);
    }
    if(!removed) print("Min amount of phylos reached\n");
}

void phylo_command(int argc, char **argv){
    srand(time());
    char *c = " ";
    init_phylos();
    while(1){
        if(peek_read_pipe()){
            if (read(c, 1) == 1){
                switch (c[0]){
                    case 'a':
                        sem_down(mutex);
                        add_phylo();
                        sem_up(mutex);
                        break;
                    case 'r':
                        sem_down(mutex);
                        remove_phylo();
                        sem_up(mutex);
                        break;
                    default:
                        print("heyo\n");
                        break;
                }
            }
        }
        sem_down(mutex);
        if(new_state==TRUE){
            show_phylo_table();
            new_state=FALSE;
        } 
        sem_up(mutex);
        sleep(1,0);
    }
}

void init_phylos(){
    sem_close(mutex);
    mutex = sem_open("phylo_sem", 1);
    phylo_amount = 0;
    sem_down(mutex);
    for(int i=0; i<PHYLO_INIT; i++){
        add_phylo();
    }
    sem_up(mutex);
}

void show_phylo_table(){
    for(int i=0; i<phylo_amount; i++){
        if(phylo_table[i].state==EATING) print("E ");
        else print(". ");
    }
    print("\n");
}

void set_phylo_name(int num, char str[]){
    int digit1 = num/10;
    int digit2 = num % 10;

    str[6] = '0' + digit1;
    str[7] = '0' + digit2;
}

int get_phylo_num(char str[]){
    int to_ret = str[1]-'0';
    to_ret += (str[0]-'0')*10;
    return to_ret;
}
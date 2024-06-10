
#include "include/shell.h"
#include "include/userlibasm.h"
#include "include/snake.h"
#include "include/userlib.h"
#include <stdint.h>
#include <stddef.h>

#define COMMANDS 23 //AGREGUE UNO TODO: VOLVER A 15
extern char endOfBinary;//ESTO TMB
extern char bss;//ESTO TMB
#define VERT_SIZE 32
#define LINE_SIZE 63
#define BUFFER_SIZE 128

// Esto es un "string" manual para poder imprimir el caracter 128 de nuestro font de kernel usando lsa funciones estandar
char PROMPT_START[] = {127, 0};
#define ERROR_PROMPT "Unknown command: "

// Buffers
char screen_buffer[VERT_SIZE][LINE_SIZE];
char command_buffer[BUFFER_SIZE];
static char* commands[COMMANDS] = {"exit", "clear", "inc-size", "dec-size", "time", "sleep", "infoSleep", "help", "milisleep", "nanosleep", "registers", "snake", "test-div", "test-invalid", "speak", "mem", "malloc", "free", "ps", "bt", "pipe", "cproc", "sexo"};
char char_buffer[1];

//cproc test stuf TODO SACAR Y MODULAR BIEN!
void dummy_process(){
    write_out("im dummy\n");
}

//TODO ps stuff
void ps(void);
void sexo(int argc, char **argv);

char * get_process_status(State state){
    switch(state){
        case READY:
            return "READY";
        case RUNNING:
            return "RUNNING";
        case BLOCKED:
            return "BLOCKED";
        default:
            return "UNKOWN";
    }
}

char * get_process_foreground(uint8_t foreground){
    if(foreground==TRUE) return "YES";
    else return "NO";
}

void ps(){
    uint16_t process_amount = 0;
    ProcessView ** processes  = get_processes(&process_amount);
    char aux_aux[BUFFER_SIZE];
    if(!processes){
        write_out("No processes\n");
        write_out("amount:");
        uintToBase(process_amount, aux_aux, 10);
        write_out(aux_aux);
        write_out("\n");
        return;
    }
    write_out("PID\t | STATE\t | PRIORITY\t | RSP\t | RBP\t | RIP\t | FOREGROUND\t | PARENT\t | CHILDREN\n");
    char aux[BUFFER_SIZE];
    for(int i = 0; i<process_amount; i++){
        uintToBase(processes[i]->pid, aux, 10);
        write_out(aux);
        write_out("\t |");
        write_out(get_process_status(processes[i]->state));
        write_out("\t |");
        uintToBase(processes[i]->priority, aux, 10);
        write_out(aux);
        write_out("\t |");
        uintToBase(processes[i]->registers.rsp, aux, 16);
        write_out(aux);
        write_out("\t |");
        uintToBase(processes[i]->registers.rbp, aux, 16);
        write_out(aux);
        write_out("\t |");
        uintToBase(processes[i]->registers.rip, aux, 16);
        write_out(aux);
        write_out("\t |");
        write_out(get_process_foreground(processes[i]->foreground));
        write_out("\t |");
        uintToBase(processes[i]->fatherPid, aux, 10);
        write_out(aux);
        write_out("\t |");
        uintToBase(processes[i]->children_amount, aux, 10);
        write_out(aux);
        write_out("\n");

        my_free(processes[i]);
    }
    my_free(processes);
}

//TODO end of  ps stuff

typedef enum {
    EXIT,
    CLEAR,
    INC_SIZE,
    DEC_SIZE,
    TIME,
    SLEEP,
    INFO_SLEEP,
    HELP,
    MILISLEEP,
    NANOSLEEP,
    REGISTERS,
    SNAKE,
    TEST_DIV,
    TEST_INVALID,
    SPEAK,
    MEM,
    MALLOC,
    FREE,
    PS,
    BT,
    PIPE,
    CPROC,
    SEXO,
} COMMAND_TYPE;


// Cursors & flags
int command_cursor = 0;
int cursor_y;
int cursor_x;
int exit_command;

// Important values
uint8_t font_size;
int rows_to_show;
int limit_index = VERT_SIZE - 1;
int line_size = LINE_SIZE;

int hrs = 0;
int min = 0; 
int sec = 0;

char aux[128];
int cantRegs = 18;
uint64_t regs[18];
char* regsNames[18] = {"rax:", "rbx:", "rcx:", "rdx:", "rsi:", "rdi:", "rbp:", "rsp:", "r8:", "r9:",
                       "r10:", "r11:", "r12:", "r13:", "r14:", "r15:", "rip:", "rflags:"};

                       
uint64_t aux_mem_state[3];
char byteUnit[2]={0};
int * aux_mem_pointer = NULL;

pid_t the_shell(){
    return create_process(&shell, 0, NULL);
}

int shell(){
    cursor_x = 0;
    cursor_y = 0;
    exit_command = 0;

    clearScreen();
    init_shell();
    write_out(PROMPT_START);


    while(!exit_command){
        if (read(char_buffer, 1) == 1){
            process_key(char_buffer[0]);
        }
        //sleep_once();
    }

    //clearScreen();
    //exit();
    return 0;
}


void process_key(char key){
    if (key == '\n'){

        command_buffer[command_cursor] = '\0';

        write_out("\n");
        process_command(command_buffer);

        command_cursor = 0;
        
        write_out(PROMPT_START);
        return;
    }

    if (key == '\b'){
        if (!command_cursor)
            return;

        command_cursor--;
        cursor_x = mod(cursor_x - 1, line_size);

        // aca va printChar y no write_out porq es un caso especial
        printChar(key);
        return;
    }

    // a partir de aca si esta lleno el buffer nos vamos
    if (command_cursor == BUFFER_SIZE - 1) 
        return;

    else {
        command_buffer[command_cursor++] = key;
        write_out(char_buffer);
    }
}

void process_command(char* buffer){
   if (buffer[0] == '\0'){
        return;
    }

   for(int i = 0; i < COMMANDS; i++){
        if (!strcmp(buffer, commands[i])){
            switch (i) {
                case 0:
                    exit_command = 1;
                    write_out("Bye now! Hope you enjoyed your stay!");
                    //sleep(2, 0);
                    exit();         /*OJO AL PIOJO ESTE EXIT ES EXITANTE*/
                    break;
                case 1:
                    clearScreen(); 
                    cursor_y = 0;
                    cursor_x = 0;
                    limit_index = VERT_SIZE/font_size - 1;
                    break;
                case 2:
                    if (font_size == 2){
                        write_out("Font size max!\n");
                    }
                    else {
                        change_font(++font_size);
                        resize();
                    }
                    break;
                
                case 3:
                    if (font_size == 1){
                        write_out("Font size minimum!\n");
                    }
                    else {
                        change_font(--font_size);
                        desize();
                    }
                    break;
                case 4:
                    getClock(&hrs, &min, &sec);
                    write_out("La hora es: ");
                    uintToBase(hrs, aux, 10);
                    if(hrs<10){
                        write_out("0");
                        write_out(aux);
                    }
                    else{
                        write_out(aux);
                    }
                    write_out(":");
                    uintToBase(min, aux, 10);
                    if(min<10){
                        write_out("0");
                        write_out(aux);
                    }
                    else{
                        write_out(aux);
                    }
                    write_out(":");
                    uintToBase(sec, aux, 10);
                    if(sec<10){
                        write_out("0");
                        write_out(aux);
                    }
                    else{
                        write_out(aux);
                    }
                    write_out("\n");
                    break;
                case 5:
                    write_out("Vamos a esperar 4 segundos... ");
                    sleep(4, 0);
                    write_out("Listo!\n");
                    break;
                case 6:
                    write_out("El comando sleep efectuara una espera de 4 segundos para demostrar el funcionamiento de la syscall. Los comandos milisleep y nanosleep haran algo equivalente pero con sus respectivas unidades\n");
                    break;
                case 7:
                    write_out("Los comandos existentes son:\n");
                    for(int i=0; i<COMMANDS; i++){
                        write_out(commands[i]);
                        write_out("\n");
                    }
                    break;
                
                case 8:
                    sleep(3000, 1);
                    break;

                case 9:
                    sleep(3000000, 2);
                    break;

                case 10:
                    if(getRegs(regs)==0){
                        write_out("Antes de pedir los registros debe apretar la tecla alt izquierda para que los mismos se guarden\n");
                    }

                    else{
                        for(int i=0; i<cantRegs; i++){
                            if (i != cantRegs - 1)
                                write_out("-");
                            write_out(regsNames[i]);
                            uintToBase(regs[i], aux, 10);
                            write_out(aux);
                            write_out("\n");
                        }
                    }
                    break;
         
                case 11:
                    Snake();
                    cursor_y = 0;
                    cursor_x = 0;
                    limit_index = VERT_SIZE/font_size - 1;
                    flushBuffer();
                    clearScreen();
                    break;

                case 12:
                    write_out("Vamos a testear dividir 1 por 0 en:\n");
                    write_out("3...\n");
                    //sleep(1, 0);
                    write_out("2...\n");
                    //sleep(1, 0);
                    write_out("1...\n");
                    //sleep(1, 0);
                    int a = 1;
                    int b = 0;
                    if (a/b ==1)
                        write_out("You really shouldnt be here chief... medio que rompiste la matematica\n");
                    break;

                case 13:
                    write_out("Vamos a tratar de desafiar al runtime de asm en:\n");
                    write_out("3...\n");
                    sleep(1, 0);
                    write_out("2...\n");
                    sleep(1, 0);
                    write_out("1...\n");
                    sleep(1, 0);
                    _opError();    
                    break;

                case 14:
                    beep(1000, 50);
                    break;

                case 15://TODO: AYUDAME LOCO
                    write_out("endOfBinary: ");
                    uintToBase((uint64_t) &endOfBinary, aux, 16);
                    write_out(aux);
                    write_out("\n");
                    write_out("bss: ");
                    uintToBase((uint64_t) &bss, aux, 16);
                    write_out(aux);
                    write_out("\n");
                    getMemState(aux_mem_state);
                    write_out("Total: ");
                    //byteUnit[0] = byteConverter(aux_mem_state);
                    uintToBase(aux_mem_state[0], aux, 10);
                    write_out(aux);
                    //write_out(byteUnit);
                    write_out("B\n");
                    write_out("Disponible: ");
                    //byteUnit[0] = byteConverter(aux_mem_state+1);
                    uintToBase(aux_mem_state[1], aux, 10);
                    write_out(aux);
                    //write_out(byteUnit);
                    write_out("B\n");
                    write_out("Ocupada: ");
                    //byteUnit[0] = byteConverter(aux_mem_state+2);
                    uintToBase(aux_mem_state[2], aux, 10);
                    write_out(aux);
                    //write_out(byteUnit);
                    write_out("B\n");
                    break;

                case 16:
                    write_out("Probamos malloc(sizeof(int))\n");
                    if(aux_mem_pointer) write_out("Eu, acordate del free antes, no?\n");
                    else{
                        getMemState(aux_mem_state);
                        write_out("Pre-Ocupada: ");
                        uintToBase(aux_mem_state[2], aux, 10);
                        write_out(aux);
                        write_out("B\n");
                        aux_mem_pointer=(int *) my_malloc(sizeof(int));
                        if(!aux_mem_pointer) write_out("No pudo guardar la memoria :(\n");
                        else{
                            *(aux_mem_pointer)=777;
                            write_out("aux_mem_pointer = ");
                            uintToBase((uint64_t) aux_mem_pointer, aux, 10);
                            write_out(aux);
                            write_out("\n");
                            write_out("*(aux_mem_pointer) = ");
                            uintToBase(*aux_mem_pointer, aux, 10);
                            write_out(aux);
                            write_out("\n");
                            getMemState(aux_mem_state);
                            write_out("Post-Ocupada: ");
                            uintToBase(aux_mem_state[2], aux, 10);
                            write_out(aux);
                            write_out("B\n");
                        }
                    }
                    break;

                case 17:
                    write_out("Probamos free(sizeof(int)\n");
                    if(!aux_mem_pointer) write_out("Eu, acordate del malloc primero, no?\n");
                    else{
                        getMemState(aux_mem_state);
                        write_out("Pre-Ocupada: ");
                        uintToBase(aux_mem_state[2], aux, 10);
                        write_out(aux);
                        write_out("B\n");
                        my_free(aux_mem_pointer);
                        getMemState(aux_mem_state);
                        write_out("Post-Ocupada: ");
                        uintToBase(aux_mem_state[2], aux, 10);
                        write_out(aux);
                        write_out("B\n");
                        aux_mem_pointer=NULL;//reset
                    }
                    break;

                case 18:
                    write_out("Probamos ps\n");
                    getMemState(aux_mem_state);
                    write_out("Pre-Ocupada: ");
                    uintToBase(aux_mem_state[2], aux, 10);
                    write_out(aux);
                    write_out("B\n");
                    ps();
                    getMemState(aux_mem_state);
                    write_out("Post-Ocupada: ");
                    uintToBase(aux_mem_state[2], aux, 10);
                    write_out(aux);
                    write_out("B\n");
                    break;

                case 19:
                    write_out("Probamos bloquear : )\n");
                    write_out("Abrimos sem\n");
                    sem_t * my_sem = sem_open("my_sem", 0);
                    if(!my_sem){
                        write_out("algo salio mal\n");
                        break;
                    }
                    write_out("up(sem)\n");
                    sem_up(my_sem);
                    write_out("down(sem)\n");
                    sem_down(my_sem);
                    write_out("down(sem)\n");
                    sem_down(my_sem);
                    write_out("????\n");
                    break;
                case 20:
                    write_out("Probamos pipes (literalmente anda a saber)\n");
                    int aux_fds[2];
                    if(pipe(aux_fds)){
                        write_out("salio mal\n");
                        break;
                    }
                    if(write(aux_fds[1], "holi\0", strlen("holi\0"))<0){
                        write_out("nao write\n");
                        break;
                    }
                    if(read_fd(aux_fds[0], aux, 128)<=0){
                        write_out("nao read\n");
                        break;
                    }

                    write_out(aux);
                    write_out("\n");

                    write_out("before blocks");
                    if(read_fd(aux_fds[0], aux, 128)<=0){
                        write_out("nao read\n");
                        break;
                    }
                    write_out(aux);
                    write_out("\n");
                    break;

                case 21:
                    write_out("Probamos crear un proceso\n");
                    create_process(&dummy_process, 0, NULL);
                    break;
                case 22:
                    int aux_argc = 1;
                    char *aux_text = "5";
                    char *aux_argv[aux_argc];
                    aux_argv[0] = aux_text;
                    sexo(aux_argc, aux_argv);
            }   
            return;
        }
    }

    if (strlen(buffer) == BUFFER_SIZE){
        write_out("Buenas... una poesia?\n");
        return;
    }

    // En caso de no encontrar hacemos esto
    cursor_x = 0;
    write_out(ERROR_PROMPT);
    write_out(buffer);
    write_out("\n");
    return;
}

void shift(){
    clearScreen();

    for (int i = 1; i < rows_to_show; i++){
        
        int line_number = mod(i + (limit_index - rows_to_show + 1), VERT_SIZE);
        print(screen_buffer[line_number]);
        if (i != rows_to_show - 1)
            print("\n");
    }
}

int check_shift(){
    int shifted = 0;
    if (cursor_y == limit_index){
        shift();
        shifted = 1;
        limit_index = (limit_index + 1) % VERT_SIZE;
    }

    cursor_y = (cursor_y + 1) % VERT_SIZE;
    return shifted;
}


void write_out(char* string){
    for (int c = 0; c < strlen(string)-1; c++){
        // el menos 1 es porq line_size es 1 mas que el maximo indice
        if (cursor_x == line_size - 1|| string[c] == '\n'){
            if (string[c] == '\n') 
                screen_buffer[cursor_y][cursor_x] = '\0'; // null terminate en caso de print
            else
                c--;    // se que parece raro pero esto esta para no escribir el \n en la tabla pero si los demas despues del shift
            check_shift();
            cursor_x = 0;
        }
        // ese else c-- esta para que pueda entrar a este else con el ultimo caracter de cada linea
        else {
            screen_buffer[cursor_y][cursor_x++] = string[c];
        }
    }

    print(string);
}


void init_shell(){
    font_size = getFontSize();
    rows_to_show = VERT_SIZE/font_size;
    line_size = LINE_SIZE/font_size;
}


void resize(){
    init_shell();

    int from = mod(cursor_y - rows_to_show, VERT_SIZE);
    int offset = 0;

    // voy a hacer un for auxiliar para no tener que shiftear mil veces
    for (int i = 0; i < rows_to_show; i++){
        int line_len = strlen(screen_buffer[(from + i) % VERT_SIZE]);
        if (screen_buffer[(from + i) % VERT_SIZE][0] == 0 || line_len > line_size)

            // si esto cambia a dejar size mas grande de 2 tendria que validar ese caso pero como es un desproposito la letra tan grande no lo implemente
            offset++; 
    }

    limit_index = (cursor_y + rows_to_show - 1) % VERT_SIZE;

    clearScreen();

    for (int i = 0; i < rows_to_show - offset; i++){
        write_out(screen_buffer[(offset + from + i) % VERT_SIZE]);
        write_out("\n");
    }

}

void desize(){

    // El from va antes del init_shell para no agarrar cosas de mas cunado cambia r_t_s
    clearScreen();

    int from = mod(limit_index - rows_to_show + 1, VERT_SIZE);
    int until = mod(cursor_y - from, VERT_SIZE);
    init_shell();

    limit_index = (cursor_y + rows_to_show - 1) % VERT_SIZE;

    for (int i = 0; i < until && screen_buffer[(from + i) % VERT_SIZE][0] != '\0'; i++){
        write_out(screen_buffer[(from + i) % VERT_SIZE]);
        write_out("\n");
    }

}

void sexo_command(int argc, char **argv){
    write_out("La cantidad de argumentos es: ");
    uintToBase(argc, aux, 10);
    write_out(aux);
    write_out("\n");
    write_out("Numero es: ");
    write_out(argv[0]);
    write_out("\n");
    if(argv[0][0] == '5'){
        write_out("por el culo te la hinco\n");
    }
    exit();
}

void sexo(int argc, char **argv){
    create_process(&sexo_command, argc, argv);
}


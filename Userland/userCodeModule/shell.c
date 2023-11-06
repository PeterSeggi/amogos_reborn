#include "include/userlib.h"
#include "include/shell.h"
#include "include/userlibasm.h"
#include "include/snake.h"
#include "include/snakeModule.h"
#include <stdint.h>

#define COMMANDS 15
#define VERT_SIZE 32
#define LINE_SIZE 63
#define BUFFER_SIZE 128

int PROMPT_START[] = {128, 0};
#define ERROR_PROMPT "Unknown command: "

// Buffers
char screen_buffer[VERT_SIZE][LINE_SIZE];
char command_buffer[BUFFER_SIZE];
static char* commands[COMMANDS] = {"exit", "clear", "inc-size", "dec-size", "time", "sleep", "infoSleep", "help", "milisleep", "nanosleep", "registers", "snake", "test-div", "test-invalid", "speak"};
char char_buffer[1];

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

int *hrs, *min, *sec;
char aux[128];
int cantRegs = 18;
uint64_t regs[18];
char* regsNames[18] = {"rax:", "rbx:", "rcx:", "rdx:", "rsi:", "rdi:", "rbp:", "rsp:", "r8:", "r9:",
                       "r10:", "r11:", "r12:", "r13:", "r14:", "r15:", "rip:", "rflags:"};

// commands to do: help, resize, time, registers
// commands to shitpost: cowsay, ls, amogus?


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
        sleep_once();
    }

    clearScreen();

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
                    sleep(2, 0);
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
                    write_out("La hora es...");
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
                    sleep(4, 0);
                    break;
                case 6:
                    write_out("Voce quer usar o nosso sleep??\nVoce deve nos passar dois parametros\no primeiro e a quantidade de segundos/milissegundos/nanosegundoso segundo sera 0=segundos, 1=milissegundos e 2=nanosegundos\n");
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
                            uintToBase(regs[i], aux, 16);
                            write_out(aux);
                            write_out("\n");
                        }
                    }
                    break;
         
                case 11:
                    Snake(snakeScreen(),PLAYER1_DEFAULT_COLOR,PLAYER2_DEFAULT_COLOR);
                    cursor_y = 0;
                    cursor_x = 0;
                    limit_index = VERT_SIZE/font_size - 1;
                    flushBuffer();
                    clearScreen();
                    break;

                case 12:
                    write_out("Vamos a testear dividir 1 por 0");
                    int a = 1;
                    int b = 0;
                    int c = a/b;
                    break;

                case 13:
                    write_out("Vamos a tratar de desafiar al runtime de asm");
                    _opError();    
                    break;

                case 14:
                    _beep(1000, 50);
                    break;
                    
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

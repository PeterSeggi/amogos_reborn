#include "include/userlib.h"
#include "include/userlibasm.h"
#include "include/file_descriptors.h"


//================================================================================================================================
// Writting
//================================================================================================================================
//================================================================================================================================

void print(char * string){
    _print(STDOUT, string, strlen(string));
}

void printError(char * string){
    _print(STDERROR, string, strlen(string));
}

void read(char* buffer, int length){
    _read(STDIN, buffer, length);
}

int strlen(char * string){
    int i=0;
    while(string[i++]!=0);
    return i;
}

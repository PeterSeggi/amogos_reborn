#include "include/userlib.h"
#include "include/userlibasm.h"

#define STDOUT 1
#define STDERROR 2

//================================================================================================================================
// Writting
//================================================================================================================================
//================================================================================================================================

void printf(char * string){
    _print(STDOUT, string, strlen(string));
}

void printError(char * string){
    _print(STDERROR, string, strlen(string));
}

int strlen(char * string){
    int i=0;
    while(string[i++]!=0);
    return i;
}

#include "include/ourlib.h"
#include "include/ourasmlib.h"

#define STDOUT 1
#define STDERROR 2

//================================================================================================================================
// Writting
//================================================================================================================================
//================================================================================================================================

void printf(char * string){
    _print(string,strlen(string),STDOUT);
}

void printError(char * string){
    _print(string,strlen(string),STDERROR);
}

int strlen(char * string){
    int i=0;
    while(string[i++]!=0);
    return i;
}
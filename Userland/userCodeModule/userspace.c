#include "include/userlib.h"
#include "include/userlibasm.h"


int main(){
    // esto se core

    char* test = "Hello amogOS!\n";
    char buffer_space[1];
    int hrs;
    int min;
    int seg;
    print(test);
    print("\n");
    getClock(&hrs, &min, &seg);
    print("La hora es...");
    if(hrs<10) print("0");
    printDec(hrs);
    print(":");
    if(min<10) print("0");
    printDec(min);
    print(":");
    if(seg<10) print("0");
    printDec(seg);
    print("\n");
    int cant=4; 
    int uni=0; //0=segundos, 1=miliseg, 2=nanoseg
    sleep(cant, uni);
    printClock();

    while(1){
        if (read(buffer_space, 1) == 1){
            if (buffer_space[0] == 0x1E)
                print("a");  
        }
    }
    return 0;
}

int returns_48(){
    return 48;
}

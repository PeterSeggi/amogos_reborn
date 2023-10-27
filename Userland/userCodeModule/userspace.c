#include "include/userlib.h"
#include "include/userlibasm.h"


int main(){
    // esto se core

    char* test = "Hello amogOS!\n";
    char buffer_space[1];
    uint32_t hrs;
    uint32_t min;
    uint32_t seg;
    print(test);
    print("\n");
    getClock(&hrs, &min, &seg);
    print("La hora es...");
    printDec(hrs);
    print(":");
    printDec(min);
    print(":");
    printDec(seg);
    print("\n");
    int cant=3;
    int uni=0;
    sleep(cant, uni);
    print("\n");
    getClock(&hrs, &min, &seg);
    print("La hora es...");
    printDec(hrs);
    print(":");
    printDec(min);
    print(":");
    printDec(seg);
    print("\n");

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

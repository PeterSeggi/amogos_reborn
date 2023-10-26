#include "include/userlib.h"
#include "include/userlibasm.h"


int main(){
    // esto se core

    char* test = "Hello amogOS!\n";
    char buffer_space[1];
    int cant=2;
    int uni=4;
    print(test);
    printTime();
    sleep(cant, uni);
    print("prueba");
    while(1){
        if (read(buffer_space, 1) == 1){
            if (buffer_space[0] == 0x1E)
                print("Boca ");
        }
    }
    return 0;
}

int returns_48(){
    return 48;
}

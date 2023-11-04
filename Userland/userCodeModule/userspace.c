#include "include/userlib.h"
#include "include/shell.h"
#include "include/userlibasm.h"
#include "include/drawings.h"
#include "include/snake.h"
#include "include/snakeModule.h"
#include "include/rand.h"


int main(){    
    
    while(1){
        uint8_t selection= mainMenu();

        if(selection==1){
            shell();
        }
        else{
            uint8_t amountPlayers=snakeScreen();
            Snake(amountPlayers,PLAYER1_DEFAULT_COLOR,PLAYER2_DEFAULT_COLOR);
        }
    }
    

    return 0;
}

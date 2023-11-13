#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

//quit signal to be used when user presses Ctrl + C
void quitSignal(){
    signal(SIGINT, SIG_DFL);
    endwin();
    raise(SIGINT);
}

int main(int ac, char *av[]){

    //signal, initialize curses, hide cursor, etc...
    signal(SIGINT, quitSignal);
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    clear();

    //get speed from command line argument, if applicable
    int speed = 1;

    if (ac == 2){
        speed = atoi(av[1]);
    }

    //speed input must be between 1-99
    if (speed > 99 || speed < 1){
        speed = 1;
    }

    //2D array of terminal window initially stores blank (space) characters
    char windowArray[LINES][COLS + 1];

    for (int i = 0; i < LINES; i++){

        for (int j = 0; j < COLS; j++){
            windowArray[i][j] = ' ';
        }

        windowArray[i][COLS] = '\0';
        
    }

    //loop forever
    while (1){

        //fill each row with 0s, 1s, and spaces
        for (int i = 0; i < COLS; i++){
            int randomNum = rand() % 3;
            if (randomNum == 0){
                windowArray[0][i] = '0';
            } else if (randomNum == 1){
                windowArray[0][i] = '1';
            } else {
                windowArray[0][i] = ' ';
            }
        }

        windowArray[0][COLS] = '\0';

        //print each row of characters
        for (int i = 0; i < LINES; i++){
            mvprintw(i, 0, windowArray[i]);
            refresh();
        }

        //make program "rain" down lines of characters
        for (int i = LINES - 1; i > 0; i--){
            strcpy(windowArray[i], windowArray[i - 1]);    
        }

        usleep(1000000 / speed);

    }
    
}
#include<stdio.h> 
#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
// Semester Project - Snake Game
// by Andy Weekes and William Horton
// CS 355-02

// NOTE - needs to be compiled with -lncurses

// boolean to determine game end - not utilized often due to signal raising
    // primarily utilized when snake attempts to reverse direction
int gameEnd = 0;

// is there a tail to add - counts how many tails to add - increments upon eaten fruit - decrements after each tail added each frame
int tailNum = 0; // for when 

// snake location array - represented by index being segment of the body, 0 being the head, and two stored values per segment which represent row,col
int snakeLocation[2048][2];

// fruit location array - two values which represent row,col, trophy number
int fruitLocation [3];

// snake current size - INCLUDES THE HEAD + 1
int currentSize = 4;

// stores the current direction of the snake
int defaultDirection;

// stores previous move of the snake
int prevDirection;

// stores the speed of the snake
int speed = 150000;

// stores the time since the last fruit generated
float timeElapsed = 0;

// stores the amount time until the current fruit expires
int expirationTimer = 0;


// methods
void loop(int signum); // loops the game
void draw(); // draws the board and snake
void checkDeathConditions(); // checks the death conditions of the snake
void snakeMovement(); // updates the next step for the snake
void fruitGenerator(); // generates a new fruit on the field with a random expiration time, digit, and location

void ouch(int signum); // ends the game using ctl - c

// Draws the board, snake, and fruit
// Author - by Andy
void draw() {

    // First - draw the border
    move(0,0);
    // first row
    for (int i = 0; i < COLS; i++) {
        printw("#");
    }
    refresh();
    // all middle rows
    for (int i = 0; i < LINES - 2; i++) {
        for (int j = 0; j < COLS; j++) {
            if (j == 0 || j == COLS - 1) {
                printw("#");
            } else {
                printw(" ");
            }
        }
    }
    refresh();
    // last row
    for (int i = 0; i < COLS; i++) {
        printw("#");
    }
    refresh();

    // Second - draw snake location
    // draw the headhead
    move(snakeLocation[0][0], snakeLocation[0][1]);
    // draw the snake depending on direction
    if (defaultDirection == KEY_UP) {
        addstr("^"); 
    } else if(defaultDirection == KEY_DOWN) {
        addstr("v"); 
    } else if(defaultDirection == KEY_LEFT) {
        addstr("<"); 
    } else if(defaultDirection == KEY_RIGHT) {
        addstr(">"); 
    }
    refresh();
    // draw the body
    for (int i = 1; i < currentSize; i++) {
        move(snakeLocation[i][0],snakeLocation[i][1]);
        addstr("o");
        refresh();
    }
    refresh();

    // Third - print fruit
    move(fruitLocation[0],fruitLocation[1]);
    printw("%d",fruitLocation[2]);
    refresh();
}

// Determines the next movement of the snake based on user input and current direction
// Author - by Will
void snakeMovement() {

    //nodelay(stdscr,TRUE);
    // initializes the first segment of the snake to the head location
    snakeLocation[1][0] = snakeLocation[0][0];
    snakeLocation[1][1] = snakeLocation[0][1];

    // if there is a tail to be added - shift the location of all n segments to the location of n-1 except the head and the first segmenet
    int addingStep = 0;
    if (tailNum > 0) {
        currentSize++;
        for (int i = currentSize; i > 1; i--) {
            snakeLocation[i][0] = snakeLocation[i-1][0];
            snakeLocation[i][1] = snakeLocation[i-1][1];
        }
        tailNum--;
        addingStep = 1;
    } 
    // if there was not a segment added this frame, move the rest of the body one step, else keep the body in place
    if (addingStep == 0) {
        int savedRow = snakeLocation[0][0];
        int savedCol = snakeLocation[0][1];
        for (int i = 1; i < currentSize; i++) {
            int tempRow = snakeLocation[i][0];
            int tempCol = snakeLocation[i][1];
            snakeLocation[i][0] = savedRow;
            snakeLocation[i][1] = savedCol;
            savedRow = tempRow;
            savedCol = tempCol;
        }
    }
    
    // updates the next location of the head depending on key press
    if (defaultDirection == KEY_UP) {
        snakeLocation[0][0]--;
    } else if (defaultDirection == KEY_DOWN) {
        snakeLocation[0][0]++;
    } else if (defaultDirection == KEY_LEFT) {
        snakeLocation[0][1]--;
    } else if (defaultDirection == KEY_RIGHT) {
        snakeLocation[0][1]++;
    } 
    // sets the previous action to the last key press - for checking reversing direction
    prevDirection = defaultDirection;
}

// Checks most death conditions - including boundary crossing, and running into itself
// Author - by Andy
void checkDeathConditions() {
    // check if row is less than or equal 0 or greater than or equal to amount of rows
    if (snakeLocation[0][0] <= 0 || snakeLocation[0][0] >= LINES - 1) {
        endwin();
        raise(SIGINT); 
    }

    // check if the snake ran into the column sides of the screen
    if (snakeLocation[0][1] <= 0 || snakeLocation[0][1] >= COLS - 1) {
        endwin();
        raise(SIGINT); 
    }

    // check if the snake ran into itself
    for (int i = 1; i < currentSize; i++) {
        if (snakeLocation[0][0] == snakeLocation[i][0] && snakeLocation[0][1] == snakeLocation[i][1]) {
            endwin();
            raise(SIGINT);
        }
    }
}

// Generates a new fruit randomly located on the screen and resets the expiration timer
// Author - by Will
void fruitGenerator() {
    
    // sets the digit of the fruit to a random value between 1 to 9
    fruitLocation[2] = (rand() % 9) + 1;    
    
    // to search for a new fruit location - must be in the borders as well as not on the snake head or segmenents
    int fruitFound = 0;
    while (fruitFound == 0) {
        // generate a new random row and col for the fruit
        int fruitRow = rand() % (LINES - 3) + 1;
        int fruitCol = rand() % (COLS - 3) + 1;    
        
        // check if the fruit overlaps with the snake
        int fruitOverlap = 0;
        for (int i = 0; i < currentSize; i++) {
            if (fruitRow  == snakeLocation[i][0] && fruitCol == snakeLocation[i][1]) {
                fruitOverlap = 1;
            }
        }
        // if all conditions passed, set as new row and col for the fruit
        if (fruitOverlap == 0) {
            fruitFound = 1;
            fruitLocation[0] = fruitRow;
            fruitLocation[1] = fruitCol;
        }    
    }    
    // set expiration time to random number between 1 - 9
    expirationTimer = (rand() % 9) + 1;

    // reset the time elapsed since getting a fruit
    timeElapsed = 0.0;
}

// Game loop - allows the program to continue updating
// Author - by Andy
void loop(int signum) {
    // sets the speed proportional to the snake size
    speed = 1000000/ (currentSize + 5);
    clear();
    draw(); // calls upon draw method
    snakeMovement(); // updates the snake movement
    checkDeathConditions(); // checks death conditions

    // checks if the player has made contact with the fruit
    if (fruitLocation[0] == snakeLocation[0][0] && fruitLocation[1] == snakeLocation[0][1]) {
        tailNum += fruitLocation[2]; 
        fruitGenerator();
    }

    timeElapsed+= (float) speed/1000000; // converts speed to seconds - able to count the amount of time between each frame

    // if the current time elapsed is greater than the random expiration time of the fruit - generate a new fruit
    if (expirationTimer < timeElapsed) {
        fruitGenerator();    
    }

    // win condition for the player - where current size is greater or equal to the perimeter/2
    if (currentSize >= (COLS + LINES) + 1) {
        endwin();
        raise(SIGINT);
    }

    // calls the loop method after speed time
    ualarm(speed,0);
}


// Initialization method and user input - main driver
// Author - by Will
int main(int ac, char* av[]) {
    // initialize new seed for random number generator to ensure different fruit locations and initial directions
    srand(time (NULL));

    // begin curses screen
    initscr();
    clear();
    
    // choose a random direction between right left up down
    int initialDirection = rand() % 4;
    switch(initialDirection) {
        case 0:
            defaultDirection = KEY_RIGHT;
            // set head of snake
            snakeLocation[0][0] = LINES/2;
            snakeLocation[0][1] = COLS/2;

            // first sections of snake
            snakeLocation[1][0] = LINES/2;
            snakeLocation[1][1] = (COLS/2) - 1;

            snakeLocation[2][0] = LINES/2;
            snakeLocation[2][1] = (COLS/2) - 2;

            break;            
        case 1:
            defaultDirection = KEY_LEFT;
            // set head of snake
            snakeLocation[0][0] = LINES/2;
            snakeLocation[0][1] = (COLS/2) - 2;

            // first sections of snake
            snakeLocation[1][0] = LINES/2;
            snakeLocation[1][1] = (COLS/2) - 1;

            snakeLocation[2][0] = LINES/2;
            snakeLocation[2][1] = (COLS/2);
            break;
        case 2:
            defaultDirection = KEY_UP;
            snakeLocation[0][0] = LINES/2;
            snakeLocation[0][1] = COLS/2;

            // first sections of snake
            snakeLocation[1][0] = LINES/2;
            snakeLocation[1][1] = (COLS/2) - 1;

            snakeLocation[2][0] = LINES/2;
            snakeLocation[2][1] = (COLS/2) - 2;
            break;
        case 3:
            defaultDirection = KEY_DOWN;
            snakeLocation[0][0] = LINES/2;
            snakeLocation[0][1] = COLS/2;

            // first sections of snake
            snakeLocation[1][0] = LINES/2;
            snakeLocation[1][1] = (COLS/2) - 1;

            snakeLocation[2][0] = LINES/2;
            snakeLocation[2][1] = (COLS/2) - 2;
            break;
    }

    // ouch ctl c enabled
    signal(SIGINT,ouch); 
    noecho();
    crmode();
    // set cursor visibility to 0
    curs_set(0); 
    // begins loop alarm - starts the game loop
    signal(SIGALRM,loop);
    ualarm(1000,0);

    // enables working with the arrow keys
    keypad(stdscr,TRUE);    
    // create a fruit at the beginning of the game
    fruitGenerator();

    // user input loop 
    while (gameEnd == 0) {

        // always wait for character input from user
        int ch = getch();
        refresh();

        // sets the user input to next direction for snake, however if player attempts to reverse, game ends
        switch (ch) {
            case KEY_UP:
                if (prevDirection == KEY_DOWN) {
                    gameEnd = 1;
                }
                defaultDirection = KEY_UP;
                break;
            case KEY_DOWN:
                if (prevDirection == KEY_UP) {
                    gameEnd = 1;
                }
                defaultDirection = KEY_DOWN;
                break;
            case KEY_LEFT:
                if (prevDirection == KEY_RIGHT) {
                    gameEnd = 1;
                }
                defaultDirection = KEY_LEFT;
                break;
            case KEY_RIGHT:
                if (prevDirection == KEY_LEFT) {
                    gameEnd = 1;
                }
                defaultDirection = KEY_RIGHT;
                break;
        }
        refresh();
    }
    endwin(); // end window upon end game
}

// Ouch method - upon ctl c able to end program
// by Andy/Kurkovsky code
void ouch(int signum) {
    signal(SIGINT,SIG_DFL);
    raise(SIGINT);    
}
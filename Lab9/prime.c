#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <curses.h>

int largestPrimeNum = 0;        //stores value of largest prime number calculated so far

// interrupt signal handler
void quitSignal(int signum) {
    initscr();
    printw("\nLargest prime calculated so far: %d --- Quit [y/n]? ", largestPrimeNum);
    refresh();
    int stop = 0;                               //used to stop program execution after user enters a character
    if (stop == 0){
        char characterInput = getch();                      //use this to scan and evaluate user input (a single character)
        if (characterInput == 'y' || characterInput == 'Y'){
            refresh();                          //refresh screen and display changes
            signal(SIGINT, SIG_DFL);            //restore the default interrupt signal handler
            raise(SIGINT);                      //generates an interrupt signal, which kills the process
        } else if (characterInput == 'n' || characterInput == 'N'){
            printw("\nCalculating primes...");
            refresh();     
            stop = 1;                           //set stop == 1 so that we can refresh screen and continue calculating primes
        } else {
            printw("\n---ERROR, please enter 'y' or 'n': ");       //if input is not 'y' or 'Y' or 'n' or 'N'...
            char ch = getch();                                     //get next character to either quit or continue
            if (ch == 'y' || ch == 'Y'){
                refresh();
                signal(SIGINT, SIG_DFL);
                raise(SIGINT);
            } else  if (ch == 'n' || ch == 'N'){
                printw("\nCalculating primes...");
                refresh();     
                stop = 1;
            }
        }
    }
}

// function used to determine if current number is a prime
int calculatePrime(int index) {
    if (index <= 1) {       //ignore 0 and 1 because they are not prime numbers
        return -1;
    }
    for (int i = 2; i <= index / 2; i++) {  //Beginning at 2, determine if any number up to
        if (index % i == 0) {               //half the value of the passed-in integer
            return 0;                       //can divide the passed-in integer with no remainder.
        }                                   //If any number can, return 0 because the passed-in integer
    }                                       //is thus not a prime number.
    return 1;           //If control reaches here, return 1 because
                        //the passed-in integer is a prime number.
}

//function used to determine the largest prime number calculated so far
void calculateLargestPrime() {
    printf("Calculating primes...\n");
    for (int i = 2; i <= 1000000; i++) {      //Iterate from 2 (the smallest prime number) to the upper bound
        usleep(10000000 / 100);               //Make program sleep for 100000 microseconds (0.1 seconds) between each prime calculation
        if (calculatePrime(i) == 1) {         //If current index is a prime, assign its value to largest prime number
            largestPrimeNum = i;
        }
    }
}

//function main
int main(){
    signal(SIGINT, quitSignal);       //reprograms the interrupt signal handler
    calculateLargestPrime();          //calculate primes
    return 0;
}
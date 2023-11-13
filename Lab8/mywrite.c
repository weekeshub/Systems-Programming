/*
Working scaffolding for the write system command
Ctrl-D sends EOF character
Ctrl-Z suspends the process
    use ps to see the list of all processes and get pid of a suspended processes
    use kill (or kill -KILL) to terminate the suspended proecess, if needed
    you may also try using top or htop commands to see the list of running
processes
*/

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <utmp.h>
#include <stdlib.h>

#define BUFSIZE 1024

time_t get_time() {
    time_t currentTime = time(NULL);
    char *timeString = ctime(&currentTime);
    return currentTime;
}

int main(int ac, char *av[]) {

    struct utmp currentRecord;
    int intUTMPfile;
    int intUTMPlength = sizeof(currentRecord);
    time_t time = get_time();
    char username[UT_NAMESIZE];
    char terminal[] = "/dev/";

    intUTMPfile = open(UTMP_FILE, O_RDONLY);
    if (intUTMPfile == -1) {
        perror(UTMP_FILE);
        return 1;
    }

    if (ac != 2) {      // the program requires one parameter: the username whose terminal you wish to write to
        fprintf(stderr, "usage: %s username\n", av[0]);
    } else {

        while (read(intUTMPfile, &currentRecord, intUTMPlength) == intUTMPlength) {
            strncpy(username, currentRecord.ut_user, UT_NAMESIZE);
            if (strcmp(av[1], username) != 0){
                printf("ERROR --- please enter a username that is currently logged into a terminal.\n");
                exit(1);
            }
        }
        
        strncat(terminal, currentRecord.ut_line, 6);
        printf("Currently writing from %s on %s:\n", currentRecord.ut_user, terminal);

        int fd = open(terminal, O_WRONLY);         // open the target terminal for writing
        if (fd == -1) {                            // what if it could not be opened
            perror(av[1]);
        } else {

            char buf[BUFSIZE];
            char hiMessage1[] = "\nMessage from ";
            char hiMessage2[] = "@cloudshell";
            char hiMessage3[] = " on ";
            char hiMessage6[] = " at ";
            char byeMessage[] = "EOF\n";

            write(fd, hiMessage1, strlen(hiMessage1));          //"Message from "
            write(fd, currentRecord.ut_user, UT_NAMESIZE);      //"username"
            write(fd, hiMessage2, strlen(hiMessage2));          //"@cloudshell"
            write(fd, hiMessage3, strlen(hiMessage3));          //" on "
            write(fd, currentRecord.ut_line, UT_LINESIZE);      //"terminal"
            write(fd, hiMessage6, strlen(hiMessage6));          //" at "
            write(fd, ctime(&time), strlen(ctime(&time)));      //"current time"

            while (fgets(buf, BUFSIZE, stdin) != 0) {           // read a string from the current terminal
                if (write(fd, buf, strlen(buf)) == -1) {        // and write it to the target terminal
                    break;
                }
            }

            write(fd, byeMessage, strlen(byeMessage));          // print EOF at the very end of the session
            close(fd);

        }
    }
}
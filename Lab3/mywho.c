#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(){

    struct utmp currentRecord;                  // a single UTMP record will be stored here
    int intUTMPfile;                            // a file descriptor to work with the UTMP file
    int intUTMPlength=sizeof(currentRecord);    // how many bytes are there in a UTMP record

    //open UTMP
    intUTMPfile = open(UTMP_FILE, O_RDONLY);

    //print error if file descriptor is -1
    if (intUTMPfile == -1) {
        perror(UTMP_FILE);
        return 1;
    }

    //read UTMP file
    while(read(intUTMPfile, &currentRecord, intUTMPlength) == intUTMPlength){

        //display record
        time_t time = currentRecord.ut_tv.tv_sec;       //variable to hold current calendar time
        printf("%s %s        %.24s (%s) \n", currentRecord.ut_user, currentRecord.ut_line, ctime(&time), currentRecord.ut_host);
        
    }

    //close UTMP
    close(intUTMPfile);

    return 0;
}
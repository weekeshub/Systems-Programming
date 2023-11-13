#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <asm-generic/ioctls.h>
#include <sys/ioctl.h>

//open directory
//while not end of directory
    //read directory entry
    //print directory entry name
//close directory

char directoryArray[100][100];
int directoryArraySize = 0;
char hiddenDirectoryArray[100][100];
int hiddenDirectoryArraySize = 0;
char finalArray[100][100];
int finalArraySize = 0;
int rowNum = 0;
int hiddenRowNum = 0; 

int myStringCompare1(const void *a, const void *b){      //void means has no type, these are generic pointers
    return strcmp(a,b);                                  //string in C are pointers to characters, they are character arrays
}

int myStringCompare2(const void *a, const void *b){      //void means has no type, these are generic pointers
    return -strcmp(a,b);                                 //string in C are pointers to characters, they are character arrays
}

int get_screen_width(){
    struct winsize wbuf;
    int windowWidth = 0;
    if ( ioctl(0, TIOCGWINSZ, &wbuf) != -1 ){
        windowWidth = wbuf.ws_col;
    }
    return windowWidth;
}

void do_ls1(char *dir_name){
    DIR *dir_ptr;                       //directory pointer
    struct dirent *dirent_ptr;          //directory entry pointer
    dir_ptr = opendir(dir_name);        //system call to open a directory, pointer points to directory
    if (dir_ptr == NULL){
        perror(dir_name);
    }

    while((dirent_ptr = readdir(dir_ptr)) != 0){            //read directory, pointer points to directory to read
        if (dirent_ptr -> d_name[0] == '.'){
            continue;
        }
        int colnum = 0;
        for (int index = 0; index < 100; index++){
            directoryArray[rowNum][colnum] = dirent_ptr -> d_name[index];
            colnum++;
        }
        directoryArraySize++;
        rowNum++;
    }

    char longestString[100];
    int longestStringLength;
    int windowWidth = get_screen_width();
    int columnLength = 0;
    int numColumns = 0;
    int numRows = 0;

    for (int i = 0; i < directoryArraySize; i++){
        if ( strlen(directoryArray[i]) > longestStringLength) {
            longestStringLength = strlen(directoryArray[i]);
            strcpy(longestString, directoryArray[i]);
        }
    }

    columnLength = longestStringLength;
    numColumns = windowWidth / longestStringLength;
    numRows = (directoryArraySize / numColumns) + 1;
    int overflowModifier = 0;
    int filledColumns = directoryArraySize % numColumns;

    for (int rowIncrement = 0; rowIncrement < numRows; rowIncrement++){
        for (int arrayIndex = rowIncrement; arrayIndex < directoryArraySize; arrayIndex += (numRows - overflowModifier)){
            printf("%s", directoryArray[arrayIndex]);
            for (int spaces = 0; spaces < longestStringLength - strlen(directoryArray[arrayIndex]); spaces++){
                printf(" ");
            }
            printf(" ");
            if ((arrayIndex + 1) == (filledColumns * numRows)){
                break;
            }
            if (arrayIndex < (filledColumns * numRows)){
                overflowModifier = 0;
            } else {
                overflowModifier = 1;
            }
        }
        printf ("\n");
    }

    closedir(dir_ptr);
}

void do_ls2(char *dir_name){
    DIR *dir_ptr;                       //directory pointer
    struct dirent *dirent_ptr;          //directory entry pointer
    dir_ptr = opendir(dir_name);        //system call to open a directory, pointer points to directory
    if (dir_ptr == NULL){
        perror(dir_name);
    }

    while((dirent_ptr = readdir(dir_ptr)) != 0){            //read directory, pointer points to directory to read
        int colnum = 0;
        for (int index = 0; index < 100; index++){
            finalArray[rowNum][colnum] = dirent_ptr -> d_name[index];
            colnum++;
        }
        finalArraySize++;
        rowNum++;
    }

    char longestString[100];
    int longestStringLength;
    int windowWidth = get_screen_width();
    int columnLength = 0;
    int numColumns = 0;
    int numRows = 0;

    for (int i = 0; i < finalArraySize; i++){
        if ( strlen(finalArray[i]) > longestStringLength) {
            longestStringLength = strlen(finalArray[i]);
            strcpy(longestString, finalArray[i]);
        }
    }

    columnLength = longestStringLength;
    numColumns = windowWidth / longestStringLength;
    numRows = (finalArraySize / numColumns) + 1;
    int overflowModifier = 0;
    int filledColumns = finalArraySize % numColumns;

    for (int rowIncrement = 0; rowIncrement < numRows; rowIncrement++){
        for (int arrayIndex = rowIncrement; arrayIndex < finalArraySize; arrayIndex += (numRows - overflowModifier)){
            printf("%s", finalArray[arrayIndex]);
            for (int spaces = 0; spaces < longestStringLength - strlen(finalArray[arrayIndex]); spaces++){
                printf(" ");
            }
            printf(" ");
            if ((arrayIndex + 1) == (filledColumns * numRows)){
                break;
            }
            if (arrayIndex < (filledColumns * numRows)){
                overflowModifier = 0;
            } else {
                overflowModifier = 1;
            }
        }
        printf ("\n");
    }

    closedir(dir_ptr);
}

void do_ls3(char *dir_name){
    DIR *dir_ptr;                       //directory pointer
    struct dirent *dirent_ptr;          //directory entry pointer
    dir_ptr = opendir(dir_name);        //system call to open a directory, pointer points to directory
    if (dir_ptr == NULL){
        perror(dir_name);
    }

    while((dirent_ptr = readdir(dir_ptr)) != 0){            //read directory, pointer points to directory to read
        if (dirent_ptr -> d_name[0] == '.'){
            continue;
        }
        int colnum = 0;
        for (int index = 0; index < 100; index++){
            directoryArray[rowNum][colnum] = dirent_ptr -> d_name[index];
            colnum++;
        }
        directoryArraySize++;
        rowNum++;
    }

    qsort(directoryArray, directoryArraySize, 100, myStringCompare1);

    char longestString[100];
    int longestStringLength;
    int windowWidth = get_screen_width();
    int columnLength = 0;
    int numColumns = 0;
    int numRows = 0;

    for (int i = 0; i < directoryArraySize; i++){
        if ( strlen(directoryArray[i]) > longestStringLength) {
            longestStringLength = strlen(directoryArray[i]);
            strcpy(longestString, directoryArray[i]);
        }
    }

    columnLength = longestStringLength;
    numColumns = windowWidth / longestStringLength;
    numRows = (directoryArraySize / numColumns) + 1;
    int overflowModifier = 0;
    int filledColumns = directoryArraySize % numColumns;

    for (int rowIncrement = 0; rowIncrement < numRows; rowIncrement++){
        for (int arrayIndex = rowIncrement; arrayIndex < directoryArraySize; arrayIndex += (numRows - overflowModifier)){
            printf("%s", directoryArray[arrayIndex]);
            for (int spaces = 0; spaces < longestStringLength - strlen(directoryArray[arrayIndex]); spaces++){
                printf(" ");
            }
            printf(" ");
            if ((arrayIndex + 1) == (filledColumns * numRows)){
                break;
            }
            if (arrayIndex < (filledColumns * numRows)){
                overflowModifier = 0;
            } else {
                overflowModifier = 1;
            }
        }
        printf ("\n");
    }

    closedir(dir_ptr);
}

void do_ls4(char *dir_name){
    DIR *dir_ptr;                       //directory pointer
    struct dirent *dirent_ptr;          //directory entry pointer
    dir_ptr = opendir(dir_name);        //system call to open a directory, pointer points to directory
    if (dir_ptr == NULL){
        perror(dir_name);
    }

    while((dirent_ptr = readdir(dir_ptr)) != 0){            //read directory, pointer points to directory to read
        if (dirent_ptr -> d_name[0] == '.'){
            continue;
        }
        int colnum = 0;
        for (int index = 0; index < 100; index++){
            directoryArray[rowNum][colnum] = dirent_ptr -> d_name[index];
            colnum++;
        }
        directoryArraySize++;
        rowNum++;
    }

    qsort(directoryArray, directoryArraySize, 100, myStringCompare2);

    char longestString[100];
    int longestStringLength;
    int windowWidth = get_screen_width();
    int columnLength = 0;
    int numColumns = 0;
    int numRows = 0;

    for (int i = 0; i < directoryArraySize; i++){
        if ( strlen(directoryArray[i]) > longestStringLength) {
            longestStringLength = strlen(directoryArray[i]);
            strcpy(longestString, directoryArray[i]);
        }
    }

    columnLength = longestStringLength;
    numColumns = windowWidth / longestStringLength;
    numRows = (directoryArraySize / numColumns) + 1;
    int overflowModifier = 0;
    int filledColumns = directoryArraySize % numColumns;

    for (int rowIncrement = 0; rowIncrement < numRows; rowIncrement++){
        for (int arrayIndex = rowIncrement; arrayIndex < directoryArraySize; arrayIndex += (numRows - overflowModifier)){
            printf("%s", directoryArray[arrayIndex]);
            for (int spaces = 0; spaces < longestStringLength - strlen(directoryArray[arrayIndex]); spaces++){
                printf(" ");
            }
            printf(" ");
            if ((arrayIndex + 1) == (filledColumns * numRows)){
                break;
            }
            if (arrayIndex < (filledColumns * numRows)){
                overflowModifier = 0;
            } else {
                overflowModifier = 1;
            }
        }
        printf ("\n");
    }

    closedir(dir_ptr);
}

void do_ls5(char *dir_name){
    DIR *dir_ptr;                       //directory pointer
    struct dirent *dirent_ptr;          //directory entry pointer
    dir_ptr = opendir(dir_name);        //system call to open a directory, pointer points to directory
    if (dir_ptr == NULL){
        perror(dir_name);
    }

    while((dirent_ptr = readdir(dir_ptr)) != 0){            //read directory, pointer points to directory to read
        int colnum = 0;
        for (int index = 0; index < 100; index++){
            finalArray[rowNum][colnum] = dirent_ptr -> d_name[index];
            colnum++;
        }
        finalArraySize++;
        rowNum++;
    }

    qsort(finalArray, finalArraySize, 100, myStringCompare1);

    char longestString[100];
    int longestStringLength;
    int windowWidth = get_screen_width();
    int columnLength = 0;
    int numColumns = 0;
    int numRows = 0;

    for (int i = 0; i < finalArraySize; i++){
        if ( strlen(finalArray[i]) > longestStringLength) {
            longestStringLength = strlen(finalArray[i]);
            strcpy(longestString, finalArray[i]);
        }
    }

    columnLength = longestStringLength;
    numColumns = windowWidth / longestStringLength;
    numRows = (finalArraySize / numColumns) + 1;
    int overflowModifier = 0;
    int filledColumns = finalArraySize % numColumns;

    for (int rowIncrement = 0; rowIncrement < numRows; rowIncrement++){
        for (int arrayIndex = rowIncrement; arrayIndex < finalArraySize; arrayIndex += (numRows - overflowModifier)){
            printf("%s", finalArray[arrayIndex]);
            for (int spaces = 0; spaces < longestStringLength - strlen(finalArray[arrayIndex]); spaces++){
                printf(" ");
            }
            printf(" ");
            if ((arrayIndex + 1) == (filledColumns * numRows)){
                break;
            }
            if (arrayIndex < (filledColumns * numRows)){
                overflowModifier = 0;
            } else {
                overflowModifier = 1;
            }
        }
        printf ("\n");
    }

    closedir(dir_ptr);
}

void do_ls6(char *dir_name){
    DIR *dir_ptr;                       //directory pointer
    struct dirent *dirent_ptr;          //directory entry pointer
    dir_ptr = opendir(dir_name);        //system call to open a directory, pointer points to directory
    if (dir_ptr == NULL){
        perror(dir_name);
    }

    while((dirent_ptr = readdir(dir_ptr)) != 0){            //read directory, pointer points to directory to read
        int colnum = 0;
        for (int index = 0; index < 100; index++){
            finalArray[rowNum][colnum] = dirent_ptr -> d_name[index];
            colnum++;
        }
        finalArraySize++;
        rowNum++;
    }

    qsort(finalArray, finalArraySize, 100, myStringCompare2);

    char longestString[100];
    int longestStringLength;
    int windowWidth = get_screen_width();
    int columnLength = 0;
    int numColumns = 0;
    int numRows = 0;

    for (int i = 0; i < finalArraySize; i++){
        if ( strlen(finalArray[i]) > longestStringLength) {
            longestStringLength = strlen(finalArray[i]);
            strcpy(longestString, finalArray[i]);
        }
    }

    columnLength = longestStringLength;
    numColumns = windowWidth / longestStringLength;
    numRows = (finalArraySize / numColumns) + 1;
    int overflowModifier = 0;
    int filledColumns = finalArraySize % numColumns;

    for (int rowIncrement = 0; rowIncrement < numRows; rowIncrement++){
        for (int arrayIndex = rowIncrement; arrayIndex < finalArraySize; arrayIndex += (numRows - overflowModifier)){
            printf("%s", finalArray[arrayIndex]);
            for (int spaces = 0; spaces < longestStringLength - strlen(finalArray[arrayIndex]); spaces++){
                printf(" ");
            }
            printf(" ");
            if ((arrayIndex + 1) == (filledColumns * numRows)){
                break;
            }
            if (arrayIndex < (filledColumns * numRows)){
                overflowModifier = 0;
            } else {
                overflowModifier = 1;
            }
        }
        printf ("\n");
    }

    closedir(dir_ptr);
}

int main(int ac, char *av[]){

    int flagA = 0, flagS = 0, flagR = 0;
    char option;
    struct winsize wbuf;

    //check if current command line argument is an option
    while ((option = getopt(ac, av, "asr")) != -1){
        switch (option) {
            case 'a':
                flagA = 1;
                printf("flag A enabled\n");
                break;
            case 's':
                flagS = 1;
                printf("flag S enabled\n");
                break;
            case 'r':
                flagR = 1;
                printf("flag R enabled\n");
                break;
        }
    }

    //if no parameters are given, run ls on current directory with no flags
    if (ac == 1){
        do_ls1(".");
    //if parameters are given and ac == 2
    } else if (ac == 2){
        for (int i = 1; i < ac; i++){
            //check if the second parameter is a flag
            if (av[i][0] == '-'){
                //if second parameter is a flag, check if there are other parameters after it
                if (i != (ac - 1)){
                    continue;
                } else {
                    if (flagA == 1){        //-a option
                        do_ls2(".");
                    } else if (flagS == 1){     //-s option
                        do_ls3(".");
                    } else if (flagR == 1){     //-r option
                        do_ls4(".");
                    } else {        //no option specified
                        do_ls1(".");
                    }
                }
            } else {
                if (flagA == 1){        //-a option
                    do_ls2(av[i]);
                } else if (flagS == 1){     //-s option
                    do_ls3(av[i]);
                } else if (flagR == 1){     //-r option
                    do_ls4(av[i]);
                } else {        //no option specified
                    do_ls1(av[i]);
                }
            }
        }
    //if parameters are given and ac > 2
    } else {
        for (int i = 1; i < ac; i++){
            if (av[i][0] == '-'){
                continue;
            }
            if (flagA == 1){        //-a option
                if (flagS == 1){
                    do_ls5(av[i]);
                } else if (flagR == 1){
                    do_ls6(av[i]);
                } else {
                    do_ls2(av[i]);
                }
            } else if (flagS == 1){     //-s option
                do_ls3(av[i]);
            } else if (flagR == 1){     //-r option
                do_ls4(av[i]);
            } else {        //no option specified
                do_ls1(av[i]);
            }
        }
    }
    return 0;
}
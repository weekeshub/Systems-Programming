#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

//open directory
//while not end of directory
    //read directory entry
    //print directory entry name
//close directory

char directoryArray[100][100];
int directoryArraySize = 0;
char hiddenDirectoryArray[100][100];
int hiddenDirectoryArraySize = 0;
int rowNum = 0;
int hiddenRowNum = 0; 

void do_ls(char *dir_name){
    DIR *dir_ptr;                       //directory pointer
    struct dirent *dirent_ptr;          //directory entry pointer
    dir_ptr = opendir(dir_name);        //system call to open a directory, pointer points to directory
    if (dir_ptr == NULL){
        perror(dir_name);
    }

    while((dirent_ptr = readdir(dir_ptr)) != 0){            //read directory, pointer points to directory to read
        if (dirent_ptr -> d_name[0] == '.'){
            int hiddenColNum = 0;
            for (int index = 0; index < 100; index++){
                hiddenDirectoryArray[hiddenRowNum][hiddenColNum] = dirent_ptr -> d_name[index];
                hiddenColNum++;
            }
            hiddenDirectoryArraySize++;
            hiddenRowNum++;
        } else {
            int colnum = 0;
            for (int index = 0; index < 100; index++){
                directoryArray[rowNum][colnum] = dirent_ptr -> d_name[index];
                colnum++;
            }
            directoryArraySize++;
            rowNum++;
        }
    }
    closedir(dir_ptr);
}

int myStringCompare1(const void *a, const void *b){      //void means has no type, these are generic pointers
    return strcmp(a,b);                                  //string in C are pointers to characters, they are character arrays
}

int myStringCompare2(const void *a, const void *b){      //void means has no type, these are generic pointers
    return -strcmp(a,b);                                 //string in C are pointers to characters, they are character arrays
}

int main(int ac, char *av[]){

    int flagA = 0, flagS = 0, flagR = 0;
    char option;

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
        do_ls(".");
        for (int index = 0; index < directoryArraySize; index++){
            printf("%s\n", directoryArray[index]);
        }
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
                        do_ls(".");
                        for (int index = 0; index < directoryArraySize; index++){
                            printf("%s\n", directoryArray[index]);
                        }
                        for (int index = 0; index < hiddenDirectoryArraySize; index++){
                            printf("%s\n", hiddenDirectoryArray[index]);
                        }
                    } else if (flagS == 1){     //-s option
                        do_ls(".");
                        qsort(directoryArray, directoryArraySize, 100, myStringCompare1);
                        for (int index = 0; index < directoryArraySize; index++){
                            printf("%s\n", directoryArray[index]);
                        }
                    } else if (flagR == 1){     //-r option
                        do_ls(".");
                        qsort(directoryArray, directoryArraySize, 100, myStringCompare2);
                        for (int index = 0; index < directoryArraySize; index++){
                            printf("%s\n", directoryArray[index]);
                        }
                    } else {        //no option specified
                        do_ls(".");
                        for (int index = 0; index < directoryArraySize; index++){
                            printf("%s\n", directoryArray[index]);
                        }
                    }
                }
            } else {
                if (flagA == 1){        //-a option
                    do_ls(av[i]);
                    for (int index = 0; index < directoryArraySize; index++){
                        printf("%s\n", directoryArray[index]);
                    }
                    for (int index = 0; index < hiddenDirectoryArraySize; index++){
                        printf("%s\n", hiddenDirectoryArray[index]);
                    }
                } else if (flagS == 1){     //-s option
                    do_ls(av[i]);
                    qsort(directoryArray, directoryArraySize, 100, myStringCompare1);
                    for (int index = 0; index < directoryArraySize; index++){
                        printf("%s\n", directoryArray[index]);
                    }
                } else if (flagR == 1){     //-r option
                    do_ls(av[i]);
                    qsort(directoryArray, directoryArraySize, 100, myStringCompare2);
                    for (int index = 0; index < directoryArraySize; index++){
                        printf("%s\n", directoryArray[index]);
                    }
                } else {        //no option specified
                    do_ls(av[i]);
                    for (int index = 0; index < directoryArraySize; index++){
                        printf("%s\n", directoryArray[index]);
                    }
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
                do_ls(av[i]);
                for (int index = 0; index < directoryArraySize; index++){
                    printf("%s\n", directoryArray[index]);
                }
                for (int index = 0; index < hiddenDirectoryArraySize; index++){
                    printf("%s\n", hiddenDirectoryArray[index]);
                }
            } else if (flagS == 1){     //-s option
                do_ls(av[i]);
                qsort(directoryArray, directoryArraySize, 100, myStringCompare1);
                for (int index = 0; index < directoryArraySize; index++){
                    printf("%s\n", directoryArray[index]);
                }
            } else if (flagR == 1){     //-r option
                do_ls(av[i]);
                qsort(directoryArray, directoryArraySize, 100, myStringCompare2);
                for (int index = 0; index < directoryArraySize; index++){
                    printf("%s\n", directoryArray[index]);
                }
            } else {        //no option specified
                do_ls(av[i]);
                for (int index = 0; index < directoryArraySize; index++){
                    printf("%s\n", directoryArray[index]);
                }
            }
        }
    }
    return 0;
}
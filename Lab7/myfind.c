#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

char fileSearchString[100];             // stores the pattern to match against various file names
char directoryName[100];                // stores the specified directory (if one is specified)
                                        // if none are specified, directoryName = "." (current directory)
char subdirectoryPath[100];             // stores the path of each subdirectory traversal
int isDir = 0;                          // boolean to check if current directory entry is a subdirectory
int directorySpecified = 0;             // boolean to check if a directory has been specified
char filePath[100];                     // stores the total file path that has been traversed

void process_entry(char *dir_entry_name);
void do_find(char *fileName, char *dir_name);

// this function is used to convert a file's permission bits into a string
void mode_to_string(int mode, char str[]){
    strcpy(str, "----------");

    if(S_ISDIR(mode)){
        str[0] = 'd';
    }
    if(S_ISCHR(mode)){
        str[0] = 'c';
    }
    if(S_ISBLK(mode)){
        str[0] = 'b';
    }
    if(S_ISLNK(mode)){
        str[0] = 'l';
    }

    if(mode & S_IRUSR){
        str[1] = 'r';
    }
    if(mode & S_IWUSR){
        str[2] = 'w';
    }
    if(mode & S_IXUSR){
        str[3] = 'x';
    }

    if(mode & S_IRGRP){
        str[4] = 'r';
    }
    if(mode & S_IWGRP){
        str[5] = 'w';
    }
    if(mode & S_IXGRP){
        str[6] = 'x';
    }

    if(mode & S_IROTH){
        str[7] = 'r';
    }
    if(mode & S_IWOTH){
        str[8] = 'w';
    }
    if(mode & S_IXOTH){
        str[9] = 'x';
    }
}

// this function is called to print the directory entry name and its mode
void show_dir_entry(char *dir_entry_name, struct stat *info){
    char modestring[11];
    mode_to_string(info -> st_mode, modestring);                          // this converts the mode to a string
    printf("%s (%d/%s)\n", dir_entry_name, info ->st_mode, modestring);   // this prints the directory entry and its mode
}

// this function is called for every encountered directory entry
void process_entry(char *dir_entry_name) {
    struct stat info;

    if(stat(filePath, &info)==-1){
        perror(filePath);
    } else {
        if(S_ISDIR(info.st_mode)) {                     // if this directory entry is a subdirectory
            char restorePath[100];                      // used to jump out of a subdirectory traversal back to the previous path value
            strcpy(restorePath, subdirectoryPath);
            isDir = 1;
            do_find(fileSearchString, filePath);        // explore this subdirectory
            strcpy(filePath, restorePath);              // restore previous value of file path
            strcpy(subdirectoryPath, restorePath);      // restore previous value of subdirectory file path
        } else if(S_ISREG(info.st_mode)) {              // use strstr() to check if the current directory entry's name matches the substring
            isDir = 0;
            if (strstr(filePath, fileSearchString) != NULL){    // check if fileSearchString pattern exists within the current directory entry
                if (strstr(filePath, "./") == NULL){    // if path does not contain "./", manually print it at the beginning of the path
                    printf("./");
                }
                show_dir_entry(dir_entry_name, &info);  // print matching directory entry and its mode
            }
            strcpy(filePath, subdirectoryPath);         // copy current subdirectory path to the file path
                                                        // to continue searching through that subdirectory
        } else {
            printf("%s is not a file or directory\n",  dir_entry_name);
        }                                               
    }
}

// this function is used to explore all entries of the given directory
void do_find(char *fileSearchString, char *dir_name) {
    DIR *dir_ptr;                                       // pointer to a directory structure
    struct dirent *dirent_ptr;                          // pointer to the current directory entry
    dir_ptr = opendir(dir_name);                        // open the directory

    if(dir_ptr==0) {                                    // opendir could fail, possibly because
        perror(dir_name);                               // the directory does not exist
    }
    else {
        strcat(dir_name, "/");                          // add a "/" for proper file path specification                                              // iterate over all directory entries
        while((dirent_ptr=readdir(dir_ptr)) != 0) {     // begin reading directory
            if (directorySpecified == 1 && strcmp(dir_name, "") == 0){   // if a directory was specified AND dir_name is nothing
                strcat(dir_name, "./");                                  // prepend "./" and "directoryName" to dir_name
                strcat(dir_name, directoryName);                         // so that we can search the specified directory properly
            }
            if (isDir == 1){                            // if current entry is a directory, update the subdirectory path
                strcpy(subdirectoryPath, dir_name);
            }
            if (strcmp(dirent_ptr->d_name, ".") == 0 || strcmp(dirent_ptr->d_name, "..") == 0) {
                continue;                               // skip consideration of "." and "..", it is unnecessary and problematic to examine them
            }
            strcpy(filePath, dir_name);                 // update file path
            strcat(filePath, dirent_ptr->d_name);       // add entry name to file path
            process_entry(filePath);                    // process current directory entry
        }
        closedir(dir_ptr);                              // close directory
    }
}

int main(int ac, char *av[]) {
    if (ac == 1){
        printf("Usage: %s <filename> <startpath>\n", av[0]);    // if only "./a.out" is run
    } else if (ac == 2){
        strcpy(fileSearchString, av[1]);
        strcpy(directoryName, ".");
        do_find(fileSearchString, directoryName);               // explore the contents of the current directory
    } else {
        directorySpecified = 1;
        strcpy(fileSearchString, av[1]);
        strcpy(directoryName, av[2]);
        do_find(fileSearchString, directoryName);               // explore the contents of the specified directory
    }
    return 0;
}
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFERSIZE 2048
#define COPYMODE 0644

int getArraySize(char *array){
    int count = 0;
    for (int i = 0; array[i] != 0; i++){
        count++;
    }
    return count;
}

int blankLineCheck(char *line){
    int size = getArraySize(line);
    int nonBlankCharacters = 0;
    for (int index = 0; index < size; index++){
        if (!(line[index] == ' ' || line[index] == '\t' || line[index] == '\n' || line[index] == '\r')){
            nonBlankCharacters++;
        } else {
            //no action taken if character is whitespace character
        }
    }
    if (nonBlankCharacters == 0){
        return 1;
    } else {
        return 0;
    }
}

int main(int ac, char* av[]) {
    
    int src, dst;
    char buffer[BUFFERSIZE];
    int intNumBytes;
    int flagB = 0, flagE = 0, flagN = 0, flagS = 0;
    char option;
    int lineNumber = 1;
    int nonBlankLineNumber = 1;
    int i;

    //check if current command line argument is an option
    while ((option = getopt(ac, av, "bens")) != -1){

        switch (option) {
            case 'b':
                flagB = 1;
                printf("flag B enabled\n");
                break;
            case 'e':
                flagE = 1;
                printf("flag E enabled\n");
                break;
            case 'n':
                flagN = 1;
                printf("flag N enabled\n");
                break;
            case 's':
                flagS = 1;
                printf("flag S enabled\n");
                break;
        }

    }

    //go through each command line argument
    for (i = 1; i < ac; i++){

        if (av[i][0] == '-'){
            continue;
        }

        //open source file
        src=open(av[i], O_RDONLY);
        //print error if there is an error opening the source file
        if (src < 0) {
            perror(av[i]);
            return 1;
        }

        //begin reading file
        while ((intNumBytes=read(src, buffer, BUFFERSIZE)) > 0) {

            int newLineCharacters = 0;
            int nonNewLineCharacters = 0;
            int longestLine = 0;

            for (int index = 0; index < intNumBytes; index++){
                if (buffer[index] == '\n'){
                    newLineCharacters++;
                }                
            }

            for (int index = 0; index < intNumBytes; index++){
                if (buffer[index] != '\n'){
                    nonNewLineCharacters++;
                    if ((nonNewLineCharacters + 1) > longestLine){
                        longestLine = nonNewLineCharacters + 1;
                    }
                } else {
                    nonNewLineCharacters = 0;
                }
            }

            //declare the array to hold the data from read file
            char fileArray[newLineCharacters + 1][longestLine];

            //this is where we fill the newly created array with data from the read file, attn to new lines
            
            int rownum = 0;
            int columnnum = 0;

            for (int index = 0; index < intNumBytes; index++){
                if (buffer[index] != '\n'){
                    fileArray[rownum][columnnum] = buffer[index];
                    columnnum++;
                    if(index == intNumBytes - 1) {
                        for (int index = columnnum; index < longestLine; index++) {
                            fileArray[rownum][index] = '\0';
                        }
                    }
                } else {
                    fileArray[rownum][columnnum] = buffer[index];
                    for (int index = columnnum + 1; index < longestLine; index++){
                        fileArray[rownum][index] = '\0';
                    }
                    rownum++;
                    columnnum = 0;

                }

            }

            //Flag B: print non blank line number
            if (flagB == 1){
                char fileArray2[longestLine];
                for (int row = 0; row <= newLineCharacters; row++){
                    for (int column = 0; column < longestLine; column++){
                        fileArray2[column] = fileArray[row][column];                       
                    }
                    //test for blank line
                    if (blankLineCheck(fileArray2) == 1){
                        if (flagS == 1){
                            continue;
                        }
                        if (flagN == 1){
                            printf("%d: ", lineNumber);
                            lineNumber++;
                        }
                        printf("%s", fileArray2);
                    } else {
                        if (flagN == 1){
                            printf("%d: ", lineNumber);
                            lineNumber++;
                        }
                        printf("%d: ", nonBlankLineNumber);
                        nonBlankLineNumber++;
                        for (int index = 0; index < strlen(fileArray2); index++){
                            if (flagE == 1){
                                if (fileArray2[index] == '\n'){
                                    printf(" $");
                                }
                            }
                        printf("%c", fileArray2[index]);
                        }
                    }
                }
                nonBlankLineNumber = 1;
                lineNumber = 1;
                //Flag E: print $ at the end of every (blank and non-blank) line
            } else if (flagE == 1){
                if (flagN == 1){
                    printf("%d: ", lineNumber);
                    lineNumber++;
                    if (flagS == 1){
                        char fileArray2[longestLine];
                        for (int row = 0; row <= newLineCharacters; row++){
                            printf("%d: ", lineNumber);
                            lineNumber++;
                            for (int column = 0; column < longestLine; column++){
                                fileArray2[column] = fileArray[row][column];                       
                            }
                            //test for blank line
                            if (blankLineCheck(fileArray2) == 1){
                                continue;                        
                            } else {
                                for (int index = 0; index < longestLine; index++){
                                    if (fileArray2[index] == '\n'){
                                        printf(" $");
                                    }
                                    printf("%c", fileArray2[index]);
                                }
                            }
                        }
                    }
                }
                if (flagS == 1){
                    char fileArray2[longestLine];
                    for (int row = 0; row <= newLineCharacters; row++){
                        for (int column = 0; column < longestLine; column++){
                            fileArray2[column] = fileArray[row][column];                       
                        }
                        //test for blank line
                        if (blankLineCheck(fileArray2) == 1){
                            continue;                        
                        } else {
                            for (int index = 0; index < longestLine; index++){
                                if (fileArray2[index] == '\n'){
                                    printf(" $");
                                }
                                printf("%c", fileArray2[index]);
                            }
                        }
                    }
                } else {
                    for (int row = 0; row <= newLineCharacters; row++){
                        if (flagN == 1){
                            printf("%d: ", lineNumber);
                            lineNumber++;
                        }
                        for (int column = 0; column < longestLine; column++){
                            if (fileArray[row][column] == '\n'){
                                printf(" $");
                            }
                            //print new full 2d array of lines
                            printf("%c", fileArray[row][column]);
                        }
                    }
                }
                //Flag N: print line number at the beginning of every (blank and non-blank) line
            } else if (flagN == 1){
                printf("%d: ", lineNumber);
                lineNumber++;
                if (flagS == 1){
                    char fileArray2[longestLine];
                    for (int row = 0; row <= newLineCharacters; row++){
                        for (int column = 0; column < longestLine; column++){
                            fileArray2[column] = fileArray[row][column];                       
                        }
                        //test for blank line
                        if (blankLineCheck(fileArray2) == 1){
                            continue;                        
                        } else {
                            for (int index = 0; index < longestLine; index++){
                                if (fileArray2[index] == '\n'){
                                    printf("%c", fileArray2[index]);
                                    printf("%d: ", lineNumber);
                                    lineNumber++;
                                } else {
                                    printf("%c", fileArray2[index]);
                                }
                            }
                        }
                    }
                } else {
                    for (int row = 0; row <= newLineCharacters; row++){
                        for (int column = 0; column < longestLine; column++){
                            if (fileArray[row][column] == '\n'){
                                if (flagE == 1){
                                    printf(" $");
                                }
                                printf("%c", fileArray[row][column]);
                                printf("%d: ", lineNumber);
                                lineNumber++;
                            } else {
                                //print new full 2d array of lines
                                printf("%c", fileArray[row][column]);
                            }
                        }
                    }
                }
                lineNumber = 1;
                //Flag S: squeeze file output to print single spaced
            } else if (flagS == 1){
                char fileArray2[longestLine];
                for (int row = 0; row <= newLineCharacters; row++){
                    for (int column = 0; column < longestLine; column++){
                        fileArray2[column] = fileArray[row][column];                       
                    }
                    //test for blank line
                    if (blankLineCheck(fileArray2) == 1){
                        continue;                        
                    } else {
                        for (int index = 0; index < longestLine; index++){
                            printf("%c", fileArray2[index]);
                        }
                    }
                }
            }

            //No Flag
            if (flagB == 0 && flagE == 0 && flagN == 0 && flagS == 0){
                for (int row = 0; row <= newLineCharacters; row++){
                    for (int column = 0; column < longestLine; column++){
                        //print new full 2d array of lines
                        printf("%c", fileArray[row][column]);
                    }
                }
            }
            
        }//end of while loop

    }

    //print error if there is an error close the source or destination files
    if (close(src) < 0) {
        perror("Error closing files");
        return 1;
    }

    //close both files
    close(src);

    //print error if there is an error reading from the source file
    if (intNumBytes < 0) {
        perror("Read error");
        return 1;
    }

    return 0;

}
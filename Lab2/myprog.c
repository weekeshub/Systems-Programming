#include <stdio.h>

int getArraySize(char array[]){
    int count = 0;
    for (int i = 0; array[i] != 0; i++){
        count++;
    }
    return count;
}

int main(int ac, char *av[]){
    char *charArray;
    for (int i = 1; i < ac; i++){
        charArray = av[i];
        int size = getArraySize(charArray);
        for (int j = 0; j < size; j++){
            for (int k = 0; k <= j; k++){
                if ( (charArray[k] >= 65 && charArray[k] <= 90) || (charArray[k] >= 97 && charArray[k] <= 122) ){
                    printf("%c", charArray[k]);
                } else {
                    //do something
                }
            }
            printf("\n");
        }
    }
    return 0;
}
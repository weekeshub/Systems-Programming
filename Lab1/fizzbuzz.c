#include <stdio.h>

int fizzbuzz(int number){
    if (number % 3 == 0 && number % 5 == 0){
        printf("FizzBuzz\n");
    } else if (number % 3 == 0){
        printf("Fizz\n");
    } else if (number % 5 == 0){
        printf("Buzz\n");
    } else {
        printf("%d\n", number);
    }
    return number;
}

int main(){
    int number;
    printf("Please enter an integer: ");
    scanf("%d", &number);
    for (int index = 0; index <= number; index++){
        fizzbuzz(index);
    }
    return 0;
}
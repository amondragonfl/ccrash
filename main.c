#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

double *get_double(char *prompt, double *doubleptr);

int main(void){
    return 0;
}

double *get_double(char *prompt, double *doubleptr){
    double num;
    char buf[1024];
    int success;
    do {
        printf("%s", prompt);
        if(!fgets(buf, 1024, stdin)){
            return NULL;
        }
        char *endptr;
        errno = 0;
        num = strtod(buf, &endptr);
        if (errno == ERANGE || endptr == buf){
            success = 0;
        } else {
            success = 1;
        }
    } while(!success);
    *doubleptr = num;
    return doubleptr;
}

#include "controller.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

int n = 0;

void* addNumbers();
int main(){
    loop();
    //test()
    

    return 0;
}

void test(){
    pthread_t t1;
    pthread_create(&t1, NULL, addNumbers, NULL);

    while(1){
        sleep(2);
        printf("\nn: %d", n);
    }
}

void* addNumbers(){
    while(1){
        
        sleep(0.04);
        n++;
    }
}
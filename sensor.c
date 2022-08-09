#include "sensor.h"
#include "tools.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

int wheelTurnTime_ms = 100;
int magnetSignal = 0;

/*
*   TODO:
*   wheelTurn -> sensor -> readSensor() -> controller 
*      ^           |
*      <-----------+ 
*/

void* turnWheel(void* _sensor){
    Sensor* sensor = (Sensor*)_sensor;
    while(1){
        usleep(100);
        magnetSignal = 0;
        usleep(wheelTurnTime_ms * 1000);
        magnetSignal = 1;    
    }
}

void* startSensor(void* _sensor){
    Sensor* sensor = (Sensor*)_sensor;
    pthread_t wheelThread;
    pthread_create(&wheelThread, NULL, turnWheel, sensor);
    while(1){
        usleep(1);
        if(magnetSignal == 1){
            sensor->state = 1;
        }
        if(sensor->resetFlag == 1){
            sensor->state = 0;
            sensor->resetFlag = 0;
        }
    }
}

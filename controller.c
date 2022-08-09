#include "controller.h"
#include "sensor.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "constants.h"
#include <stdlib.h>
#include "tools.h"
#include <pthread.h>

float diameter = 0.0f; // meters
float circumference = 0.0f;
float totalDistanceTraveled = 0.0f; // meters
float speed_kmh = 0;
unsigned short roundedSpeed = 0;
int numberOfImpulses = 0;

float deltaTime = 0;
time_t beginTurnTime = 0;
time_t endTurnTime = 0;
time_t beginTripTime = 0;
time_t endTripTime = 0;

void loop(){
    long refresh_rate_ms = 1000;
    //sensor setup
    Sensor* sensor = (Sensor*)malloc(sizeof(Sensor));
    sensor->id = 'X';
    pthread_t sensorThread;
    pthread_create(&sensorThread, NULL, startSensor, sensor);
    pthread_t impulseReadthread;
    pthread_create(&impulseReadthread, NULL, readSensor, sensor);
    //pthread_join(sensorThread, NULL);
    //pthread_join(impulseReadthread, NULL);

    diameter = 0.6985f;
    circumference = PI * diameter;
    //int wheel_rpm = 60;
    //long wheelSpinTime_ms = MILISECONDS_IN_MINUTE / wheel_rpm;

    float wheel_rpm = 0.0f; //wheel_rpm = MILISECONDS_IN_MINUTE / wheelSpinTime_ms;
    float wheel_rps = 0.0f;
    beginTripTime = clock();
    //loop
    while(1){
        beginTurnTime = clock();
        //usleep(1000000);
        wait(1000000);
        endTurnTime = clock();
        deltaTime = (((float)endTurnTime - (float)beginTurnTime) / 1000.0f);
        //deltaTime = refresh_rate_ms;
        //wheel_rpm = (float)MILISECONDS_IN_MINUTE / (float)deltaTime;
        //wheel_rpm = (float)numberOfImpulses / (float)deltaTime / 1000.0f * 60000.0f; //todo
        wheel_rps = (float)numberOfImpulses / ((float)deltaTime / 1000.0f); //todo
        wheel_rpm = wheel_rps * 60.0f;
        speed_kmh = wheel_rpm * circumference * (60.0f / 1000.0f);
        roundedSpeed = (unsigned short)round(speed_kmh);

        totalDistanceTraveled += circumference;

        
        //printf("\nWheel spin time: %ld (%i rpm)", deltaTime, (unsigned short)round(wheel_rpm));
        //printf("\nSpeed: %.2f km/h -> [%i km/h]\n", speed_kmh, roundedSpeed);
        //printf("\nDistance traveled: %.2f m", totalDistanceTraveled);
        //printf("\nDeltatime: %f", deltaTime);
        printf("\n| %.2f RPM |- %i km/h - %d imps", wheel_rpm, roundedSpeed, numberOfImpulses);
        //printf("\n%d Imps", numberOfImpulses);
        numberOfImpulses = 0;

        if(totalDistanceTraveled >= 40000.0f){
            endTripTime = clock();
            break;
        }
    }
    free(sensor);
    printf("\n---\nTrip time: %ld s", (endTripTime - beginTripTime) / 1000000);
    printf("\nTotal distance traveled: %.2f", totalDistanceTraveled);
    printf("\n\n----\n");
}

void* readSensor(void* _sensor){
    Sensor* sensor = (Sensor*)_sensor;
    while(1){
        usleep(100);
        if(sensor->state == 1){
            numberOfImpulses++;
            //rintf("\nIMPULSE: %d", numberOfImpulses);
            sensor->resetFlag = 1;
        }
    }
}

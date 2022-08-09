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

    float wheel_rpm = 0.0f;
    float wheel_rps = 0.0f;
    beginTripTime = clock();

    //loop
    while(1){
        beginTurnTime = clock();
        wait(1000000);
        endTurnTime = clock();
        deltaTime = (((float)endTurnTime - (float)beginTurnTime) / 1000.0f);
        wheel_rps = (float)numberOfImpulses / ((float)deltaTime / 1000.0f); //todo
        wheel_rpm = wheel_rps * 60.0f;
        speed_kmh = wheel_rpm * circumference * (60.0f / 1000.0f);
        roundedSpeed = (unsigned short)round(speed_kmh);

        totalDistanceTraveled += circumference;

        printf("\n| %.2f RPM |- %i km/h - %d imps", wheel_rpm, roundedSpeed, numberOfImpulses);
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
            sensor->resetFlag = 1;
        }
    }
}

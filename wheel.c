#include "headers/wheel.h"
#include "headers/sensor.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#define PI 3.141592653589793
#define MILISECONDS_IN_MINUTE 60000

/*
* everything is in meters
*/
float diameter = 0.0f; 
float circumference = 0.0f; //total distance around its outside
float totalDistanceTraveled = 0.0f;
float speed_kmh = 0;
unsigned short roundedSpeed = 0;

/*
* time stuff
*/
unsigned long deltaTime = 0;
time_t beginTurnTime = 0;
time_t endTurnTime = 0;

time_t beginTripTime = 0;
time_t endTripTime = 0;

void wait(long time_microseconds){
    time_t t1 = clock();
    while(t1 + time_microseconds >= clock());
}

void startTurningWheel(){
    //setup
    diameter = 0.6985f;
    circumference = PI * diameter;
    //int wheel_rpm = 60;
    //long wheelSpinTime_ms = MILISECONDS_IN_MINUTE / wheel_rpm;

    long wheelSpinTime_ms = 320;
    float wheel_rpm = 0; //wheel_rpm = MILISECONDS_IN_MINUTE / wheelSpinTime_ms;

    beginTripTime = clock();
    //loop
    while(1){
        

        beginTurnTime = clock();
        setSensorState(0);

        wait(wheelSpinTime_ms * 1000);

        setSensorState(1);
        endTurnTime = clock();
        totalDistanceTraveled += circumference;

        deltaTime = ((endTurnTime - beginTurnTime) / 1000); //this would be calculated on arduino
        wheel_rpm = (float)MILISECONDS_IN_MINUTE / (float)deltaTime;

        speed_kmh = wheel_rpm * circumference * (60.0f / 1000.0f);
        roundedSpeed = (unsigned short)round(speed_kmh);

        printf("\nWheel spin time: %ld (%i rpm)", deltaTime, (unsigned short)round(wheel_rpm));
        printf("\nSpeed: %.2f km/h -> [%i km/h]\n", speed_kmh, roundedSpeed);
        printf("\nDistance traveled: %.2f m", totalDistanceTraveled);

        if(totalDistanceTraveled >= 40000.0f){
            endTripTime = clock();
            break;
        }
    }
    printf("\n---\nTrip time: %ld s", (endTripTime - beginTripTime) / 1000000);
    printf("\nTotal distance traveled: %.2f", totalDistanceTraveled);
    printf("\n\n----\n");
}
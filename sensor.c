#include "headers/sensor.h"

unsigned short state = 0;

unsigned short getSensorState(){
    return state;
}

void setSensorState(unsigned short _state){
    if(_state >= 1)
        state = 1;
    else
        state = 0;
}

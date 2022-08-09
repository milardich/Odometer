#ifndef SENSOR
#define SENSOR
    
    typedef struct Sensor{
        int state;
        char id;
        int resetFlag;
    }Sensor;

    void* startSensor(void*);
#endif
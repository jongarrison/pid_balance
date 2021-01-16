#ifndef PID_GLOBALS_H
#define PID_GLOBALS_H

#include <Arduino.h>
#include <PID_v1.h>

namespace pid_globals {

    extern int version;
    
    //Define Variables we'll be connecting to
    extern double pid_setpoint; 
    extern double pid_input; 
    extern double pid_output;

    //Specify the links and initial tuning parameters
    extern double Kp;
    extern double Ki;
    extern double Kd;

    extern PID myPID;

    extern boolean is_pid_enabled;
    extern boolean is_pid_tuner_enabled;

};

#endif 
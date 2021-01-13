
#include "pid_globals.h"

namespace pid_globals {

    double pid_setpoint; 
    double pid_input; 
    double pid_output;

    //Specify the links and initial tuning parameters
    double Kp = 0.2;
    double Ki = 0.3;
    double Kd = 0.1;

    PID myPID(&pid_input, &pid_output, &pid_setpoint, Kp, Ki, Kd, DIRECT);

    boolean is_pid_enabled = true;
    boolean is_pid_tuner_enabled = false;

}
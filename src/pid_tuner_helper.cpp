#include "pid_tuner_helper.h"

namespace pid_tuner_helper {

  byte previousAutoTuneMode=2;
  //double kpmodel=1.5; //taup=100;

  double aTuneStep=50;
  double aTuneNoise=1; 
  double aTuneStartValue=100;

  unsigned int aTuneLookBack=20;

  boolean tuning = false;
  unsigned long serialTime;

  PID_ATune aTune(&pid_globals::pid_input, &pid_globals::pid_output);

  void autoTunePersistence(boolean start);
  void SerialSend();
  void toggleAutoTuneEnabled();

  void setup() {
    //Setup the pid 
    pid_globals::myPID.SetMode(AUTOMATIC);

    if(tuning) {
      tuning = false;
      toggleAutoTuneEnabled();
      tuning = true;
    }
    
    serialTime = 0;
  }

  void loop() {
    pid_globals::pid_input = hardware_helper::get_distance(false);

    if (tuning) {
      byte val = (aTune.Runtime());
      if (val!=0) {
        tuning = false;
      }

      if (!tuning) { //we're done, set the tuning parameters
        double kp = aTune.GetKp();
        double ki = aTune.GetKi();
        double kd = aTune.GetKd();

        Serial.println("New Auto Tuned Parameters:");
        Serial.print("kp: "); Serial.print(kp); Serial.print(" ");
        Serial.print("ki: "); Serial.print(ki); Serial.print(" ");
        Serial.print("kd: "); Serial.print(kd); Serial.println();

        pid_globals::myPID.SetTunings(kp, ki, kd);
        autoTunePersistence(false);
      }
    } else {

      pid_globals::myPID.Compute();
      
    }

    hardware_helper::servo_go(pid_globals::pid_output);
    
    //Put out a log message if it's time
    if(millis()>serialTime) {
      SerialSend();
      serialTime+=500;
    }
  }

  void toggleAutoTuneEnabled() {
    if(!tuning) {

      //Set the output to the desired starting frequency.
      pid_globals::pid_output = aTuneStartValue;
      aTune.SetNoiseBand(aTuneNoise);
      aTune.SetOutputStep(aTuneStep);
      aTune.SetLookbackSec((int)aTuneLookBack);
      autoTunePersistence(true);
      tuning = true;

    } else { //cancel autotune

      aTune.Cancel();
      tuning = false;
      autoTunePersistence(false);

    }
  }

  void autoTunePersistence(boolean start) {
    if(start) {
      previousAutoTuneMode = pid_globals::myPID.GetMode();
    } else {
      pid_globals::myPID.SetMode(previousAutoTuneMode);
    }
  }

  void SerialSend() {
    Serial.print("setpoint: ");Serial.print(pid_globals::pid_setpoint); Serial.print(" ");
    Serial.print("input: ");Serial.print(pid_globals::pid_input); Serial.print(" ");
    Serial.print("output: ");Serial.print(pid_globals::pid_output); Serial.print(" ");
    if(tuning) {
      Serial.println("tuning mode");
    } else {
      Serial.print("kp: ");Serial.print(pid_globals::myPID.GetKp());Serial.print(" ");
      Serial.print("ki: ");Serial.print(pid_globals::myPID.GetKi());Serial.print(" ");
      Serial.print("kd: ");Serial.print(pid_globals::myPID.GetKd());Serial.println();
    }
  }

}
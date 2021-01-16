#include "pid_tuner_helper.h"

namespace pid_tuner_helper {

  byte previousAutoTuneMode = AUTOMATIC;

  double aTuneStep = 50;
  double aTuneNoise = 1; 
  double aTuneStartValue = 100;
  unsigned int aTuneLookBack = 20;

  unsigned long serialTime;

  PID_ATune aTune(&pid_globals::pid_input, &pid_globals::pid_output);

  void serialSend();

  void setup() {
    previousAutoTuneMode = pid_globals::myPID.GetMode();

    //Setup the existing pid 
    pid_globals::myPID.SetMode(AUTOMATIC);

    //Set the output to the desired starting frequency.
    pid_globals::pid_output = aTuneStartValue;
    aTune.SetNoiseBand(aTuneNoise);
    aTune.SetOutputStep(aTuneStep);
    aTune.SetLookbackSec((int)aTuneLookBack);
    
    serialTime = 0;
  }

  /**
   * Returns false when done, calling code will need to respond accordingly
   */
  boolean loop() {

    pid_globals::pid_input = hardware_helper::get_distance(false);

    if (aTune.Runtime() != 0) {
      //then we're done

        pid_globals::Kp = aTune.GetKp();
        pid_globals::Ki = aTune.GetKi();
        pid_globals::Kd = aTune.GetKd();
        aTune.Cancel();

        Serial.println("New Final Auto Tuned Parameters:");
        Serial.print("kp: "); Serial.print(pid_globals::Kp); Serial.print(" ");
        Serial.print("ki: "); Serial.print(pid_globals::Ki); Serial.print(" ");
        Serial.print("kd: "); Serial.print(pid_globals::Kd); Serial.println();

        pid_globals::myPID.SetTunings(pid_globals::Kp, pid_globals::Ki, pid_globals::Kd);
        pid_globals::myPID.SetMode(previousAutoTuneMode);

        return false;
    }
    
    hardware_helper::servo_go(pid_globals::pid_output);

    //Put out a log message if it's time
    if (millis() > serialTime) {
      serialSend();
      serialTime += 500;
    }

    return true;
  }

  void serialSend() {
        Serial.println("In Progress AutoTuning Parameters:");
        Serial.print("kp: "); Serial.print(aTune.GetKp()); Serial.print(" ");
        Serial.print("ki: "); Serial.print(aTune.GetKi()); Serial.print(" ");
        Serial.print("kd: "); Serial.print(aTune.GetKd()); Serial.println();
  }

}
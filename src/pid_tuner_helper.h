#ifndef pid_tuner_helper_h
#define pid_tuner_helper_h

#include <Arduino.h>
#include <PID_AutoTune_v0.h>

#include "pid_globals.h"
#include "hardware_helper.h"

namespace pid_tuner_helper {
  void setup();
  boolean loop();
}

#endif
#include <Arduino.h>
#include <StreamCommandParser.h>

#include "pid_tuner_helper.h"
#include "pid_globals.h"
#include "hardware_helper.h"
#include "cmd_handlers.h"

StreamCommandParser commandParser(Serial, "serialCommandParser");

void setup() {
  Serial.begin(9600);

  hardware_helper::setup();

  //initialize the variables we're linked to
  pid_globals::pid_input = hardware_helper::get_distance(false);
  pid_globals::pid_setpoint = 10;

  //setup all the serial commands
  commandParser.addCommand("servo", cmd_handlers::servo);
  commandParser.addCommand("dist", cmd_handlers::dist);
  commandParser.addCommand("start", cmd_handlers::start);
  commandParser.addCommand("stop", cmd_handlers::stop);
  commandParser.addCommand("set", cmd_handlers::set);
  commandParser.addCommand("v", cmd_handlers::v);
  commandParser.addCommand("gpid", cmd_handlers::gpid);
  commandParser.addCommand("spid", cmd_handlers::spid);
  commandParser.addCommand("help", cmd_handlers::help);

  //turn the PID on
  pid_globals::myPID.SetMode(AUTOMATIC);
  pid_globals::is_pid_enabled = true;
}

static bool pid_changed = false;

void loop() {
  if (pid_globals::is_pid_tuner_enabled) {

    //during the tuning process we let the tuner do all of the work
    pid_globals::is_pid_tuner_enabled = pid_tuner_helper::loop();

  } else if (pid_globals::is_pid_enabled) {

    pid_globals::pid_input = hardware_helper::get_distance(false);
    pid_changed = pid_globals::myPID.Compute();
    hardware_helper::servo_go(pid_globals::pid_output);

  }
  
  if (!pid_globals::is_pid_tuner_enabled) {
    commandParser.readSerial(Serial);
  }
}
#include <Arduino.h>
#include <StreamCommandParser.h>

#include "pid_tuner_helper.h"
#include "pid_globals.h"
#include "hardware_helper.h"

int version = 1;

StreamCommandParser commandParser(Serial, "serialCommandParser");

void cmd_help_handler(StreamCommandParser& commandParser) {
  commandParser.printAvailableCommands(Serial);
}

void cmd_servo_handler(StreamCommandParser& commandParser) {
  uint8_t servoto = atoi(commandParser.next());
  hardware_helper::servo_go(servoto);
  commandParser.preferredResponseStream.println("received servo goto: " + String(servoto));
}

void cmd_dist_handler(StreamCommandParser& commandParser) {
  commandParser.preferredResponseStream.println("Distance: " + String(hardware_helper::get_distance(false)) + " Raw: " + String(hardware_helper::get_distance(true)));
}

void cmd_v_handler(StreamCommandParser& commandParser) {
  commandParser.preferredResponseStream.println("Version: " + String(version));
}

void cmd_tune_handler(StreamCommandParser& commandParser) {
  pid_globals::is_pid_enabled = true;
  pid_tuner_helper::setup();
}

void cmd_stop_handler(StreamCommandParser& commandParser) {
  pid_globals::is_pid_enabled = false;
  pid_globals::myPID.SetMode(MANUAL);
  Serial.println("Stopped");
}

void cmd_start_handler(StreamCommandParser& commandParser) {
  pid_globals::is_pid_enabled = true;
  pid_globals::myPID.SetMode(AUTOMATIC);
  Serial.println("Started");
}

void cmd_set_handler(StreamCommandParser& commandParser) {
    uint8_t distto = atoi(commandParser.next());
    pid_globals::pid_setpoint = distto;
    commandParser.preferredResponseStream.println("Set pid_setpoint: " + String(distto));
}

void cmd_gpid_handler(StreamCommandParser& commandParser) {
    commandParser.preferredResponseStream.println("Kp " + String(pid_globals::Kp) + " Ki " + String(pid_globals::Ki) + " Kd " + String(pid_globals::Kd));
}

void cmd_spid_handler(StreamCommandParser& commandParser) {
    commandParser.preferredResponseStream.println("Current: spid " + String(pid_globals::Kp) + " " + String(pid_globals::Ki) + " " + String(pid_globals::Kd));
    pid_globals::Kp = atof(commandParser.next());
    pid_globals::Ki = atof(commandParser.next());
    pid_globals::Kd = atof(commandParser.next());
    commandParser.preferredResponseStream.println("New: spid " + String(pid_globals::Kp) + " " + String(pid_globals::Ki) + " " + String(pid_globals::Kd));
}

void setup() {
  Serial.begin(9600);

  hardware_helper::setup();

  //initialize the variables we're linked to
  pid_globals::pid_input = hardware_helper::get_distance(false);
  pid_globals::pid_setpoint = 10;

  //setup all the commands
  commandParser.addCommand("servo", cmd_servo_handler);
  commandParser.addCommand("dist", cmd_dist_handler);
  commandParser.addCommand("start", cmd_start_handler);
  commandParser.addCommand("stop", cmd_stop_handler);
  commandParser.addCommand("set", cmd_set_handler);
  commandParser.addCommand("v", cmd_v_handler);
  commandParser.addCommand("gpid", cmd_gpid_handler);
  commandParser.addCommand("spid", cmd_spid_handler);
  commandParser.addCommand("help", cmd_help_handler);

  //turn the PID on
  pid_globals::myPID.SetMode(AUTOMATIC);
  pid_globals::is_pid_enabled = true;
}

static bool pid_changed = false;

void loop() {
  if (pid_globals::is_pid_tuner_enabled) {

    pid_globals::pid_input = hardware_helper::get_distance(false);
    pid_tuner_helper::loop();
    hardware_helper::servo_go(pid_globals::pid_output);

  } else if (pid_globals::is_pid_enabled) {

    pid_globals::pid_input = hardware_helper::get_distance(false);
    pid_changed = pid_globals::myPID.Compute();
    hardware_helper::servo_go(pid_globals::pid_output);

  } else if (!pid_globals::is_pid_tuner_enabled) {

    commandParser.readSerial(Serial);
  
  }
}
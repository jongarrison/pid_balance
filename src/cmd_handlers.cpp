#include "cmd_handlers.h"

namespace cmd_handlers {

    void help(StreamCommandParser& commandParser) {
    commandParser.printAvailableCommands(Serial);
    }

    void servo(StreamCommandParser& commandParser) {
    uint8_t servoto = atoi(commandParser.next());
    hardware_helper::servo_go(servoto);
    commandParser.preferredResponseStream.println("received servo goto: " + String(servoto));
    }

    void dist(StreamCommandParser& commandParser) {
    commandParser.preferredResponseStream.println("Distance: " + String(hardware_helper::get_distance(false)) + " Raw: " + String(hardware_helper::get_distance(true)));
    }

    void v(StreamCommandParser& commandParser) {
    commandParser.preferredResponseStream.println("Version: " + String(pid_globals::version));
    }

    void tune(StreamCommandParser& commandParser) {
    pid_globals::is_pid_enabled = true;
    pid_tuner_helper::setup();
    }

    void stop(StreamCommandParser& commandParser) {
    pid_globals::is_pid_enabled = false;
    pid_globals::myPID.SetMode(MANUAL);
    Serial.println("Stopped");
    }

    void start(StreamCommandParser& commandParser) {
    pid_globals::is_pid_enabled = true;
    pid_globals::myPID.SetMode(AUTOMATIC);
    Serial.println("Started");
    }

    void set(StreamCommandParser& commandParser) {
        uint8_t distto = atoi(commandParser.next());
        pid_globals::pid_setpoint = distto;
        commandParser.preferredResponseStream.println("Set pid_setpoint: " + String(distto));
    }

    void gpid(StreamCommandParser& commandParser) {
        commandParser.preferredResponseStream.println("Kp " + String(pid_globals::Kp) + " Ki " + String(pid_globals::Ki) + " Kd " + String(pid_globals::Kd));
    }

    void spid(StreamCommandParser& commandParser) {
        commandParser.preferredResponseStream.println("Current: spid " + String(pid_globals::Kp) + " " + String(pid_globals::Ki) + " " + String(pid_globals::Kd));
        pid_globals::Kp = atof(commandParser.next());
        pid_globals::Ki = atof(commandParser.next());
        pid_globals::Kd = atof(commandParser.next());
        commandParser.preferredResponseStream.println("New: spid " + String(pid_globals::Kp) + " " + String(pid_globals::Ki) + " " + String(pid_globals::Kd));
    }


}

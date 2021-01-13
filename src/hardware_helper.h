#ifndef HARDWARE_HELPER_H
#define HARDWARE_HELPER_H

#include <Arduino.h>
#include <Servo.h>

#define SERVO_PIN 9
#define ECHO_PIN 10
#define TRIG_PIN 11

#define SERVO_MIN_PWM 1200 //actual min is around 800, 1300 pretty good
//1450 middle
#define SERVO_MAX_PWM 1600 //actual min is around 2100, 1600 pretty good,

#define SERVO_MIN_DEG 45 //actual min is around 0
#define SERVO_MAX_DEG 120 //actual min is around 180

namespace hardware_helper {
    void setup();
    long get_distance(bool raw);
    void servo_go(uint8_t move_amount);
}

#endif

#include "hardware_helper.h"

namespace hardware_helper {

    Servo servoRail;

    void setup() {
        pinMode(SERVO_PIN, OUTPUT);
        pinMode(LED_BUILTIN, OUTPUT);
        pinMode(TRIG_PIN, OUTPUT);
        pinMode(ECHO_PIN, INPUT);

        servoRail.attach(SERVO_PIN);
    }

    void servo_go(uint8_t move_amount) {
        servoRail.write(map(move_amount, 0, 255, SERVO_MIN_DEG, SERVO_MAX_DEG));
        //servoRail.write(map(move_amount, 0, 255, SERVO_MIN_PWM, SERVO_MAX_PWM));
    }

    long get_distance(bool raw) {
        long duration;
        int distance;

        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2);
        // Sets the trigPin on HIGH state for 10 micro seconds
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);

        // Reads the echoPin, returns the sound wave travel time in microseconds
        duration = pulseIn(ECHO_PIN, HIGH);
        // Calculating the distance
        distance = duration * 0.034 / 2;
        // Prints the distance on the Serial Monitor

        if (raw == false) {
            if (distance > 1000) {
            distance = 4;
            } else if (distance > 24) {
            distance = 24;
            }
        }
        return distance;
    }




}
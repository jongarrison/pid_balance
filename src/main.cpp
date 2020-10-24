
#include <Arduino.h>
#include <PID_v1.h>
#include <ServoTimer2.h>

#define SERVO_PIN 9
#define ECHO_PIN 10
#define TRIG_PIN 11

#define SERVO_MIN_PWM 800
#define SERVO_MAX_PWM 2100

//Define Variables we'll be connecting to
double pid_setpoint, pid_input, pid_output;

//Specify the links and initial tuning parameters
PID myPID(&pid_input, &pid_output, &pid_setpoint, 2, 5, 1, DIRECT);
ServoTimer2 servoRail;

long get_distance() {
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

  if (distance > 1000) {
    distance = 4;
  } else if (distance > 24) {
    distance = 24;
  }
  Serial.print("Distance: ");
  Serial.println(distance);

  return distance;
}

void serve_go_percentage(uint8_t percentage_up) {
  servoRail.write(map(percentage_up, 1, 100, SERVO_MIN_PWM, SERVO_MAX_PWM));
}

void setup() {
  Serial.begin(9600);

  pinMode(SERVO_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  servoRail.attach(SERVO_PIN);

  //initialize the variables we're linked to
  pid_input = get_distance();
  pid_setpoint = 12;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
}

void loop() {
  //digitalWrite(LED_BUILTIN, HIGH);

  pid_input = get_distance();
  myPID.Compute();

  Serial.println("pid_input: " + String(pid_input) + " pid_output: " + String(pid_output));
  serve_go_percentage(pid_output);
}
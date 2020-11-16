#include <Arduino.h>
#include <PID_v1.h>
#include <Servo.h>
#include <StreamCommandParser.h>

#define SERVO_PIN 9
#define ECHO_PIN 10
#define TRIG_PIN 11

#define SERVO_MIN_PWM 1300 //actual min is around 800
//1450 middle
#define SERVO_MAX_PWM 1600 //actual min is around 2100

#define SERVO_MIN_DEG 45 //actual min is around 0
#define SERVO_MAX_DEG 120 //actual min is around 180


//Define Variables we'll be connecting to
double pid_setpoint, pid_input, pid_output;
int version = 1;
//Specify the links and initial tuning parameters
//double Kp=2, Ki=5, Kd=1;
double Kp = 2;
double Ki = 3;
double Kd = 1;
PID myPID(&pid_input, &pid_output, &pid_setpoint, Kp, Ki, Kd, DIRECT);
StreamCommandParser commandParser(Serial, "serialCommandParser");
Servo servoRail;
boolean isPidEnabled = true;

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

void serve_go(uint8_t move_amount) {
  servoRail.write(map(move_amount, 0, 255, SERVO_MIN_DEG, SERVO_MAX_DEG));
  //servoRail.write(map(move_amount, 0, 255, SERVO_MIN_PWM, SERVO_MAX_PWM));
}

void cmd_help_handler(StreamCommandParser& commandParser) {
  commandParser.printAvailableCommands(Serial);
}

void cmd_servo_handler(StreamCommandParser& commandParser) {
    uint8_t servoto = atoi(commandParser.next());
    serve_go(servoto);
    commandParser.preferredResponseStream.println("received servo goto: " + String(servoto));
}

void cmd_dist_handler(StreamCommandParser& commandParser) {
    commandParser.preferredResponseStream.println("Distance: " + String(get_distance(false) + " Raw: " + String(get_distance(true))));
}

void cmd_v_handler(StreamCommandParser& commandParser) {
    commandParser.preferredResponseStream.println("Version: " + String(version));
}

void cmd_stop_handler(StreamCommandParser& commandParser) {
  isPidEnabled = false;
  myPID.SetMode(MANUAL);
  Serial.println("Stopped");
}

void cmd_start_handler(StreamCommandParser& commandParser) {
  isPidEnabled = true;
  myPID.SetMode(AUTOMATIC);
  Serial.println("Started");
}

void cmd_set_handler(StreamCommandParser& commandParser) {
    uint8_t distto = atoi(commandParser.next());
    pid_setpoint = distto;
    commandParser.preferredResponseStream.println("Set pid_setpoint: " + String(distto));
}

void cmd_gpid_handler(StreamCommandParser& commandParser) {
    commandParser.preferredResponseStream.println("Kp " + String(Kp) + " Ki " + String(Ki) + " Kd " + String(Kd));
}

void cmd_spid_handler(StreamCommandParser& commandParser) {
    commandParser.preferredResponseStream.println("Current Kp: " + String(Kp) + " Ki: " + String(Ki) + " Kd: " + String(Kd));
    Kp = atof(commandParser.next());
    Ki = atof(commandParser.next());
    Kd = atof(commandParser.next());
    commandParser.preferredResponseStream.println("New Kp: " + String(Kp) + " Ki: " + String(Ki) + " Kd: " + String(Kd));
}

void setup() {
  Serial.begin(9600);

  pinMode(SERVO_PIN, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  servoRail.attach(SERVO_PIN);

  //initialize the variables we're linked to
  pid_input = get_distance(false);
  pid_setpoint = 10;

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
  myPID.SetMode(AUTOMATIC);
  isPidEnabled = true;
}

static bool pid_changed = false;

void loop() {
  commandParser.readSerial(Serial);

  if (isPidEnabled) {
    pid_input = get_distance(false);
    pid_changed = myPID.Compute();
    serve_go(pid_output);
  }
  // Serial.println("pid_input: " + String(pid_input) + " pid_output: " + String(pid_output));
}
#include <Arduino.h>
#include <Servo.h>
#include "servo_controller.h"
#include "stepper_controller.h"
#include <EEPROM.h>

#define SERVO_ATTACHMENT_PIN 6

Servo servo_motor;
AccelStepper stepper_motor;

Servo_controller servo_controller;
Stepper_controller stepper_controller;

void setup() {
  Serial.begin(9600);
  servo_controller.attach(SERVO_ATTACHMENT_PIN);
  servo_controller.move_to_origin();
  // servo_controller.test_routine();

  stepper_controller.attach(11, 9, 10, 8);
  // stepper_controller.move_to_origin();

  // test for writing to EEPROM
  /*
  Values stored on the EEPROM are stored as 8bit, so to read them we're gonna have to do some fucky wucky
  bit manipulation BS or find a library to do it for us
  though bit manipulation is probably as easy as doing `output << 8, 16, 24, 32` etc
  */
  // EEPROM.write(0, 127);
}

void read_servo_input();

void read_stepper_input();

void handle_stepper_movement();

void loop() {
  // Serial.print(EEPROM.read(0));
  handle_stepper_movement();

  if (!Serial.available()) {
    return;
  }

  // read_servo_input();
  read_stepper_input();
}

void read_servo_input()
{
  String input_string;
  input_string = Serial.readStringUntil('\n');
  input_string.trim();
  servo_controller.take_serial_input(input_string);
}

void read_stepper_input()
{
  String input_string;
  input_string = Serial.readStringUntil('\n');
  input_string.trim();
  stepper_controller.take_serial_input(input_string);
}

void handle_stepper_movement()
{
  stepper_controller.handle_movement();
}

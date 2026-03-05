#include <Arduino.h>
#include <Servo.h>
#include "servo_controller.h"
#include "stepper_controller.h"

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
}

void read_servo_input();

void read_stepper_input();

void handle_stepper_movement();

void loop() {
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

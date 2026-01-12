#include <Arduino.h>
#include <Servo.h>
#include "servo_controller.h"

#define SERVO_ATTACHMENT_PIN 9

Servo servo_motor;

Servo_controller servo_controller;

void setup() {
  Serial.begin(9600);
  servo_controller.attach(SERVO_ATTACHMENT_PIN);
  servo_controller.move_to_origin();
  // servo_controller.test_routine();
}

void loop() {
  if (!Serial.available()) {
    return;
  }

  String input_string;
  input_string = Serial.readStringUntil('\n');
  input_string.trim();
  servo_controller.take_serial_input(input_string);
}

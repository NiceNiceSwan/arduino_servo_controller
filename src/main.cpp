#include <Arduino.h>
#include <Servo.h>
#include <AccelStepper.h>
#include "servo_controller.h"

#define VRX_PIN  A1 // Arduino pin connected to VRX pin
#define VRY_PIN  A0 // Arduino pin connected to VRY pin

#define SERVO_ATTACHMENT_PIN 9

#define FULLSTEP 4
#define STEP_PER_REVOLUTION 2048

int xValue = 0; // To store value of the X axis
int yValue = 0; // To store value of the Y axis

Servo servo_motor;
AccelStepper stepper(FULLSTEP, 11, 9, 10, 8);

Servo_controller servo_controller;

void read_potentiometer();
void drive_servo_from_potentiometer();
void setup_stepper_motor();
void drive_stepper_motor();

void setup() {
  Serial.begin(9600);
  servo_controller.attach(SERVO_ATTACHMENT_PIN);
  servo_controller.move_to_origin();
  delay(5000);
  servo_controller.test_routine();
  // servo_motor.attach(9);
  // servo_motor.write(0);
  // setup_stepper_motor();
}

void loop() {
  // delay(5000);
  // servo_controller.test_routine();
}

/*
  // read_potentiometer();
  // drive_servo_from_potentiometer();
  drive_stepper_motor();

  // Serial.print("written value = ");
  // Serial.println(x_angle);

  // servo_motor.write(x_angle);
  // delay(100);

  // servo_motor.write(0);
  // delay(1000);
  // servo_motor.write(30);
  // for (size_t i = 0; i < 6; i++)
  // {
  //   servo_motor.write(i * 30);
  //   delay(5000);
  // }
  // delay(5000);
*/

void read_potentiometer()
{
  int analog_value = analogRead(A5);
  Serial.print("Potentiometer value = ");
  Serial.println(analog_value);
}

void drive_servo_from_potentiometer()
{
  int potentiometer_input = analogRead(A5);
  potentiometer_input = 1023 - potentiometer_input;
  static int last_signal = 0;
  int angle = map(potentiometer_input, 0, 1023, 20, 160);
  if (last_signal == angle)
  {
    delay(800);
    return;
  }
  
  last_signal = angle;
  servo_motor.write(angle);
  delay(800);
}

void setup_stepper_motor()
{
  stepper.setMaxSpeed(1000.0);   // set the maximum speed
  stepper.setAcceleration(50.0); // set acceleration
  stepper.setSpeed(200);         // set initial speed
  stepper.setCurrentPosition(0); // set position
  // stepper.moveTo(STEP_PER_REVOLUTION); // set target position: 64 steps <=> one revolution
}

void drive_stepper_motor()
{
  // delay(150);
  int potentiometer_input = analogRead(A5);
  potentiometer_input = 1023 - potentiometer_input;
  static int last_position = 0;
  int desired_position = map(potentiometer_input, 0, 1023, 0, STEP_PER_REVOLUTION);

  // Serial.print("Potentiometer reading = ");
  // Serial.println(potentiometer_input);
  // Serial.print("desired position = ");
  // Serial.println(desired_position);

  if (stepper.currentPosition() == desired_position)
  {
    // stepper.moveTo(desired_position);
    stepper.stop();
    return;
  }
  

  if (last_position == desired_position)
  {
    stepper.run();
    return;
  }
  
  last_position = desired_position;
  stepper.moveTo(desired_position);
  stepper.run();
}

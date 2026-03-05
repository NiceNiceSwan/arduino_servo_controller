#include "stepper_controller.h"

int Stepper_controller::_clamp(int value, int min_, int max_)
{
    value = min(value, max_);
    value = max(value, min_);
    return value;
}

Stepper_controller::Stepper_controller(int pin1, int pin2, int pin3, int pin4)
{
    attach(pin1, pin2, pin3, pin4);
}

Stepper_controller::Stepper_controller()
{

}

Stepper_controller::~Stepper_controller()
{
}

void Stepper_controller::attach(int pin1, int pin2, int pin3, int pin4)
{
    _stepper_motor = AccelStepper(AccelStepper::FULL4WIRE, pin1, pin2, pin3, pin4);
    _stepper_motor.setMaxSpeed(MOTOR_MAX_SPEED);
    _stepper_motor.setAcceleration(MOTOR_ACCELERATION);
    _stepper_motor.setSpeed(MOTOR_MAIN_SPEED);
    _moving = false;
}

void Stepper_controller::move_to_position(int position)
{
    _target_position = _logical_to_physical(position);
    _stepper_motor.moveTo(_target_position);
    // _stepper_motor.setSpeed(_target_position > 0 ? MOTOR_MAIN_SPEED : -MOTOR_MAIN_SPEED);
    // Serial.print("Position:");
    // Serial.print(_logical_position());
    // Serial.println(" steps");
}

void Stepper_controller::move_by_amount(int amount)
{
    _target_position = _stepper_motor.currentPosition() + amount;
    _stepper_motor.moveTo(_target_position);
    // _stepper_motor.setSpeed(_target_position > 0 ? MOTOR_MAIN_SPEED : -MOTOR_MAIN_SPEED);
}

long Stepper_controller::_logical_position() {
  // Logical coordinates are inverted vs. motor direction and referenced to originOffsetSteps
    return _stepper_motor.currentPosition() - _origin_offset_steps;
}

long Stepper_controller::_logical_to_physical(long logicalSteps) {
  // Convert desired logical position to physical steps for AccelStepper
    return _origin_offset_steps - logicalSteps;
}

bool Stepper_controller::take_serial_input(String input)
{
    if (input.startsWith(STEPPER_MOVE_ABSOLUTE))
    {
        long target_position = input.substring(2).toInt();
        move_to_position(target_position);
        return true;
    }
    if (input.startsWith(STEPPER_MOVE_RELATIVE))
    {
        int move_amount = input.substring(2).toInt();
        move_by_amount(move_amount);
        return true;
    }
    if (input.startsWith(STEPPER_MOVE_TO_ORIGIN))
    {
        move_to_position(0);
        return true;
    }
    if (input.startsWith(STEPPER_TEST_ROUTINE))
    {
        // test_routine();
        return true;
    }
    return false;
}

void Stepper_controller::handle_movement()
{
    if (_stepper_motor.currentPosition() != _target_position)
    {
        Serial.print("current Position:");
        Serial.print(_stepper_motor.currentPosition());
        digitalWrite(ENABLE_PIN_MAIN_MOTOR, LOW);
        _stepper_motor.runSpeedToPosition();
        digitalWrite(ENABLE_PIN_MAIN_MOTOR, HIGH);
    }
}

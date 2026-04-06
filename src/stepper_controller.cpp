#include "stepper_controller.h"

int Stepper_controller::_clamp(int value, const int &min_, const int &max_)
{
    value = min(value, max_);
    value = max(value, min_);
    return value;
}

Stepper_controller::Stepper_controller(AccelStepper stepper)
{
    _stepper_motor = stepper;
}

Stepper_controller::Stepper_controller(int pin1, int pin2)
{
    attach(pin1, pin2);
}

Stepper_controller::Stepper_controller()
{

}

Stepper_controller::~Stepper_controller()
{
}

void Stepper_controller::attach(int pin1, int pin2)
{
    _stepper_motor = AccelStepper(MOTOR_INTERFACE_TYPE, pin1, pin2);
    _stepper_motor.setMaxSpeed(MOTOR_MAX_SPEED);
    _stepper_motor.setAcceleration(MOTOR_ACCELERATION);
    pinMode(ENABLE_PIN_STEPPER_MOTOR, OUTPUT);
    digitalWrite(ENABLE_PIN_STEPPER_MOTOR, HIGH);
    _moving = false;
}

void Stepper_controller::move_to_position(int position)
{
    _target_position = _logical_to_physical(position);
    _stepper_motor.moveTo(_target_position);
    _stepper_motor.setSpeed(_target_position > 0 ? MOTOR_MAIN_SPEED : -MOTOR_MAIN_SPEED);
}

void Stepper_controller::move_by_amount(int amount)
{
    _target_position = _stepper_motor.currentPosition() - amount;
    _stepper_motor.moveTo(_target_position);
    _stepper_motor.setSpeed(_target_position > 0 ? MOTOR_MAIN_SPEED : -MOTOR_MAIN_SPEED);
}

void Stepper_controller::set_origin()
{
    _origin_offset_steps = _stepper_motor.currentPosition();
}

void Stepper_controller::test_routine()
{
    Serial.print("Starting test routine\n");
    Serial.print("Move to position: 5000\n");
    move_to_position(5000);
    priority_move();
    delay(2000);
    Serial.print("Move to position: 0\n");
    move_to_position(0);
    priority_move();
    delay(2000);
    Serial.print("Move by amount: 5000\n");
    move_by_amount(5000);
    priority_move();
    delay(2000);
    Serial.print("Setting origin\n");
    set_origin();
    delay(2000);
    Serial.print("Move to position: -5000\n");
    move_to_position(-5000);
    priority_move();
    delay(2000);
    Serial.print("Move by amount: -1000\n");
    move_by_amount(-1000);
    priority_move();
    delay(2000);
    Serial.print("Move to position: 0\n");
    move_to_position(0);
    priority_move();
    delay(2000);
    Serial.print("Move by amount: -5000\n");
    move_by_amount(-5000);
    priority_move();
    delay(2000);
    Serial.print("Setting origin\n");
    set_origin();
    Serial.print("Test routine finished\n");
}

long Stepper_controller::_logical_position() {
    return _stepper_motor.currentPosition() - _origin_offset_steps;
}

long Stepper_controller::_logical_to_physical(long logicalSteps) {
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
        test_routine();
        return true;
    }
    return false;
}

void Stepper_controller::handle_movement()
{
    if (_stepper_motor.currentPosition() != _target_position)
    {
        // Serial.print("current Position:");
        // Serial.print(_stepper_motor.currentPosition());

        digitalWrite(ENABLE_PIN_STEPPER_MOTOR, LOW);
        _stepper_motor.runSpeedToPosition();
        _moving = true;
        return;
    }
    _moving = false;
    digitalWrite(ENABLE_PIN_STEPPER_MOTOR, HIGH);
}

void Stepper_controller::priority_move()
{
    digitalWrite(ENABLE_PIN_STEPPER_MOTOR, LOW);
    while (_stepper_motor.currentPosition() != _target_position)
    {
        _stepper_motor.runSpeedToPosition();
    }
    digitalWrite(ENABLE_PIN_STEPPER_MOTOR, HIGH);
}


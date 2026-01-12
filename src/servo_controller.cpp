#include "servo_controller.h"

Servo_controller::Servo_controller(Servo servo)
{
    _servo_motor = servo;
}

Servo_controller::Servo_controller(int attachment_pin)
{
    _servo_motor.attach(attachment_pin);
}

Servo_controller::Servo_controller(){}

Servo_controller::~Servo_controller()
{
    _servo_motor.detach();
}


int Servo_controller::_clamp(int value, int min_, int max_)
{
    value = min(value, max_);
    value = max(value, min_);
    return value;
}

void Servo_controller::move_to_angle(int angle)
{
    angle = _clamp(angle, -90, 90);
    // angle = map(angle, -90, 90, 0, 180);
    angle += 90;
    _servo_motor.write(angle);
    // int pulse_width = map(angle, -180, 180, SERVO_FULL_LEFT, SERVO_FULL_RIGHT);
    // _servo_motor.writeMicroseconds(pulse_width);
}

void Servo_controller::move_to_origin()
{
    // _servo_motor.writeMicroseconds(600);
    _servo_motor.write(90);
}

void Servo_controller::move_by_angle(int angle)
{
    // PWM implementation
    // int pulse_width;
    // int current_position = _servo_motor.readMicroseconds();
    // int micro_seconds;

    // angle = _clamp(angle, -180, 180);
    // pulse_width = map(angle, -180, 180, -PULSE_WIDTH_RANGE / 2, PULSE_WIDTH_RANGE / 2);
    // micro_seconds = current_position + pulse_width;
    // micro_seconds = _clamp(micro_seconds, SERVO_FULL_LEFT, SERVO_FULL_RIGHT);
    // _servo_motor.writeMicroseconds(micro_seconds);

    // write angle implementation
    int current_pos = _servo_motor.read();
    int desired_pos = current_pos + angle;
    desired_pos = _clamp(desired_pos, 0, 180);
    _servo_motor.write(desired_pos);
}

void Servo_controller::test_routine()
{
    move_to_origin();
    delay(4000);
    move_to_angle(90);  // rotate max to the right
    delay(4000);
    move_to_angle(-90); // rotate max to the left
    delay(4000);
    move_to_angle(45);  // rotate to 45 degrees right of center
    delay(4000);
    move_to_angle(-45); // rotate to 45 degrees left of center
    delay(4000);
    move_by_angle(90);  // rotate right 90
    delay(4000);
    move_to_origin();   // center
    delay(4000);
    move_by_angle(90);  // rotate max to the right
    delay(4000);
    move_to_origin();   // center
    delay(4000);
}

bool Servo_controller::take_serial_input(String input)
{
    if (input.startsWith(SERVO_MOVE_ABSOLUTE))
    {
        int target_angle = input.substring(2).toInt();
        move_to_angle(target_angle);
        return true;
    }
    if (input.startsWith(SERVO_MOVE_RELATIVE))
    {
        int move_angle = input.substring(2).toInt();
        move_by_angle(move_angle);
        return true;
    }
    if (input.startsWith(SERVO_MOVE_TO_ORIGIN))
    {
        move_to_origin();
        return true;
    }
    if (input.startsWith(SERVO_TEST_ROUTINE))
    {
        test_routine();
        return true;
    }
    return false;
}

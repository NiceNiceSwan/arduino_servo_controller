#include <Arduino.h>
#include <AccelStepper.h>

/// moves the stepper to a set position from origin
#define STEPPER_MOVE_ABSOLUTE "ZA"
/// moves the stepper to a position relative to it's current one
#define STEPPER_MOVE_RELATIVE "ZR"
/// returns the stepper to the origin point
#define STEPPER_MOVE_TO_ORIGIN "ZH"
/// performs a test routine
#define STEPPER_TEST_ROUTINE "ZT"

#define STEP 1
#define HALFSTEP 2
#define FULLSTEP 4

#define DIR_PIN_MAIN_MOTOR 2
#define STEP_PIN_MAIN_MOTOR 3
#define ENABLE_PIN_MAIN_MOTOR 4
#define MOTOR_MAX_SPEED 10000
#define MOTOR_ACCELERATION 10000
#define MOTOR_MAIN_SPEED 10000
#define MOTOR_INTERFACE_TYPE 1


class Stepper_controller
{
private:
    // AccelStepper _stepper_motor(MOTOR_INTERFACE_TYPE, STEP_PIN_MAIN_MOTOR, DIR_PIN_MAIN_MOTOR);
    AccelStepper _stepper_motor;
    /// @brief clamps the value
    /// @param value value which you want clamped
    /// @param min minimum value this can be
    /// @param max maximum value this can be
    /// @return value clamped to range min and max, including min and max
    int _clamp(int value, int min, int max);

    int _attachment_pin1;
    int _attachment_pin2;
    int _attachment_pin3;
    int _attachment_pin4;
    
    long _origin_offset_steps = 0; // physical position that represents logical origin

    long _target_position = 0;

    /// @brief Is the motor moving right now
    bool _moving;

    long _logical_position();

    long _logical_to_physical(long logicalSteps);
public:
    Stepper_controller(AccelStepper stepper);
    Stepper_controller(int pin1, int pin2, int pin3, int pin4);
    Stepper_controller();
    ~Stepper_controller();

    void attach(int pin1, int pin2, int pin3, int pin4);

    void move_to_position(int position);

    void move_by_amount(int amount);

    void move_to_center();

    void test_routine();

    /// @brief takes string input from the serial port, and moves the stepper accordingly
    /// @param input string containing the command (2 letters, Z[A,R,H,T], followed by the number of steps). For example, ZA5000 moves the stepper forward 5000 steps
    /// @return true if the command was accepted, false if it wasn't
    bool take_serial_input(String input);

    /// @return true if the motor is currently moving, false otherwise
    bool moving() { return _moving; };

    void handle_movement();
};


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

#define ENABLE_PIN_STEPPER_MOTOR 4
#define MOTOR_MAX_SPEED 10000
#define MOTOR_ACCELERATION 10000
#define MOTOR_MAIN_SPEED 10000
#define MOTOR_INTERFACE_TYPE 1


class Stepper_controller
{
private:
    /// @brief our stepper motor
    AccelStepper _stepper_motor;
    /// @brief clamps the value
    /// @param value value which you want clamped
    /// @param min minimum value this can be
    /// @param max maximum value this can be
    /// @return value clamped to range min and max, including min and max
    int _clamp(int value, const int &min, const int &max);

    int _attachment_pin1;
    int _attachment_pin2;

    /// @brief physical position that represents logical origin
    long _origin_offset_steps = 0;

    /// @brief the position to which we want to move
    long _target_position = 0;

    /// @brief Is the motor moving right now
    bool _moving;

    /// @brief Logical coordinates are inverted vs. motor direction and referenced to originOffsetSteps
    long _logical_position();

    /// @brief Convert desired logical position to physical steps for AccelStepper
    long _logical_to_physical(long logicalSteps);
public:
    /// @brief if you already have an attached stepper
    /// @param stepper already attached stepper
    Stepper_controller(AccelStepper stepper);
    /// @brief attaches the stepper to pins
    Stepper_controller(int pin1, int pin2);
    /// @brief to initialize later. Note that as of writing this (10.03.2026, 20:08), this class does not check whether at any point the stepper is attached.
    Stepper_controller();
    ~Stepper_controller();

    /// @brief attach the stepper to the given pins
    void attach(int pin1, int pin2);

    /// @brief moves the stepper to the given position from origin
    /// @param position position from origin
    void move_to_position(int position);

    /// @brief moves the stepper the given amount from current position
    /// @param amount the amount by which to move the stepper
    void move_by_amount(int amount);

    /// @brief sets the current point as origin
    void set_origin();

    /// @brief simple test routine. This takes full control over the microcontroller as it executes
    void test_routine();

    /// @brief takes string input from the serial port, and moves the stepper accordingly
    /// @param input string containing the command (2 letters, Z[A,R,H,T], followed by the number of steps). For example, ZA5000 moves the stepper forward 5000 steps
    /// @return true if the command was accepted, false if it wasn't
    bool take_serial_input(String input);

    /// @return true if the motor is currently moving, false otherwise
    bool moving() { return _moving; };

    /// @brief moves the stepper a single step. Should be used in the main loop of the micro controller
    void handle_movement();

    /// @brief takes control of the micro controller until it reaches the desired position.
    void priority_move();
};


#include <Arduino.h>
#include <Servo.h>

/// moves the servo to a set position from origin
#define SERVO_MOVE_ABSOLUTE "SA"
/// moves the servo to a position relative to it's current one
#define SERVO_MOVE_RELATIVE "SR"
/// returns the servo to the origin point
#define SERVO_MOVE_TO_ORIGIN "SH"
/// performs a test routine
#define SERVO_TEST_ROUTINE "ST"

class Servo_controller
{
private:
    /// @brief our servo motor
    Servo _servo_motor;
    /// @brief clamps the value
    /// @param value value which you want clamped
    /// @param min minimum value this can be
    /// @param max maximum value this can be
    /// @return value clamped to range min and max, including min and max
    int _clamp(int value, int min, int max);
public:
    /// @brief if you already have an attached servo
    /// @param servo servo you have already attached
    Servo_controller(Servo servo);
    /// @brief if you don't have a servo defined yet
    /// @param attachment_pin pin to which the servo is attached
    Servo_controller(int attachment_pin);
    /// @brief to initialize later. Note that as of writing this (12.01.2026, 19:02), this class does not check whether at any point the servo is attached.
    Servo_controller();
    ~Servo_controller();

    // setters

    /// @param attachment_pin pin to which the servo is attached
    void attach(int attachment_pin) { _servo_motor.attach(attachment_pin); };
    void detach() { _servo_motor.detach(); };

    // getters

    /// @return angle at which the servo is 
    int position() { return _servo_motor.read(); };

    // functions

    /// @brief move to angle from -90 to 90
    /// @param angle angle to which we want the servo to move. Clamped between [-90, 90]
    void move_to_angle(int angle);
    /// @brief moves the servo to position 0 (middle)
    void move_to_origin();
    /// @brief moves the servo by the given amount
    /// @param angle amount by which to move the servo. Negative values rotate left, positive rotate right. Will be clamped to the servo's maximum rotation angle
    void move_by_angle(int angle);
    /**
     * @brief tests the control commands (`move_to_angle`, `move_to_origin`, `move_by_angle`).
     * Note that this command uses the delay functions, and will take complete control over the program
     * for as long as it is running.
     */
    void test_routine();
    /// @brief takes string input from the serial port, and moves the servo accordingly
    /// @param input string containing the command (2 letters, S[A,R,H,T], followed by the angle). For example, SA90 moves the servo 90 degrees right of center
    /// @return true if the command was accepted, false if it wasn't
    bool take_serial_input(String input);

};

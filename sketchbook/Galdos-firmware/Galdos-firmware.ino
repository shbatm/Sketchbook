/* 
    Galdos-firmware.ino
    By Duality.
*/

#include <Ticker.h>
#include <Servo.h>

// define servo pins (SPIN)
#define ROTATION_SPIN 4
#define BOTTOM_SPIN 5
#define HEAD_SPIN 15
#define BASE_SPIN 13

// keep an array of servos/pins.
#define NUM_SPINS 4
#define NUM_SERVOS 4
// keep track of which index is what.
enum {ROTATION=0, BOTTOM, HEAD, BASE};

/*
    structure that holds data on its current postion.
    it's goal positions.
    which Servo it is.
    and on which pin it is attached.
*/
typedef struct
{
    Servo servo;
    uint8_t pin;
    uint8_t goal;
    uint8_t current;
} servo_t;

servo_t servos[NUM_SPINS];

#define MAX_ROTATION 180
#define MIN_ROTATION 0
#define HALF_ROTATION (MAX_ROTATION/2)

// create tickers for setting servo speeds.
// speed is time/degree where time in milliseconds.
float servoSpeed = 1000/180;
Ticker servoSpeedControl;

void setupServos()
{
    // assign pins to the servos.
    servos[ROTATION].pin = ROTATION_SPIN;
    servos[BOTTOM].pin = BOTTOM_SPIN;
    servos[HEAD].pin = HEAD_SPIN;
    servos[BASE].pin = BASE_SPIN;
    // initialize servo pins to output
    // and attach the servos to each pin.
    for(int i = 0; i < NUM_SPINS; i++)
    {
        pinMode(servos[i].pin, OUTPUT);
        servos[i].servo.attach(servos[i].pin);
        servos[i].goal = HALF_ROTATION;
        servos[i].current = HALF_ROTATION;
    }
    // attach the servo control function with a speed.
    servoSpeedControl.attach(servoSpeed, controlServoPos);
}

void controlServoPos()
{

}

void centerServos()
{
    for(int i = 0; i < NUM_SPINS; i++)
    {
        servos[i].servo.write(HALF_ROTATION);
    }
}

void setServoGoal()
{

}

void setup()
{
    setupServos();
    centerServos();
}

void loop()
{
}

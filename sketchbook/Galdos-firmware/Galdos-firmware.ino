/* 
    Galdos-firmware.ino
    By Duality.
*/

#include <ESP8266WiFi.h>

#include <Ticker.h>
#include <Servo.h>

#include <otaupload.h>

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
// speed is time/degree where time in seconds
float servoSpeed = (3.0/180.0);
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
    uint8_t current;
    uint8_t goal;
    Servo servo;
    for(int i = 0; i < NUM_SERVOS; i++)
    {
        current = servos[i].current;
        goal = servos[i].goal;
        servo = servos[i].servo;

        // if servo already in position go to the next servo.
        if(current == goal)
        {
            continue;
        }
        // if servo position less then goal position increment
        // the servo position.
        else if(current < goal)
        {
            current++;
        }
        // if servo position
        else if(current > goal)
        {
            current--;
        }
        // store adjust value if adjusted
        servos[i].current = current;
        servo.write(current);
    }
}



void setServoGoal(int servoID, uint8_t newGoal)
{
    // check for valid values
    if(servoID < ROTATION || servoID > BASE)
    {
        return;
    }
    servos[servoID].goal = newGoal;
}

void centerServos()
{
    // center all the servo's
    for(int i = 0; i < NUM_SPINS; i++)
    {
        setServoGoal(i, HALF_ROTATION);
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("");

    setupOta();

    setupServos();
    centerServos();
}

void loop()
{
    setServoGoal(BASE, 0);
    setServoGoal(ROTATION, 0);
    setServoGoal(HEAD, 0);
    setServoGoal(BOTTOM, 0);
    delay(1000);
    setServoGoal(BASE, 180);
    setServoGoal(ROTATION, 180);
    setServoGoal(HEAD, 180);
    setServoGoal(BOTTOM, 180);
    delay(1000);
}

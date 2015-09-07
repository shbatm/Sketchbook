/* 
    Galdos-firmware.ino
    By Duality.
*/

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

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

// open udp port for servo controll.
// glados controle unit input.
WiFiUDP GCU;

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
    }
}

void updateServos()
{
    for(int i = 0; i < 4; i++)
    {
        servos[i].servo.write(servos[i].current);
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

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void handleGcuInput()
{

}

void setup()
{
    Serial.begin(115200);
    Serial.println("");

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.begin("www.tkkrlab.nl", "hax4or2the2paxor3");
    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    printWifiStatus();

    GCU.begin(1337);

    setupServos();
    centerServos();
}

void loop()
{
    handleGcuInput();
    updateServos();
}

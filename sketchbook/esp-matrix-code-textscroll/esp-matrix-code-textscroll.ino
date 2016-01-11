#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <SPI.h>

#define SCLK 14     // purple wire
#define R 13        // green wire

#define A 4         // orange wire
#define B 5         // yellow wire
#define C 16        // brown wire

/* Set these to your desired credentials. */
const char *ssid = "www.tkkrlab.nl";
const char *password = "hax4or2the2paxor3";

#define ota ArduinoOTA

#define FRAME_WIDTH (16)
#define FRAME_HEIGHT (8)
#define FRAMEBUFFERSIZE (FRAME_WIDTH * FRAME_HEIGHT)
uint8_t frame_buffer[FRAMEBUFFERSIZE];

void setrow(uint8_t row)
{
    // mask bits to be sure
    row = row & 0x07;
    digitalWrite(A, (row & 0x01));
    digitalWrite(B, (row & 0x02) >> 1);
    digitalWrite(C, (row & 0x04) >> 2);
}

void setupSpi()
{
    SPI.begin();
    SPI.setDataMode(SPI_MODE1);
    SPI.setClockDivider(SPI_CLOCK_DIV2);
}

void writeRow()
{
    static int row;
    setrow(row);

    uint8_t data = 0;
    for(int i = 0; i < FRAMEBUFFERSIZE; i+= 8)
    {
        
    }

    row++;
    row &= 0x07;
}

void setupOTA()
{
    ota.onStart([]()
    {
        Serial.println("Start");
    });

    ota.onEnd([]()
    {
        Serial.println("\nEnd");
    });

    ota.onProgress([](unsigned int progress, unsigned int total)
    {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });

    ota.onError([](ota_error_t error)
    {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ota.begin();
}

void setup()
{
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(SCLK, OUTPUT);
    pinMode(R, OUTPUT);

    // setrow(0x01);
    // setupSpi();

    Serial.begin(115200);
    Serial.println();

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("connecting: ");
    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println("Connected.");
    IPAddress myIP = WiFi.localIP();
    Serial.print("IP address: ");
    Serial.println(myIP);
    setupOTA();
    digitalWrite(R, 1);
}

void loop()
{
    digitalWrite(B, HIGH);
    digitalWrite(SCLK, HIGH);
    delay(1);
    digitalWrite(SCLK, LOW);
    delay(1);
    delay(100);
    // handle ota for firmware upload
    ota.handle();
}

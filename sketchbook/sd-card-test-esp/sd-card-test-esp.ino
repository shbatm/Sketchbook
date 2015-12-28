#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

#include <ArduinoOTA.h>

#include <SPI.h>
#include <SD.h>

#include "cmd.h"

const char* board_name = "3Dprinter";
const char* ssid = "ssid";
const char* pass = "pass";

Sd2Card card;
SdVolume volume;
SdFile root;
const int chipSelect = 4;

bool hasSD = false;
File uploadFile;
File index_html;
String html_root = "/html/";
String gcode_root = "/gcode/";
const char* serverIndex = "<form method='POST' action='/upload' enctype='multipart/form-data'><input type='file' name='upload'><input type='submit' value='upload'></form>";

ESP8266WebServer server(80);
ArduinoOTA ota_server(board_name, 8266, true);

void returnOK() {
    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/plain", "");
}

void upload()
{
    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/html", serverIndex);
}

void handleFileUpload()
{
    String content = "A test.";
    Serial.println(String("uri: ") + server.uri());
    if(server.uri() != "/upload") return;
    HTTPUpload& upload = server.upload();
    if(upload.status == UPLOAD_FILE_START)
    {
        String filepath = html_root + upload.filename;
        if(SD.exists((char *)filepath.c_str()))
        {
            SD.remove((char *)filepath.c_str());
        }
        uploadFile = SD.open(filepath.c_str(), FILE_WRITE);
        Serial.print("Upload: START, filename: ");
        Serial.println(filepath);
    }
    else if(upload.status == UPLOAD_FILE_WRITE)
    {
        if(uploadFile)
        {
            uploadFile.write(upload.buf, upload.currentSize);
            uploadFile.flush();
        }
        Serial.print("Upload: WRITE, Bytes: ");
        Serial.println(upload.currentSize);
    }
    else if(upload.status == UPLOAD_FILE_END)
    {
        if(uploadFile)
        {
            uploadFile.close();
        }
        Serial.print("Upload: END, Size: ");
        Serial.println(upload.totalSize);
    }

    server.send(200, "text/html", content);
}

String read_from_file(File file)
{
    String content;
    int data = '\0';
    while(file.available())
    {
        content += (char)file.read();
    }
    file.close();
    return content;
}

void handleServerRoot()
{
    // load index.html from SD card.
    String content;
    String index = html_root + "INDEX.HTM";
    Serial.printf("loading page from: %s\n", index.c_str());
    index_html = SD.open(index, FILE_READ);
    if(index_html)
    {
        int data = '\0';
        while(index_html.available())
        {
            content += (char)index_html.read();
        }
        index_html.close();
    }
    else
    {
        content = "Content not found.";
    }
    server.send(200, "text/html", content);
}

void handleSDFileMan()
{
    server.send(200, "text/html", "<h1>Server</h1>");
}

void setupWifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    WiFi.begin(ssid, pass);
    Serial.println("connecting");
    int breakout = 0;
    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.write('.');
        delay(500);
        breakout += 1;
        if(breakout == 10)
        {
            return;
        }
    }
    Serial.println("");
    WiFi.hostname(board_name);
    Serial.println("connected");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
}

void setupWebServer()
{
    Serial.println("Setting up Server.");
    
    server.on("/", handleServerRoot);
    server.on("/SDFileMan", handleSDFileMan);
    server.on("/upload", upload);
    server.onFileUpload(handleFileUpload);

    server.begin();
    Serial.println("HTTP Server started.");
}

void setupOTA()
{
    ota_server.setup();
}

void setupSD()
{
    // check for a card.
    if(!SD.begin(chipSelect))
    {
        Serial.println("sd card failed to begin library.");
        hasSD = false;
    }
    else
    {
        Serial.println("sd begin succeeded.");
        hasSD = true;
    }
}

void printArguments(int argc, String* argv)
{
    Serial.println();
    Serial.print(String("argc: ") + argc + " arguments: ");
    for(int i = 0; i < argc; i++)
    {
        Serial.print(argv[i]);
        Serial.print(' ');
    }
    Serial.println();
}

void setup()
{
    Serial.begin(115200);
    Serial.println("numCommands: " + String(getNumCommands()));
    // Serial.setDebugOutput(true);
    Serial.println("setting up wifi.");
    setupWifi();
    Serial.println("setting up SD.");
    setupSD();
    Serial.println("setup ota.");
    setupOTA();
    Serial.println("setup http server.");
    setupWebServer();
}

void loop()
{
    if(WiFi.status() != WL_CONNECTED)
    {
        setup();
    }
    
    // get new input and interpret it.
    interpretInput(getInput());

    ota_server.handle();
    server.handleClient();
    yield();
}
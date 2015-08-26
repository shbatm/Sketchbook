#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <Time.h>
//#include <DHT.h>
#include "constants.h"
#include "data_rnplogo.h"

char s_ssid[33] = {0};
char s_password[65] = {0};
char s_ntpserver[100] = {0};
uint8_t s_state = 0;
uint8_t s_bootstate = 0;
uint8_t s_timezone = 12;
uint8_t s_summertime = 1;
uint8_t s_version = 1;

uint8_t lhour = 0;
uint8_t lminute = 0;
uint8_t lsecond = 0;
uint8_t lday = 0;
uint8_t lmonth = 0;
uint16_t lyear = 0;

bool apmode = false;

ESP8266WebServer server ( 80 );
//DHT dht(dht_pin, dht_type, 20);

unsigned int NTPlocalPort = 2390;
IPAddress timeServer;
const int NTP_PACKET_SIZE = 48;
byte NTPpacketBuffer[ NTP_PACKET_SIZE];
WiFiUDP NTPudp;

//WiFiUDP otaListener;

float humidity, temperature;

bool output1_state = false;
bool output2_state = false;

/* LED (disabled) */

/*void startled() {
  digitalWrite(led_clk, LOW);
  delayMicroseconds(600);
}

void sendled(uint8 r, uint8 g, uint8 b) {
    shiftOut(led_dat, led_clk, MSBFIRST, r);
    shiftOut(led_dat, led_clk, MSBFIRST, g);
    shiftOut(led_dat, led_clk, MSBFIRST, b);
}*/

/* Settings */

void settings_load() {
  s_state = EEPROM.read(0);
  for (int i = 0; i<sizeof(s_ssid); i++) {
    s_ssid[i] = EEPROM.read(sizeof(s_state)+i);
  }
  for (int i = 0; i<sizeof(s_password); i++) {
    s_password[i] = EEPROM.read(sizeof(s_state)+sizeof(s_ssid)+i);
  }
  s_bootstate = EEPROM.read(sizeof(s_state)+sizeof(s_ssid)+sizeof(s_password));
  s_timezone = EEPROM.read(sizeof(s_state)+sizeof(s_ssid)+sizeof(s_password)+sizeof(s_bootstate));
  s_summertime = EEPROM.read(sizeof(s_state)+sizeof(s_ssid)+sizeof(s_password)+sizeof(s_bootstate)+sizeof(s_timezone));
  for (int i = 0; i<sizeof(s_ntpserver); i++) {
    s_ntpserver[i] = EEPROM.read(sizeof(s_state)+sizeof(s_ssid)+sizeof(s_password)+sizeof(s_bootstate)+sizeof(s_timezone)+sizeof(s_summertime)+i);
  }
}

void settings_store() {
  EEPROM.write(0,s_state);
  for (int i = 0; i<sizeof(s_ssid); i++) {
    EEPROM.write(sizeof(s_state)+i, s_ssid[i]);
  }
  for (int i = 0; i<sizeof(s_password); i++) {
    EEPROM.write(sizeof(s_state)+sizeof(s_ssid)+i, s_password[i]);
  }
  EEPROM.write(sizeof(s_state)+sizeof(s_ssid)+sizeof(s_password), s_bootstate);
  EEPROM.write(sizeof(s_state)+sizeof(s_ssid)+sizeof(s_password)+sizeof(s_bootstate), s_timezone);
  EEPROM.write(sizeof(s_state)+sizeof(s_ssid)+sizeof(s_password)+sizeof(s_bootstate)+sizeof(s_timezone), s_summertime);
  for (int i = 0; i<sizeof(s_ntpserver); i++) {
    EEPROM.write(sizeof(s_state)+sizeof(s_ssid)+sizeof(s_password)+sizeof(s_bootstate)+sizeof(s_timezone)+sizeof(s_summertime)+i, s_ntpserver[i]);
  }
  EEPROM.commit();
}

void factoryReset() {
  //Serial.println("---FACTORY RESET---");
  String("").toCharArray(s_ssid, sizeof(s_ssid));
  String("").toCharArray(s_password, sizeof(s_password));
  String("time.nist.gov").toCharArray(s_ntpserver, sizeof(s_ntpserver));
  s_state = 0;
  s_bootstate = 0;
  s_timezone = 12;
  s_summertime = 1;
  s_version = 1;
  settings_store();
  delay(1000);
  ESP.reset();
}

/* Webserver */

void startWebserver() {
  server.on ("/", handleRoot);
  server.on ("/command", handleCommand);
  server.on ("/setup", handleSetup);
  server.on ("/setupstore", handleSetupStore);
  //server.on ("/logo.png", []() { server.send_P( 200, MIME_png, DATA_logo ); } );
  server.on ("/style.css", []() { server.send_P( 200, MIME_css, DATA_css ); } );
 // server.on ("/switchery.min.js", []() { server.send_P( 200, MIME_javascript, DATA_switchery ); } );
  server.on ("/upgrade", []() { server.send_P ( 200, MIME_html, DATA_upgrade ); } );

  server.onNotFound ( handleNotFound );
  server.onFileUpload (handleFileUpload);
  server.begin();
  Serial.println("<INFO> HTTP server started.");
}

void handleFileUpload(){
  //Serial.println("To-do: file-upload for firmware update.");
      if(server.uri() != "/upgrade_do") return;
      HTTPUpload& upload = server.upload();
      if(upload.status == UPLOAD_FILE_START){
        Serial.setDebugOutput(true);
        WiFiUDP::stopAll();
        Serial.printf("Update: %s\n", upload.filename.c_str());
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        if(!Update.begin(maxSketchSpace)){//start with max available size
          Update.printError(Serial);
        }
      } else if(upload.status == UPLOAD_FILE_WRITE){
        if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
          Update.printError(Serial);
        }
      } else if(upload.status == UPLOAD_FILE_END){
        if(Update.end(true)){ //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      }
      yield();
}

void handleStaticHeader(WiFiClient client, uint32_t s, String type) {
  String headers = "HTTP/1.1 200 OK\r\nContent-Type: "+type+"\r\nContent-Length: "+String(s)+"\r\nConnection: close\r\nAccess-Control-Allow-Origin: *\r\n\r\n";
  client.write(headers.c_str(), headers.length());
}
/*
void handleLogo() {
  Serial.print("Sending logo (old method)...");
  //server.send_P (200, "image/png", DATA_css);
  //Serial.println(" DONE");
  ESP.wdtDisable();
  WiFiClient client = server.client();
  handleStaticHeader(client, sizeof(DATA_logo), "image/png");
  client.write(DATA_logo, sizeof(DATA_logo));
  client.stop();
  //ESP.wdtEnable(1000);
  Serial.println(" DONE");
}*/

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void handleSetupStore() {
  String message = "Setup:\n";
  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
    if (server.argName(i) == "ssid") {
      message += " SSID set to " + server.arg(i) + "\n";
      String valuestr = String(server.arg(i));
      valuestr.toCharArray(s_ssid, sizeof(s_ssid));
    }
    if (server.argName(i) == "password") {
      message += " Password set to " + server.arg(i) + "\n";
      String valuestr = String(server.arg(i));
      valuestr.toCharArray(s_password, sizeof(s_password));
    }
    if (server.argName(i) == "ntpserver") {
      message += " NTP server address set to " + server.arg(i) + "\n";
      String valuestr = String(server.arg(i));
      valuestr.toCharArray(s_ntpserver, sizeof(s_ntpserver));
    }
    if (server.argName(i) == "bootstate") {
      String valuestr = String(server.arg(i));
      message += " Bootstate set to " + server.arg(i) + "\n";
      int bootstateint = valuestr.toInt();
      if ((bootstateint >= 0) && (bootstateint < 4)) {
        s_bootstate = bootstateint;
      } else {
        Serial.println("<ERROR> bootstate out of bounds.");
        message += "ERROR: bootstate out of bounds\n";
      };
    }
    if (server.argName(i)=="timezone") {
      String valuestr = String(server.arg(i));
      message += " Timezone set to "+server.arg(i)+"\n";
      int timezoneint = valuestr.toInt();
      if ((timezoneint>=0) && (timezoneint<256)) {
        s_timezone = timezoneint;
      } else {
        Serial.println("<ERROR> timezone out of bounds.");
        message += "ERROR: Timezone out of bounds\n";
      };
    }
    if (server.argName(i)=="summertime") {
      String valuestr = String(server.arg(i));
      message += " Summertime set to "+server.arg(i)+"\n";
      int summertimeint = valuestr.toInt();
      if ((summertimeint>=0) && (summertimeint<256)) {
        s_summertime = summertimeint;
      } else {
        Serial.println("<ERROR> summertime out of bounds.");
        message += "ERROR: Summertime out of bounds\n";
      };
    }
    if (server.argName(i) == "store") {
      s_state = 1;
      message += " Save to flash!\n";
      settings_store();
    }
  }

  server.send ( 200, "text/plain", message );
  delay(1000);
  ESP.reset();
}

int cval = 0;
bool binval = false;
uint8_t bval[4] = {0};
int brightness = 1024;
bool autobrightness = true;

uint8_t selectedled = 0;
uint8_t led_r[4] = {0};
uint8_t led_g[4] = {0};
uint8_t led_b[4] = {0};

void handleCommand() {
  digitalWrite(status_pin, HIGH);
  String message = "Command:\n";
  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
    if (server.argName(i) == "output") {
      message += " Output set to " + server.arg(i) + "\n";
      String valuestr = String(server.arg(i));
      uint8_t input = valuestr.toInt();
      if (input & 0x01) {
        digitalWrite(output1_pin, HIGH);
        output1_state = true;
      } else {
        digitalWrite(output1_pin, LOW);
        output1_state = false;
      }
      if (input & 0x02) {
        digitalWrite(output2_pin, HIGH);
        output2_state = true;
      } else {
        digitalWrite(output2_pin, LOW);
        output2_state = false;
      }
      Serial.print("<DEBUG> Set output to value '");
      Serial.print(cval);
      Serial.println("'.");
    }
    if (server.argName(i) == "output1") {
      message = "x";
      String valuestr = String(server.arg(i));
      uint8_t input = valuestr.toInt();
      if (input) {
        digitalWrite(output1_pin, HIGH);
        output1_state = true;
        message = "1";
      } else {
        digitalWrite(output1_pin, LOW);
        output1_state = false;
        message = "0";
      }
      Serial.print("<DEBUG> Set output 1 to value '");
      Serial.print(cval);
      Serial.println("'.");
    }
    if (server.argName(i) == "output2") {
      message = "x";
      String valuestr = String(server.arg(i));
      uint8_t input = valuestr.toInt();
      if (input) {
        digitalWrite(output2_pin, HIGH);
        output2_state = true;
        message = "1";
      } else {
        digitalWrite(output2_pin, LOW);
        output2_state = false;
        message = "0";
      }
      Serial.print("<DEBUG> Set output 2 to value '");
      Serial.print(cval);
      Serial.println("'.");
    }

    if (server.argName(i) == "ledsel") {
      String valuestr = String(server.arg(i));
      uint8_t input = valuestr.toInt();
      if ((input>=0) && (input<4)) {
        selectedled = input;
        message = String("OK");
      } else {
        message = String("FAILED");
      }
    }

    if (server.argName(i) == "ledr") {
      String valuestr = String(server.arg(i));
      uint8_t input = valuestr.toInt();
      led_r[selectedled] = input;
      message = String("OK");
    }
    if (server.argName(i) == "ledg") {
      String valuestr = String(server.arg(i));
      uint8_t input = valuestr.toInt();
      led_g[selectedled] = input;
      message = String("OK");
    }
    if (server.argName(i) == "ledb") {
      String valuestr = String(server.arg(i));
      uint8_t input = valuestr.toInt();
      led_b[selectedled] = input;
      message = String("OK");
    }

    if (server.argName(i) == "ledwrite") {
      /*startled();
      sendled(led_r[0],led_g[0],led_b[0]);
      sendled(led_r[1],led_g[1],led_b[1]);
      sendled(led_r[2],led_g[2],led_b[2]);
      sendled(led_r[3],led_g[3],led_b[3]);*/
      message = String("NOT SUPPORTED");
    }

    if (server.argName(i) == "temperature") {
      char temp[40];
      readDHT();
      dtostrf(temperature, 4, 2, temp);
      message = String(temp);
    }
    if (server.argName(i) == "humidity") {
      char temp[40];
      readDHT();
      dtostrf(humidity, 4, 2, temp);
      message = String(temp);
    }
    if (server.argName(i) == "state1") {
      if (output1_state) {
        message = "1";
      } else {
        message = "0";
      }
    }
    if (server.argName(i) == "state2") {
      if (output2_state) {
        message = "1";
      } else {
        message = "0";
      }
    }
    if (server.argName(i) == "factory") {
      factoryReset();
    }
  }

  server.send ( 200, "text/plain", message );
  digitalWrite(status_pin, LOW);
}

void handleRoot() {
  Serial.println("ROOT START");
  digitalWrite(status_pin, HIGH);
  for ( uint8_t i = 0; i < server.args(); i++ ) {
    if (server.argName(i) == "output") {
      String valuestr = String(server.arg(i));
      uint8_t input = valuestr.toInt();
      if (input & 0x01) {
        digitalWrite(output1_pin, HIGH);
        output1_state = true;
      } else {
        digitalWrite(output1_pin, LOW);
        output1_state = false;
      }
      if (input & 0x02) {
        digitalWrite(output2_pin, HIGH);
        output2_state = true;
      } else {
        digitalWrite(output2_pin, LOW);
        output2_state = false;
      }
      Serial.print("<DEBUG> Set output to value '");
      Serial.print(cval);
      Serial.println("'.");
    }
    if (server.argName(i) == "output1") {
      String valuestr = String(server.arg(i));
      uint8_t input = valuestr.toInt();
      if (input) {
        digitalWrite(output1_pin, HIGH);
        output1_state = true;
      } else {
        digitalWrite(output1_pin, LOW);
        output1_state = false;
      }
      Serial.print("<DEBUG> Set output 1 to value '");
      Serial.print(cval);
      Serial.println("'.");
    }
    if (server.argName(i) == "output2") {
      String valuestr = String(server.arg(i));
      uint8_t input = valuestr.toInt();
      if (input) {
        digitalWrite(output2_pin, HIGH);
        output2_state = true;
      } else {
        digitalWrite(output2_pin, LOW);
        output2_state = false;
      }
      Serial.print("<DEBUG> Set output 2 to value '");
      Serial.print(cval);
      Serial.println("'.");
    }
  }

  char temp[2501];

  readDHT();
  char temperature_string[10];
  dtostrf(temperature, 4, 2, temperature_string);
  char humidity_string[10];
  dtostrf(humidity, 4, 2, humidity_string);

  String output1_text = "<a href='/?output1=1'>(OFF) Switch on</a>";
  if (output1_state) {
    output1_text = "<a href='/?output1=0'>(ON) Switch off</a>";
  }

  String output2_text = "<a href='/?output2=1'>(OFF) Switch on</a>";
  if (output2_state) {
    output2_text = "<a href='/?output2=0'>(ON) Switch off</a>";
  }

  snprintf ( temp, 2500,

             "<!DOCTYPE html><html><head><meta charset='UTF-8' /><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>RN+ WiFi switch</title>\
             <link href='http://fonts.googleapis.com/css?family=Open+Sans:400,700' rel='stylesheet' type='text/css'><link href='style.css' rel='stylesheet' type='text/css'>\
             <script type='text/javascript'>window.onload = function() { gettemperature(); gethumidity(); setInterval(function () {getsomething()}, 5000); }</script>\
             </head><body><div class='header'><!--<img class='float_left' src='logo.png' />--><div class='float_left'><h1><span class='logo1'>R</span>N+</h1><h2>WiFi switch</h2></div>\
             <div class='float_right'><a href='/'>HOME</a><!--<a href='/schedule'>SCHEDULE</a>--><a href='/setup'>SETTINGS</a><a href='http://www.rnplus.nl/wifiswitch/help'>DOCUMENTATION</a></div></div>\
             <div class='mobile_menu'><a href='/'>HOME</a><!--<a href='/schedule'>SCHEDULE</a>--><a href='/setup'>SETTINGS</a><a href='http://www.rnplus.nl/wifiswitch/help'>DOCUMENTATION</a></div>\
             <div class='home' style='margin-top: 50px'>\
             <div class='row'><div class='left'>Local time</div><div class='right'>%02d:%02d:%02d</div></div>\
             <div class='row'><div class='left'>Temperature</div><div class='right'>%s&nbsp;&#176;c</div></div>\
             <div class='row'><div class='left'>Humidity</div><div class='right'>%s %</div></div>\
             <div class='row'><div class='left'>Output 1</div><div class='right'>%s</div></div>\
             <div class='row'><div class='left'>Output 2</div><div class='right'>%s</div></div>\
             </div></body></html>",

             lhour, lminute, lsecond, temperature_string, humidity_string, output1_text.c_str(), output2_text.c_str()
           );
  server.send ( 200, "text/html", temp );
  digitalWrite(status_pin, LOW);
  Serial.println("ROOT END");
}

void handleSetup() {
  //ESP.wdtDisable();
  digitalWrite(status_pin, HIGH);

  char stY[9];
  char stN[9];

  if (s_summertime) {
    String(" checked").toCharArray(stY, sizeof(stY));
    String("").toCharArray(stN, sizeof(stN));
  } else {
    String("").toCharArray(stY, sizeof(stY));
    String(" checked").toCharArray(stN, sizeof(stN));
  }

  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");

  String temp = "<!DOCTYPE html><html><head><meta charset='UTF-8' /><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>RN+ WiFi switch - Setup</title>\
     <link href='http://fonts.googleapis.com/css?family=Open+Sans:400,700' rel='stylesheet' type='text/css'><link href='style.css' rel='stylesheet' type='text/css'>\
     </head><body><div class='header'><!--<img class='float_left' src='logo.png' />--><div class='float_left'>&nbsp;</span><h1><span class='logo1'>R</span>N+</h1><h2>WiFi switch</h2></div>\
     <div class='float_right'><a href='/'>HOME</a><!--<a href='/schedule'>SCHEDULE</a>--><a href='/setup'>SETTINGS</a><a href='http://www.rnplus.nl/wifiswitch/help'>DOCUMENTATION</a></div></div>\
     <div class='mobile_menu'><a href='/'>HOME</a><!--<a href='/schedule'>SCHEDULE</a>--><a href='/setup'>SETTINGS</a><a href='http://www.rnplus.nl/wifiswitch/help'>DOCUMENTATION</a></div>\
     <form action='/setupstore' method='post'><div class='home wide' style='margin-top: 50px;'>\
     <div class='row'><div class='left'>SSID</div><div class='right'><input type='text' name='ssid' value='"+String(s_ssid)+"'></div></div>\
     <div class='row'><div class='left'>Password</div><div class='right'><input type='text' name='password' value='"+String(s_password)+"'></div></div>\
     <div class='row'><div class='left'>NTP server</div><div class='right'><input type='text' name='ntpserver' value='"+s_ntpserver+"'></div></div>\
     <div class='row'><div class='left'>Output state at boot (0,1,2 or 3)</div><div class='right'><input type='text' name='bootstate' value='"+s_bootstate+"'></div></div>\
     <div class='row'><div class='left'>Timezone (12 + offset to UTC)</div><div class='right'><input type='text' name='timezone' value='"+s_timezone+"'></div></div>\
     <div class='row'><div class='left'>Automatic summertime</div><div class='right'><input type='radio' name='summertime' value='1'"+String(stY)+">Yes&nbsp;<input type='radio' name='summertime' value='0'"+String(stN)+">No</div></div>\
     <div class='row'><div class='left'>&nbsp;</div><div class='right'><input type='submit' value='Save configuration'><input type='hidden' name='store' value='yes'></div></div>\
     </form></body></html>";
  WiFiClient client = server.client();
  String headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "+String(temp.length())+"\r\nConnection: close\r\nAccess-Control-Allow-Origin: *\r\n\r\n";
  client.write(headers.c_str(), headers.length());
  client.write(temp.c_str(), temp.length());
  //server.send ( 200, "text/html", temp );
  digitalWrite(status_pin, LOW);
  //ESP.wdtEnable(1000);
}

void wifiConnect() {
    Serial.print("\n<INFO> Connecting to the WiFi network");
    WiFi.mode(WIFI_STA);
    WiFi.begin(s_ssid, s_password);
    bool led = 0;
    int counter = 0;
    uint8_t a = 0;
    uint8_t b = 255;
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      digitalWrite(status_pin, led);
      led = !led;
      /*startled();
      sendled(a,0,0);
      sendled(b,0,0);
      sendled(a,0,0);
      sendled(b,0,0);*/
      if (a==255) {
        b = 255;
        a = 0;
      } else {
        b = 0;
        a = 255;
      }
      delay(500);
      counter++;
    }
      /*startled();
      sendled(0,255,0);
      sendled(0,255,0);
      sendled(0,255,0);
      sendled(0,255,0);*/
    Serial.println(" connected");
    Serial.print("<INFO> IP address: ");
    Serial.println(WiFi.localIP());
    delay(1000);
}

void setup() {
  pinMode(output1_pin, OUTPUT);
  pinMode(output2_pin, OUTPUT);
  pinMode(status_pin, OUTPUT);
  pinMode(button1_pin, INPUT);
  pinMode(button2_pin, INPUT);

  pinMode(led_dat, OUTPUT);
  pinMode(led_clk, OUTPUT);

  /*startled();
  sendled(255,0,0);
  sendled(0,0,0);
  sendled(0,0,0);
  sendled(0,0,0);*/

  //ESP.wdtEnable(1000);

  digitalWrite(status_pin, LOW);
  Serial.begin(115200);
  EEPROM.begin(sizeof(s_state) + sizeof(s_ssid) + sizeof(s_password) + sizeof(s_bootstate) + sizeof(s_timezone) + sizeof(s_summertime) + sizeof(s_ntpserver));

  Serial.println("\n\nDouble relay switch\nRN+ 2015\n\nFirmware version: 3.0\n");
  Serial.println(String("Flash size: ")+String(ESP.getFlashChipSize())+String(" bytes."));
  //uint32_t chipID = ESP.getChipId();
  //uint32_t flashID = ESP.getFlashChipId();
  //Serial.println(String("Chip ID: ")+String(chipID));
  //Serial.println(String("Flash ID: ")+String(flashID));
  //delay(2000);

  //addFrame(drawDebugFrame);

  //Read state, ssid and password from flash
  settings_load();
  
  if ((!digitalRead(button1_pin)) && (!digitalRead(button2_pin))) {
    Serial.println("<INFO> Both buttons pressed: factory reset!");
    factoryReset();
  }

  if (s_state == 0) {
    Serial.println("<INFO> The device has not been configured yet.");
    digitalWrite(status_pin, HIGH);
    apmode = true;
  } else if (s_state > 1) {
    Serial.println("<ERROR> Unknown state. Resetting to factory default settings...");
    factoryReset();
  } else {
    Serial.print("<INFO> Device has been configured to connect to network \"");
    Serial.print(s_ssid);
    Serial.println("\".");
    Serial.print("<DEBUG> WiFi password is \"");
    Serial.print(s_password);
    Serial.println("\".");
    uint8_t cnt = 0;
    Serial.flush();
    Serial.print("Press 's' to enter setup mode");
    while (cnt < 100) {
      if (Serial.available()) {
        char input = Serial.read();
        Serial.println(input);
        if (!digitalRead(button1_pin) || input == 's' || input == 'S') {
          apmode = true;
          break;
        }
      }
      delay(10);
      Serial.print(".");
      cnt++;
    }
  }

    if (apmode) {
      Serial.println("<INFO> Starting in AP mode...");
      delay(1000);
      WiFi.disconnect();
      Serial.println("<INFO> (Disconnected)");
      delay(1000);
      WiFi.mode(WIFI_AP_STA);
      Serial.println("<INFO> (Mode set)");
      delay(1000);
      WiFi.softAP("setup");
      Serial.println("<INFO> (Soft AP)");
      delay(1000);
      IPAddress myIP = WiFi.softAPIP();
      Serial.print("AP IP address: ");
      Serial.println(myIP);
      Serial.println("<INFO> OK.");
    } else {
      Serial.println("<INFO> Starting in CLIENT mode...");
      wifiConnect();
    }

  if (s_bootstate & 0x01) {
    digitalWrite(output1_pin, HIGH);
    output1_state = true;
  } else {
    digitalWrite(output1_pin, LOW);
    output1_state = false;
  }
  if (s_bootstate & 0x02) {
    digitalWrite(output2_pin, HIGH);
    output2_state = true;
  } else {
    digitalWrite(output2_pin, LOW);
    output2_state = false;
  }

  Serial.println ( "<INFO> Boot state set." );

  startWebserver();

  if (!apmode) {
    NTPudp.begin(NTPlocalPort);
    Serial.println("Resolving IP address for NTP server ´"+String(s_ntpserver)+"'...");
    if (WiFi.hostByName(s_ntpserver, timeServer)) {
      Serial.print("<INFO> NTP server IP address: ");
      Serial.println(timeServer);
    } else {
      timeServer = IPAddress(0,0,0,0);
      Serial.print("<ERROR> Could not find the NTP server.");
    }
    sendNTPpacket(timeServer);
  } else {
    Serial.println("<INFO> NTP disabled in AP mode.");
  }


  //OTA
  //otaListener.begin(8266);
  //Serial.print("Sketch size: ");
  //Serial.println(ESP.getSketchSize());
  //Serial.print("Free size: ");
  //Serial.println(ESP.getFreeSketchSpace());
}

bool button1_ispressed = false;
bool button2_ispressed = false;
void checkbuttons() {
  if (!digitalRead(button1_pin)) {
    if (!button1_ispressed) {
      button1_ispressed = true;
      output1_state = !output1_state;
      digitalWrite(output1_pin, output1_state);
    }
  } else {
    button1_ispressed = false;
  }
  if (!digitalRead(button2_pin)) {
    if (!button2_ispressed) {
      button2_ispressed = true;
      output2_state = !output2_state;
      digitalWrite(output2_pin, output2_state);
    }
  } else {
    button2_ispressed = false;
  }
}

void ota() {
  /*int cb = otaListener.parsePacket();
  if (cb) {
    IPAddress remote = otaListener.remoteIP();
    int cmd  = otaListener.parseInt();
    int port = otaListener.parseInt();
    int sz   = otaListener.parseInt();
    Serial.println("Got packet");
    Serial.printf("%d %d %d\r\n", cmd, port, sz);
    WiFiClient cl;
    if (!cl.connect(remote, port)) {
      Serial.println("failed to connect");
      return;
    }

    otaListener.stop();

    if (!ESP.updateSketch(cl, sz)) {
      Serial.println("Update failed");
    }
  }*/
}

bool readDHT() {
  bool dhtvalid = false;
  uint8_t retry = 5;
  float ihumidity, itemperature;

  while (retry) {
//    ihumidity = dht.readHumidity();
//    itemperature = dht.readTemperature(false);
    if (isnan(ihumidity) || isnan(itemperature)) {
      Serial.println("<ERROR> Failed to read from the DHT sensor.");
      retry -= 1;
      dhtvalid = false;
    } else {
      dhtvalid = true;
      temperature = itemperature;
      humidity = ihumidity;
      /*Serial.print("<INFO> Temperature: ");
      Serial.print(temperature);
      Serial.print(" degrees c, humidity: ");
      Serial.print(humidity);
      Serial.println("%");*/
      retry = 0;
    }
  }
  return dhtvalid;
}

unsigned long sendNTPpacket(IPAddress& address)
{
  if (!apmode) {
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(NTPpacketBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  NTPpacketBuffer[0] = 0b11100011;   // LI, Version, Mode
  NTPpacketBuffer[1] = 0;     // Stratum, or type of clock
  NTPpacketBuffer[2] = 6;     // Polling Interval
  NTPpacketBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  NTPpacketBuffer[12]  = 49;
  NTPpacketBuffer[13]  = 0x4E;
  NTPpacketBuffer[14]  = 49;
  NTPpacketBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  NTPudp.beginPacket(address, 123); //NTP requests are to port 123
  NTPudp.write(NTPpacketBuffer, NTP_PACKET_SIZE);
  NTPudp.endPacket();
  }
}

unsigned long ledupdate = 0;
unsigned long nextSecond = 0;

void updateNTP() {
  int cb = NTPudp.parsePacket();
  if (cb) {
    Serial.print("packet received, length=");
    Serial.println(cb);
    // We've received a packet, read the data from it
    NTPudp.read(NTPpacketBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(NTPpacketBuffer[40], NTPpacketBuffer[41]);
    unsigned long lowWord = word(NTPpacketBuffer[42], NTPpacketBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    Serial.println(epoch);

    time_t t = epoch;

    Serial.print("UTC date and time: ");
    Serial.print(day(t));
    Serial.print("-");
    Serial.print(month(t));
    Serial.print("-");
    Serial.print(year(t));
    Serial.print(" ");
    Serial.print(hour(t));
    Serial.print(":");
    Serial.print(minute(t));
    Serial.print(":");
    Serial.println(second(t));

   if (s_summertime) {
     Serial.print("Summertime correction: ");
     int beginDSTDate=  (31 - (5* year(t) /4 + 4) % 7);
     int beginDSTMonth=3;
     int endDSTDate= (31 - (5 * year(t) /4 + 1) % 7);
     int endDSTMonth=10;
     // DST is valid as:
     if (((month(t) > beginDSTMonth) && (month(t) < endDSTMonth))
     || ((month(t) == beginDSTMonth) && (day(t) >= beginDSTDate))
     || ((month(t) == endDSTMonth) && (day(t) <= endDSTDate))) {
       Serial.println("YES");
       t+= 3600;
     } else{
       Serial.println("NO");
     }
   }

   //Timezone adjust
   t+= 3600*(s_timezone-12);

    Serial.print("Local date and time: ");
    Serial.print(day(t));
    Serial.print("-");
    Serial.print(month(t));
    Serial.print("-");
    Serial.print(year(t));
    Serial.print(" ");
    Serial.print(hour(t));
    Serial.print(":");
    Serial.print(minute(t));
    Serial.print(":");
    Serial.println(second(t));
    lsecond = second(t);
    lminute = minute(t);
    lhour = hour(t);
    lday = day(t);
    lmonth = month(t);
    lyear = year(t);
  }
}

void loop() {
  if (!apmode) {
    if (WiFi.status() != WL_CONNECTED) {
      //Serial.println("<ERROR> Lost connection to WiFi network.");
      digitalWrite(status_pin, LOW);
      //delay(1000);
      wifiConnect();
    }
  }
  server.handleClient();
  ota();
  checkbuttons();
  //readDHT();

  if (millis()>nextSecond) {
    nextSecond = millis()+1000;
    lsecond++;

    if (lsecond>59) {
      lsecond = 0;
      lminute++;
      sendNTPpacket(timeServer);
    }
    if (lminute>59) {
      readDHT();
      lminute = 0;
      lhour++;
    }
    updateNTP();
  }
}

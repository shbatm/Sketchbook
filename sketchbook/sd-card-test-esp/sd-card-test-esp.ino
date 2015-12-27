#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

#include <ArduinoOTA.h>

#include <SPI.h>
#include <SD.h>

const char* board_name = "3Dprinter";

const char* ssid = "www.tkkrlab.nl";
const char* pass = "hax4or2the2paxor3";

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

void printDirectory(File dir, int numTabs)
{
    while(true)
    {
        File entry = dir.openNextFile();
        if(!entry)
        {
            break;
        }
        for(uint8_t i = 0; i < numTabs;i++)
        {
            Serial.print('\t');
        }
        Serial.print(entry.name());
        if(entry.isDirectory())
        {
            Serial.println("/");
            printDirectory(entry, numTabs+1);
        }
        else
        {
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
}

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
    Serial.print(String("argc: ") + argc + " arguments: ");
    for(int i = 0; i < argc; i++)
    {
        Serial.print(argv[i]);
        Serial.print(' ');
    }
    Serial.println();
}

void listSd(int argc, String* argv)
{
    printArguments(argc, argv);
    File file;
    if(argc == 0)
    {
        // print root
        file = SD.open("/");
        printDirectory(file, 0);
    }
    else
    {
        for(int i = 0; i < argc; i++)
        {
            if(argv[i].endsWith("/"))
            {
                file = SD.open(argv[i]);
            }
            else
            {
                file = SD.open(argv[i] + "/");
            }
            Serial.println(argv[i] + ": ");
            // printDirectory closes the file.
            printDirectory(file, 0);
        }
    }
}

void cat(int argc, String* argv)
{
    File file;
    for(int i = 0; i < argc; i++)
    {
        file = SD.open(argv[i]);
        if(file.isDirectory())
        {
            Serial.println(String("cat: ") + argv[i] + ": " + "Is a Directory.");
        }
        else
        {
            while(file.available())
            {
                Serial.write((char)file.read());
            }
        }
        // close file after writing.
        file.close();
    }
}

typedef struct {
    String name;
    void (*handle)(int, String* argv);
} command_t;

command_t commands[] = 
{
    {
        .name = String("list"),
        .handle = listSd,
    },
    {
        .name = String("cat"),
        .handle = cat,
    }
};

uint32_t numCommands = sizeof(commands) / sizeof(command_t);

// returns input that is newline terminated.
// returns string with 0 len if nothing found yet.
// or not done yet receiving.
String received;
String getInput()
{
    // check if anything new available.
    if(Serial.available())
    {
        // read in that data.
        char data = Serial.read();
        if(data == '\n')
        {
            // if end of line (marked with newline) is found.
            // return a copy and clear our own buffer.
            String nreceived = received;
            received = "";
            return nreceived;
        }
        else
        {
            // else just keep appending that new data!
            received += data;
        }
    }
    return String("");
}

// this function interprets the data.
// and executes it.
void interpretInput(String input)
{
    static const char delim[] = " ";
    if(input.length())
    {
        // print command to prompt.
        Serial.println(">> " + input);
        
        // create a copy to be used with strtok.
        char* str_input = (char *)malloc(sizeof(char) * input.length());
        if(str_input == NULL)
        {
            Serial.println("Input Copy Error.");
            return;
        }
        strcpy(str_input, input.c_str());
        // check for leading whitespace.
        if(isspace(str_input[0]))
        {
            Serial.println("Syntax error");
            free(str_input);
            return;
        }
        // trim trailing whitespace.
        int len = strlen(str_input);
        while(isspace(str_input[len - 1]))
        {
            str_input[len - 1] = '\0';
            len = strlen(str_input);
        }

        // strtok makes inter command/argument whitespaces appear and thus
        // no need to check for that.
        
        // first token is the actuall command.
        String command = String(strtok(str_input, delim));
        // Serial.println("command: " + command + " len: " + command.length());
        
        // max allowed length of argv, thus max allowed arguments.
        static int argc;
        static const uint8_t max_args = 20;
        String argv[max_args];

        // parse out all arguments for aslong as there are arguments.
        char* argument = strtok(NULL, delim);
        argc = 0;
        while(argument != NULL)
        {
            // put the arguments in order in argv
            argv[argc] = String(argument);
            // Serial.println("Argument: " + argv[argc] + " Len: " + argv[argc].length());
            argument = strtok(NULL, delim);
            argc++;
            // keep track of where to put argument in argv.
            // and check if we are not exeeding max arguments.
            // also is length of argv thus argc.
            if(argc == max_args)
            {
                free(str_input);
                return;
            }
        }
        // Serial.println("argc: " + String(argc));

        // check for command in commands list. and call it's handler.
        for(int i = 0; i < numCommands;i++)
        {
            if(command == commands[i].name)
            {
                // call that command handle
                (commands[i].handle)(argc, argv);
                free(str_input);
                return;
            }
        }

        // if we reach here it's unkown input/command.
        Serial.println("unknown input: '" + input + "'");
        free(str_input);
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("numCommands: " + String(numCommands));
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
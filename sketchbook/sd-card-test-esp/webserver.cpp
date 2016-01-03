#include "webserver.h"

ESP8266WebServer server;

static File uploadFile;
static String html_root = "/html/";
static String gcode_root = "/gcode/";

void setupWebServer()
{
    Serial.println("Setting up Server.");
    
    server.on("/", handleServerRoot);
    server.on("/SDFileMan", handleSDFileMan);
    server.on("/uploadhtml", uploadHtml);
    server.on("/uploadgcode", uploadGcode);
    server.on("/action", action);
    server.onFileUpload(handleFileUpload);

    server.begin();
    Serial.println("HTTP Server started.");
}

String loadHtml(String path)
{
    String content;
    File html;
    Serial.printf("loading page from: %s\n", path.c_str());
    html = SD.open(path, FILE_READ);
    if(html)
    {
        int data = '\0';
        while(html.available())
        {
            content += (char)html.read();
        }
        html.close();
    }
    else
    {
        content = "<h6>" + path + " Content not found.</h6>";
    }
    return content;
}

void uploadHtml()
{
    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/html", loadHtml("/html/uphtml.htm"));
}

void uploadGcode()
{
    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/html", loadHtml("/html/upgcode.htm"));
}

void handleFileUpload()
{
    bool is_html_upload = (server.uri() == "/uploadhtml");
    bool is_gcode_upload = (server.uri() == "/uploadgcode");
    static unsigned long int start;

    // Serial.printf("server.uri() = %s\n", server.uri().c_str());
    
    if(is_html_upload || is_gcode_upload)
    {
        // figure out where to put the file.
        String root = "";
        if(is_html_upload)
        {
            root = html_root;
        }
        if(is_gcode_upload)
        {
            root = gcode_root;
        }
        
        // then handle the file uploading.
        HTTPUpload& upload = server.upload();
        
        if(upload.status == UPLOAD_FILE_START)
        {
            start = millis();
            String filepath = root + upload.filename;
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
        }
        else if(upload.status == UPLOAD_FILE_END)
        {
            if(uploadFile)
            {
                uploadFile.close();
            }
            Serial.print("Upload: END, Size: ");
            Serial.println(upload.totalSize);
            Serial.printf("toke: %ld seconds\n", (millis() - start) / 1000);
        }
    }
}

void handleServerRoot()
{
    // load index.html from SD card.
    server.send(200, "text/html", loadHtml(html_root+"index.htm"));
}

void handleSDFileMan()
{
    server.send(200, "text/html", "<h1>Server</h1>");
}

void handleServerClient()
{
    // handle server
    server.handleClient();
}
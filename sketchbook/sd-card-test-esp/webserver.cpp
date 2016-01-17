#include "webserver.h"

ESP8266WebServer server;

static String html_root = "/html/";
static String gcode_root = "/gcode/";

void setupWebServer()
{   
    server.on("/", handleServerRoot);
    server.on("/SDFileMan", handleSDFileMan);
    server.on("/uploadhtml", HTTP_GET, uploadHtml);
    server.on("/uploadgcode", HTTP_GET, uploadGcode);
    server.on("/upload", HTTP_POST, upload, handleFileUpload);
    server.on("/action", action);
    server.onFileUpload(handleFileUpload);
    server.onNotFound(notFound);

    server.begin();
    Serial.println("HTTP Server started.");
}

void notFound()
{
    Serial.printf("server.uri: %s\n", server.uri().c_str());
    server.send(200, "text/plain", "h");
}

String loadHtml(String path)
{
    String content;
    File html;
    // Serial.printf("loading page from: %s\n", path.c_str());
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

void upload()
{
    Serial.println("Upload.");
    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/html", "<html><head><meta http-equiv=\"refresh\" content=\"0; url=/\" /></head></html>");
}


File uploadFile;
void handleFileUpload()
{
    static unsigned long int start;
        
    // then handle the file uploading.
    HTTPUpload& upload = server.upload();
    
    String root = "";
    String fname = upload.filename;
    fname.toLowerCase();
    if(fname.endsWith(".g") || fname.endsWith(".gco") ||
       fname.endsWith(".gcode"))
    {
        root = gcode_root;
    }
    else if(fname.endsWith(".htm") || fname.endsWith(".html"))
    {
        root = html_root;
    }
    else
    {
        return;
    }

    if(upload.status == UPLOAD_FILE_START)
    {
        start = millis();
        String filepath = root + upload.filename;
        if(SD.exists((char *)filepath.c_str()))
        {
            SD.remove((char *)filepath.c_str());
        }
        uploadFile = SD.open((char *)filepath.c_str(), FILE_WRITE);
        Serial.println("uploadfile");
        Serial.print("Upload: START, filename: ");
        Serial.println(filepath);
    }
    else if(upload.status == UPLOAD_FILE_WRITE)
    {
        if(uploadFile)
        {
            uploadFile.write(upload.buf, upload.currentSize);
            uploadFile.flush();
            Serial.print("Upload: WRITE, Bytes: ");
            Serial.println(upload.currentSize);
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
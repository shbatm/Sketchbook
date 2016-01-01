#include "webserver.h"

static ESP8266WebServer server(80);

static File uploadFile;

static String html_root = "/html/";
static String gcode_root = "/gcode/";

static const char* serverIndex = "<form method='POST' action='/upload' enctype='multipart/form-data'><input type='file' name='upload'><input type='submit' value='upload'></form>";

void setupWebServer()
{
    Serial.println("Setting up Server.");
    
    server.on("/", handleServerRoot);
    server.on("/SDFileMan", handleSDFileMan);
    server.on("/uploadhtml", uploadHtml);
    server.on("/uploadgcode", uploadGcode);
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
        content = "<h6>" + path + "Content not found.</h6>";
    }
    return content;
}

void uploadHtml()
{
    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/html", loadHtml(""));
}

void uploadGcode()
{
    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/html", loadHtml(""));
}

void handleFileUpload()
{
    String content = "A test.";
    bool isnot_html_upload = (server.uri() != "/uploadHtml");
    bool isnot_gcode_upload = (server.uri() != "/uploadGcode");
    
    if(isnot_html_upload && isnot_gcode_upload) return;
    
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
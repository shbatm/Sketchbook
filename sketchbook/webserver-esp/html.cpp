#include "html.h"

String css =
"<style>"
"#header {"
"    background-color:black;"
"    color:white;"
"    text-align:center;"
"    padding:5px;"
"}"
"#nav {"
"    line-height:30px;"
"    background-color:#eeeeee;"
"    height:500px;"
"    width:150px;"
"    float:left;"
"    padding:5px;"
"}"
"#section {"
"    width:350px;"
"    float:left;"
"    padding:10px;"
"}"
"#footer {"
"    background-color:black;"
"    color:white;"
"    clear:both;"
"    text-align:center;"
"    padding:5px;"
"}"
"#divWifiSettings {"
"    width:400px;"
"    height:330px;"
"    border-collapse: collapse;"
"    color:black;"
"    border-style: solid;"
"    border-width: 1px;"
"    padding: 10px;"
"}"
"</style>" 
;
  
String divHeader = 
"<div id='header'>"
"<h1> Esp-Info</h1>"
"</div>"
;

String divNav =
"<div id='nav'>"
"<a href=\"wifisettings\">WiFi Settings.</a><br>"
"<a href=\"ledsettings\">led settings.</a><br>"
"<a href=\"./\">Front page.</a><br>"
"</div>"
;

String divSection =
"<div id='section'>"
"<h1> Welcome there!</h1>"
"ipstr"
"</div>"
;

String divFooter =
"<div id='footer'>"
"</div>"
;

void handleStripControl()
{
  String text = "";
  for(int i = 0;i < server.args(); i++)
  {
    Serial.println(server.argName(i) + ": " + server.arg(i));
    if(server.argName(i) == "pincode")
    {
      // stripcontrol->pincode = server.argName(i).toInt();
    }
    else if(server.argName(i) == "effect");
    else if(server.argName(i) == "brightness");
    else if(server.argName(i) == "var0");
    else if(server.argName(i) == "var1");
    else if(server.argName(i) == "var2");
    else
    {
      text = "";
    }
  }
  server.send(200, "text/html", "<h1>LedControl</h1><br>" + text);
}

void handleWiFiSettings()
{
  // get inputs.
  for(int i = 0;i < server.args(); i++)
  {
    if(server.argName(i) == "ssid");
    if(server.argName(i) == "pass");
    if(server.argName(i) == "confirmSta");
    if(server.argName(i) == "bssid");
    if(server.argName(i) == "bpass");
    if(server.argName(i) == "confirmAp");
  }
  // serve page.
  String InputTableSta = 
  "ssid: <br>"
  "<input value=\"" + sta_ssid + "\" type=\"text\" name=\"ssid\"><br>"
  "password: <br>"
  "<input value=\"\" type=\"password\" name=\"pass\"><br>"
  ;

  String InputTableAp =
  "ssid: <br>"
  "<input value=\"" + ap_ssid + "\" type=\"text\" name=\"bssid\"><br>"
  "password: <br>"
  "<input value=\"\" type=\"password\" name=\"bpass\"><br>"
  ;

  String divWifiSettings = 
  "<div id='divWifiSettings'>"
  "<form action=\"\" method=\"GET\">"
  "<h3>Station settings:</h3>" +
  InputTableSta +
  "<h3>Acces Point settings:</h3>" +
  InputTableAp + 
  "<input type=\"submit\" value=\"Confirm\" name=\"confirmSta\">"
  "</form>"
  "</div>"
  ;

  String htmlWiFiSet = 
  "<!DOCTYPE html>"
  "<html>"
  "<title> esp-light page. </title>"
  "<head>" + css + "</head>"
  "<body>" +
  divHeader +
  divNav +
  divWifiSettings +
  divFooter + 
  "</body>"
  "</html>"
  ;
  server.send(200, "text/html", htmlWiFiSet);
}

void handleLedSettings()
{
  server.send(200, "text/html", "<h1> led settings</h1>");
}

void handleRoot()
{
  String htmlRoot = 
  "<!DOCTYPE html>"
  "<html>"
  "<title> esp-light page. </title>"
  "<head>"+css+"</head>"
  "<body>" +
  divHeader +
  divNav +
  divSection +
  divFooter + 
  "</body>"
  "</html>"
  ;
  server.send(200, "text/html", htmlRoot);
}
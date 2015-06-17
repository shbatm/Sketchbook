#include "htmlroot.h"

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
"    height:300px;"
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
"</style>" 
;
  
String divHeader = 
"<div id='header'>"
"<h1> City Gallery</h1>"
"</div>"
;

String divNav =
"<div id='nav'>"
"<a href=\"wifisettings\">WiFi Settings.</a><br>"
"<a href=\"ledsettings\">led settings.</a><br>"
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

void handleWiFiSettings(){
  String htmlWiFiSet = 
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
#include "html.h"
#include "stripcontrol.h"

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
"    width:auto;"
"    height:auto;"
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

String iptostr(IPAddress ip)
{
  String ipstr;
  for(int i = 0; i< 4; i++)
  {
    if(i == 3)
    {
      ipstr += String(ip[i]);
    }
    else
    {
      ipstr += String(ip[i]);
      ipstr += ".";
    }
  }
  return ipstr;
}

void urldecode2(char *dst, const char *src)
{
  char a, b;
  while (*src) {
    if ((*src == '%') &&
      ((a = src[1]) && (b = src[2])) &&
      (isxdigit(a) && isxdigit(b))) {
      if (a >= 'a')
        a -= 'a'-'A';
      if (a >= 'A')
        a -= ('A' - 10);
      else
        a -= '0';
      if (b >= 'a')
        b -= 'a'-'A';
      if (b >= 'A')
        b -= ('A' - 10);
      else
        b -= '0';
      *dst++ = 16*a+b;
      src+=3;
    } 
    else {
      *dst++ = *src++;
    }
  }
  *dst++ = '\0';
}

String decodeB64(String text)
{
  char buff[100];
  urldecode2(buff, text.c_str());
  return String(buff);
}

void handleWiFiSettings()
{
  // get inputs.
  String debug = "\n";
  if(server.args())
  {
    sta_ssid = decodeB64(server.arg("ssid"));
    sta_pass = decodeB64(server.arg("pass"));
    board_name = decodeB64(server.arg("boardname"));
    accesPin = server.arg("accesPin").toInt();
    settingsStore();
  }
  // serve page.
  String InputTableSta = 
  "ssid: <br>"
  "<input value=\"" + sta_ssid + "\" type=\"text\" name=\"ssid\"><br>"
  "password: <br>"
  "<input value=\"" + sta_pass + "\" type=\"text\" name=\"pass\"><br>"
  ;

  String InputTableAp =
  "boardname: <br>"
  "<input value=\"" + board_name + "\" type=\"text\" name=\"boardname\"><br>"
  ;

  String InputTableCode = 
  "EspLight Code: <br>"
  "<input value=\"" + String(accesPin) + "\" type=\"text\" name=\"accesPin\"><br>"
  ;

  String divWifiSettings = 
  "<div id='divWifiSettings'>"
  "<form action=\"\" method=\"GET\">"
  "<h3>EspLight Settings:</h3>" +
  InputTableSta +
  InputTableAp +
  InputTableCode +
  "<input type=\"submit\" value=\"Save\" name=\"confirm\">"
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

void handleRoot()
{
  handleWiFiSettings();
}
#include "html.h"

String css =
"<style>"
"#header {"
"    background-color:black;"
"    color:white;"
"    text-align:center;"
"    padding:5px;"
"    border-radius:15px;"
"}"
"#nav {"
"    line-height:30px;"
"    background-color:#9999FF;"
"    height:320px;"
"    width:150px;"
"    float:left;"
"    padding:5px;"
"    border-radius:15px;"
"}"
"#section {"
"    width: auto;"
"    float:left;"
"    padding:10px;"
"}"
"#footer {"
"    background-color:black;"
"    color:white;"
"    clear:both;"
"    text-align:center;"
"    padding:5px;"
"    border-radius:15px;"
"}"
"#divWifiSettings {"
"    float:left;"
"}"
"#divWifiNetworks {"
"    float:right;"
"    background-color:#9999FF;"
"    border-radius:15px;"
// "    padding-top:20%;"
// "    padding-left:20%;"
"    padding:auto;"
"    margin:auto;"
"}"
"</style>" 
;
  
String divHeader = 
"<div id='header'>"
"<h1> EspLight.</h1>"
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

String getAvailableNetworks()
{
  String networks = "";
  int n = WiFi.scanNetworks();
  if(n == 0)
  {
    return String("None available <br>");
  }
  else
  {
    networks += "Number of networks: " + String(n) + "<br>";
    networks += "Found: <br>";
    for(int i = 0; i < n; i++)
    {
      networks += (i+1) + ": ";
      networks += WiFi.SSID(i);
      networks += " (";
      networks += WiFi.RSSI(i);
      networks += ")";
      networks += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "<br>" : "*<br>";
      delay(10);
    }
  }
  return networks;
}

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
    accessPin = decodeB64(server.arg("accessPin")).toInt();
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
  "<input value=\"" + String(accessPin) + "\" type=\"text\" name=\"accessPin\"><br>"
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

  String divWifiNetworks =
  "<div id='divWifiNetworks'>" +
  getAvailableNetworks() + 
  "</div>"
  ;

  String content =
  "<div id='section'>" +
  divWifiSettings +
  divWifiNetworks +
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
  content +
  divFooter + 
  "</body>"
  "</html>"
  ;
  
  server.send(200, "text/html", htmlWiFiSet);
}

void handleLedSettings()
{
  // get inputs.
  String debug = "\n";
  if(server.args())
  {
    for(int i = 0; i < server.args(); i++)
    {
      Serial.print(server.arg(i));
      Serial.print(": ");
      Serial.println(server.arg(i).length());
    }

    if(server.arg("stripselect") == String("ws2812"))
    {
      stripselect = WS2812;
    }
    else if(server.arg("stripselect") == String("ws2801"))
    {
      stripselect = WS2801;
    }
    else if(server.arg("stripselect") == String("analog"))
    {
      stripselect = ANALOGSTRIP;
    }
    striplen = decodeB64(server.arg("striplen")).toInt();
    setupStrips(striplen);
    settingsStore();
  }
  String checked = "checked";
  String ws2801checked = (stripselect == WS2801) ? checked : "";
  String ws2812checked = (stripselect == WS2812) ? checked : "";
  String analogchecked = (stripselect == ANALOGSTRIP) ? checked : "";
  String ledSettings = 
  "<div id='section'>"
  "<form action=\"\" method=\"GET\">"
  "select strip type: <br>"
  "<input type=\"radio\" name=\"stripselect\" value=\"ws2812\"" + ws2812checked + ">ws2812 strip.<br>"
  "<input type=\"radio\" name=\"stripselect\" value=\"ws2801\"" + ws2801checked + ">ws2801 strip.<br>"
  "<input type=\"radio\" name=\"stripselect\" value=\"analog\"" + analogchecked + ">analog strip.<br>"
  "<br>"
  "input ledlength (amount of leds):<br>"
  "<input type=\"text\" value=\"" + String(striplen) + "\" name=\"striplen\"><br>"
  "<input type=\"submit\" value=\"Save\" name=\"confirm\">"
  "</form>"
  "</div>"
  ;


  String htmlRoot = 
  "<!DOCTYPE html>"
  "<html>"
  "<title> esp-light page. </title>"
  "<head>"+css+"</head>"
  "<body>" +
  divHeader +
  divNav +
  ledSettings +
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
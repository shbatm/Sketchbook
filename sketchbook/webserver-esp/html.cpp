#include "html.h"

String css = 
"html   {"
"    height: 100%;"
"    }"
"body  {"
"    height: 100%;"
"    margin: 0;"
"    padding: 0;"
    /* Permalink - use to edit and share this gradient: http://colorzilla.com/gradient-editor/#257a82+0,123845+100 */
"    background: #257a82;" /* Old browsers */
"    background: -moz-radial-gradient(center, ellipse cover,  #257a82 0%, #123845 100%); "/* FF3.6+ */
"    background: -webkit-gradient(radial, center center, 0px, center center, 100%, color-stop(0%,#257a82), color-stop(100%,#123845));" /* Chrome,Safari4+ */
"    background: -webkit-radial-gradient(center, ellipse cover,  #257a82 0%,#123845 100%);" /* Chrome10+,Safari5.1+ */
"    background: -o-radial-gradient(center, ellipse cover,  #257a82 0%,#123845 100%);" /* Opera 12+ */
"    background: -ms-radial-gradient(center, ellipse cover,  #257a82 0%,#123845 100%);" /* IE10+ */
"    background: radial-gradient(ellipse at center,  #257a82 0%,#123845 100%);" /* W3C */
"    filter: progid:DXImageTransform.Microsoft.gradient( startColorstr='#257a82', endColorstr='#123845',GradientType=1 );" /* IE6-9 fallback on horizontal gradient */
"    color: #fff;"
"    overflow: auto;"
"    }"
"h1,"
"h2  {"
"   text-align: center;"
"   text-transform: uppercase;"
"   font-weight: 100; "
"   letter-spacing: 0.1em;"
"   text-shadow: 0px 2px 2px rgba(11,61,85,0.8);"
"  }"
"h2  {"
"   font-size: 1.6em;"
"   color: #4c7183;"
"  }"
".row .panel "
"      {"
"      background-color: #0b3d55;"
"      border-radius: 3px;"
"      padding: 18px 60px 45px;"
"      box-shadow: 0px 0px 5px rgba(11,61,85,0.8);"
"      overflow: hidden;"
"      }"
".row .menu  {"
      /* Permalink - use to edit and share this gradient: http://colorzilla.com/gradient-editor/#6cbec2+0,4ba0a4+100 */
"      background: #6cbec2; "/* Old browsers */
"      background: -moz-linear-gradient(top,  #6cbec2 0%, #4ba0a4 100%);" /* FF3.6+ */
"      background: -webkit-gradient(linear, left top, left bottom, color-stop(0%,#6cbec2), color-stop(100%,#4ba0a4));" /* Chrome,Safari4+ */
"      background: -webkit-linear-gradient(top,  #6cbec2 0%,#4ba0a4 100%);" /* Chrome10+,Safari5.1+ */
"      background: -o-linear-gradient(top,  #6cbec2 0%,#4ba0a4 100%);" /* Opera 11.10+ */
"      background: -ms-linear-gradient(top,  #6cbec2 0%,#4ba0a4 100%);" /* IE10+ */
"      background: linear-gradient(to bottom,  #6cbec2 0%,#4ba0a4 100%);" /* W3C */
"      filter: progid:DXImageTransform.Microsoft.gradient( startColorstr='#6cbec2', endColorstr='#4ba0a4',GradientType=0 );" /* IE6-9 */
"      display: block;"
"      border: 1px solid #58b0b3;"
"      border-radius: 3px;"
"      padding: 12px;"
"      font-size: 1.6em;"
"      text-align: center;"
"      }"
".row .menu { color: #fff;}"
".row .menu:hover {color: #fff; text-decoration: none;}"
".row .active  {"
"        box-shadow: 0px 0px 20px rgba(99,185,188,0.5);"
"        }"
".row .active:hover  {"
"          box-shadow: 0px 0px 20px rgba(99,185,188,0.75);"
"          transition: box-shadow 0.3s ease;"
"          }"
"label   {"
"      color: #52b1b5;"
"      text-transform: uppercase;"
"      font-weight: 100;"
"      letter-spacing: 0.1em;"
"      margin: 2em 0 0;"
"      transition: color 0.3s ease;"
"      }"
"label:hover {color: #72d3d6;}"
"input[type=text]"
"      {"
"      width: 100%;"
"      background-color: transparent;"
"      border: 0;"
"      border-bottom: 1px solid #52b1b5;"
"      }"
"input[type=radio]"
"      {"
"      margin: 0 1em 0 0;"
"      vertical-align: middle;"
"      }"
"input[type=submit]"
"      {"
"      width: 80%;"
"      background-color: #215870;"
"      border: 0;"
"      border-radius: 6px;"
"      margin: 3em auto 0;"
"      padding: 12px;"
"      display: block;"
"      text-transform: uppercase;"    
"      }"
".center   {"
"      margin: 0 auto;"
"      }"
;

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
      networks += "<li>";
      networks += (i+1) + ": ";
      networks += WiFi.SSID(i);
      networks += " (";
      networks += WiFi.RSSI(i);
      networks += ")";
      networks += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "<br>" : "*<br>";
      networks += "</li>";
      delay(10);
      // limit search to 5 ssid's
      if(i == 5)
      {
        return networks;
      }
    }
  }
  return networks;
}

String generateWifiHtml()
{
  // get form data.
  if(server.args())
  {
    sta_ssid = decodeB64(server.arg("ssid"));
    sta_pass = decodeB64(server.arg("pwd"));
    board_name = decodeB64(server.arg("boardname"));
    accessPin = decodeB64(server.arg("code")).toInt();
    settingsStore();
  }

  /* hold html that show which access points are available*/
  String available_aps = getAvailableNetworks();

  String html = 
  "<!doctype html>"
  "<html lang=\"\">"
  "    <head>"
  "        <meta charset=\"utf-8\">"
  "        <meta http-equiv=\"x-ua-compatible\" content=\"ie=edge\">"
  "        <title>EspLight : WiFi</title>"
  "        <meta name=\"description\" content=\"\">"
  "        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"

  "        <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css\">"
  "        <script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js\"></script>"
  "        <style>" +
             css +     
  "        </style>"
  "    </head>"
  "    <body>"
  "    <div class=\"container\">"
  "      <div class=\"col-md-4\"></div>"
  "      <div class=\"col-md-4\">"
  "        <div class=\"row\">"
  "          <h1>EspLight</h1>"
  "        </div>"
  "        <div class=\"row\">"
  "          <div class=\"col-md-6\"><a href=\"wifisettings\" class=\"menu active\">WiFi</a></div>"
  "          <div class=\"col-md-6\"><a href=\"ledsettings\" class=\"menu\">LED</a></div>"
  "        </div>"
  "        <div class=\"row\">&nbsp;</div>"       
  "        <div class=\"row\">"
  "          <div class=\"panel\">"
  "            <h2>Settings</h2>"
  "            <div class=\"center\">"
  "              <form action=\"wifisettings\" method=\"POST\">"
  "              <label for=\"ssid\">SSID</label><br/><input type=\"text\" value=" + sta_ssid + " name=\"ssid\">"
  "              <label for=\"ssid\">password</label><br/><input type=\"text\" value=" + sta_pass + " name=\"pwd\">"
  "              <label for=\"ssid\">board name</label><br/><input type=\"text\" value=" + board_name + " name=\"boardname\">"
  "              <label for=\"ssid\">esplight code</label><br/><input type=\"text\" value=" + String(accessPin) + " name=\"code\">"
  "              <input type=\"submit\" value=\"save\">"
  "              </form>"
  "            </div>"
  "          </div>"
  "          <div class=\"panel\">"
  "            <h2>Networks</h2>"
  "            <div class=\"center\">"
  "              <ul>" +
                   available_aps +
  "              </ul>"
  "            </div>"
  "          </div>"
  "        </div>"
  "      </div>"
  "      <div class=\"col-md-4\"></div>"
  "    </div>"
  "    </body>"
  "</html>"
  ;
  return html;
}

String generateLedHtml()
{

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

  String html = 
  "<!doctype html>"
  "<html lang=\"\">"
  "    <head>"
  "        <meta charset=\"utf-8\">"
  "        <meta http-equiv=\"x-ua-compatible\" content=\"ie=edge\">"
  "        <title>EspLight : WiFi</title>"
  "        <meta name=\"description\" content=\"\">"
  "        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"

  "        <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css\">"
  "        <script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js\"></script>"
  "        <style>" +
             css +     
  "        </style>"
  "    </head>"
  "    <body>"
  "    <div class=\"container\">"
  "      <div class=\"col-md-4\"></div>"
  "      <div class=\"col-md-4\">"
  "        <div class=\"row\">"
  "          <h1>EspLight</h1>"
  "        </div>"
  "        <div class=\"row\">"
  "          <div class=\"col-md-6\"><a href=\"wifisettings\" class=\"menu\">WiFi</a></div>"
  "          <div class=\"col-md-6\"><a href=\"ledsettings\" class=\"menu active\">LED</a></div>"
  "        </div>"
  "        <div class=\"row\">&nbsp;</div> "
  "        <div class=\"row\">"
  "          <form action=\"\" method=\"POST\">"
  "          <div class=\"panel\">"
  "            <h2>Strip type</h2>"
  "            <div class=\"center\">"
  "              <input type=\"radio\" name=\"stripselect\" id=\"analog\" value=\"analog\"" + analogchecked + "><label for=\"analog\">Analog</label><br/>"
  "              <input type=\"radio\" name=\"stripselect\" id=\"ws2812\" value=\"ws2812\"" + ws2812checked + "><label for=\"ws2812\">WS2812</label><br/>"
  "              <input type=\"radio\" name=\"stripselect\" id=\"ws2801\" value=\"ws2801\"" + ws2801checked + "><label for=\"ws2801\">WS2801</label><br/>"
  "            </div>"
  "          </div>"
  "          <div class=\"panel\">"
  "            <h2>Leds</h2>"
  "            <div class=\"center\">"
  "              <label for=\"lednumber\">Number of leds</label><input type=\"text\" value=" + String(striplen) + " name=\"striplen\"><br/> "
  "              <input type=\"submit\" value=\"save\" />"
  "            </div>"
  "          </div> "
  "          </form>"
  "        </div>"
  "      </div>"
  "      <div class=\"col-md-4\"></div>"
  "    </div>"
  "    </body>"
  "</html>"
  ;
  return html;
}

void handleWiFiSettings()
{
  server.send(200, "text/html", generateWifiHtml());
}

void handleLedSettings()
{
  server.send(200, "text/html", generateLedHtml());
}

void handleRoot()
{
  handleWiFiSettings();
}
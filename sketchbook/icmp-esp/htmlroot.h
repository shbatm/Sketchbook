#ifndef HTMLROOT_H
#define HTMLROOT_H

void handleRoot()
{
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
  "    width:100px;"
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
  "London<br>"
  "Paris<br>"
  "Tokyo<br>"
  "</div>"
  ;

  IPAddress ip = WiFi.localIP();
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
  
  String divSection =
  "<div id='section'>"
  + ipstr +
  "</div>"
  ;

  String divFooter =
  "<div id='footer'>"
  "Copyright me"
  "</div>"
  ;
  
  String html = 
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
  server.send(200, "text/html", html);
}

#endif

#include <EtherCard.h>

#define ETHERRESETPIN 9

static byte mac[] = {'l','i','c','h','t','1'};
static byte myip[] = {10, 42, 4, 50};

byte Ethernet::buffer[170*3];
BufferFiller bfill;

const char hostname[] = "lightpaper";

void setup()
{
    //make the reset pin low for 100 ms, to reset the ENC28J60
    pinMode(ETHERRESETPIN, OUTPUT);
    digitalWrite(ETHERRESETPIN, LOW);
    delay(100);
    digitalWrite(ETHERRESETPIN, HIGH);
    delay(100);
    if(ether.begin(sizeof(Ethernet::buffer), mac))
    {
        Serial.println("Failed to acces Ethernet controller");
    }
    delay(5000);
    ether.enableBroadcast();
    ether.dhcpSetup(hostname, true);
}

static word homepage()
{
    long t = millis()/1000;
    word h = t/3600;
    byte m = (t/60)%60;
    byte s = t%60;
    bfill = ether.tcpOffset();
    bfill.emit_p(PSTR(
        "HTTP/1.0 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Pragma: no-cache\r\n"
        "\r\n"
        "<meta http-equiv='refresh' content='1'/>"
        "<title>RBBB server</title>"
        "<h1>$D$D:$D$D:$D$D</h1>"),
        h/10, h%10, m/10, m%10, s/10, s%10);
    return bfill.position();
}

void loop()
{
    word len = ether.packetReceive();
    word pos = ether.packetLoop(len);
    if(pos) // check if valid tcp data received
    {
        ether.httpServerReply(homepage());
    }
}
#include <Arduino.h>
#include "controller.h"
#include "debugprint.h"

#if STATIC
static byte myip[] = { 192,168,1,200 };
static byte gwip[] = { 192,168,1,1 };
#endif

uint8_t udpbuffer[600];

extern void Transmit(uint8_t* data, uint16_t size, uint16_t sourceport, const uint8_t* destip, uint16_t destport);

CController::CController() : m_artnet(*this, udpbuffer, 0, 0)
{
}

#define DATAPIN 2 //brn
#define CLOCKPIN 4 //black/white
#define ETHERRESETPIN 9

void CController::Initialize()
{

  //let the pin rise if the jumper is open
  delay(10);

  //init the led timestamp
  m_ledshowtime = millis();

  //make the reset pin low for 100 ms, to reset the ENC28J60
  pinMode(ETHERRESETPIN, OUTPUT);
  digitalWrite(ETHERRESETPIN, LOW);
  delay(100);
  digitalWrite(ETHERRESETPIN, HIGH);
  delay(100);

  wdt_reset();
  // initialize server
//  uint8_t mac[] = {'l','i','c','h','t','1'};
//  if (ether.begin(sizeof(Ethernet::buffer), mac))
//  {
//    DBGPRINT("Ethernet controller set up\n");
//  }
//  else
//  {
//    DBGPRINT("Failed to set up Ethernet controller\n");
//    for(;;); //wait for the watchdog timer reset
//  }

  //delay for the ethernet switch to bring up stuff
  wdt_reset();
  delay(5000);
  wdt_reset();

  //enable broadcast for dhcp and art-net
  

#if STATIC
  ether.staticSetup(myip, gwip);
#else
  DBGPRINT("Requesting ip address using DHCP\n");

#endif

  DBGPRINT("IP: %i.%i.%i.%i\n", ether.myip[0], ether.myip[1], ether.myip[2], ether.myip[3]);
  DBGPRINT("GW: %i.%i.%i.%i\n", ether.gwip[0], ether.gwip[1], ether.gwip[2], ether.gwip[3]);
  DBGPRINT("DNS: %i.%i.%i.%i\n", ether.dnsip[0], ether.dnsip[1], ether.dnsip[2], ether.dnsip[3]);
 // DBGPRINT("MASK: %i.%i.%i.%i\n", ether.mymask[0], ether.mymask[1], ether.mymask[2], ether.mymask[3]);

  SetPortAddressFromIp();
  //EtherCard::dhcp_renewed = false;
  m_artnet.Initialize();
  wdt_reset();

  //init last valid data timestamp
  m_validdatatime = millis();
}

void CController::SetPortAddressFromIp()
{
  //store the ip address and subnetmask into uint32_t
  uint32_t address = 0;
  uint32_t mask = 0;
  for (uint8_t i = 0; i < 4; i++)
//  { 
//    address |= (uint32_t)ether.myip[i] << (3 - i) * 8;
//    mask |= (uint32_t)ether.netmask[i] << (3 - i) * 8;
//  }
  // assign ip to address.
  address = 0;

  //do a bitwise and with the subnetmask to get the host address
  uint32_t hostaddress = address & ~mask;
  //subtract one from the host address, take the 15 least significant bits, and use it as the art-net portaddress
  uint16_t portaddress = (hostaddress - 1) & 0x3FFF;

  m_artnet.SetPortAddress(portaddress);
  //set the ArtPollReply delay so that if a lot of these controllers are on the network,
  //and their ip addresses are numbered sequentually,
  //a controller will send ArtPollReply every 2 milliseconds
  //this prevents the ENC28J60 buffer filling up with a shitload of ArtPollReply broadcasts
  uint16_t delay = (hostaddress % 1000) * 2;
  m_artnet.SetPollReplyDelay(delay);

  DBGPRINT("address:%lu\n", address);
  DBGPRINT("mask:%lu\n", mask);
  DBGPRINT("hostaddress:%lu\n", hostaddress);
  DBGPRINT("portaddress:%u\n", portaddress);
  DBGPRINT("net:%i subnet:%i universe:%i\n", (portaddress >> 8) & 0xFF, (portaddress >> 4) & 0xF, portaddress & 0xF);
  DBGPRINT("artpollreply delay: %i\n", delay);

}

void CController::Process()
{
  uint32_t now = millis();
  //if valid art-net data has been received in the last minute,
  //reset the watchdog timer
  if (now - m_validdatatime < 60000)
    wdt_reset();

  m_artnet.Process(now);
}

void CController::HandlePacket(byte ip[4], uint16_t port, uint8_t* data, uint16_t len)
{
  m_artnet.HandlePacket(ip, port, data, len);
}

void CController::Transmit(uint8_t* data, uint16_t size, uint16_t sourceport, const uint8_t* destip, uint16_t destport)
{
  ::Transmit(data, size, sourceport, destip, destport);
}

void CController::OnDmxData(uint8_t* data, uint16_t channels)
{
  m_ledshowtime = millis();
}

void CController::OnValidData()
{
  m_validdatatime = millis();
  wdt_reset();
}


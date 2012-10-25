
#include <Wire.h>;
#include <inttypes.h>;

void setup()
{
  Wire.begin(); //Initialisatie van de wire klasse
  
  /*
      -- initialisatie --
      De eerste twee commands die naar de MCP23016 gestuurd worden, zet de poorten als input of als output.
      Een command bestaat altijd uit 2 x 8 bits. (met de functie setData worden de twee bitreeksen als hexadecimaal verstuurd)
      De eerste 8 bits geeft aan welk command gestuurd word (in de initialisatie 00000110 (0x06) en 00000111 (0x07), dit zijn de commands IODIR0 en IODIR1)
      De tweede 8 bits geven aan welke poorten input zijn en welke output (0 = output, 1 = input)
      Bijvoorbeeld:
        sendData(0x06, 0x00); //binair 00000110, 00000000 //Zet alle 8 poorten op GP0 op output
        sendData(0x07, 0x05); //binair 00000111, 00000101 //Zet poort 1 en 3 van GP1 op input, de rest op output
  */
  
  sendData(0x06, 0x00); //binair 00000110, 00000000 //Zet alle 8 poorten op GP0 op output
  sendData(0x07, 0x00); //binair 00000111, 00000000 //Zet alle 8 poorten op GP1 op output
}

void loop()
{
  /*
      -- poorten hoog of laag toewijzen --
      Het command om te schrijven naar de poorten is 00000000 (0x00) voor GP0, 00000001 (0x01) voor GP1
      De tweede 8 bits geven aan welke poorten hoog zijn en welke laag (0 = laag, 1 = hoog)
      Bijvoorbeeld:
        sendData(0x00, 0x04); //binair 00000000, 00000100 //Zet poort 3 van GP0 op hoog, de rest laag
        sendData(0x01, 0x02); //binair 00000000, 00000010 //Zet poort 2 van GP0 op hoog, de rest laag
        
        23 = GP0.2
        24 = GPO.3
  */
  sendData(0x00, 0x04); //binair 00000000, 00000100 //zet poort 2 van GP0 hoog, de rest laag, pin 23 van de MCP23016 (GP0.2)
  
  delay(500); //wacht 500 ms
  
  sendData(0x00, 0x08); //binair 00000000, 00001000 //zet poort 3 van GP0 hoog, de rest laag, pin 24 van de MCP23016 (GP0.3)
  
  delay(500); //wacht 500 ms
}

void sendData(uint8_t byte1, uint8_t byte2)
{
  Wire.beginTransmission(0x20);
  Wire.send(byte1);
  Wire.send(byte2);
  Wire.endTransmission();
}

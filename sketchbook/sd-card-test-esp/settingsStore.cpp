#include "settingsStore.h"

String magicEepromWord = "deadbeaf";
uint8_t magicWordLen;

void setupSettingsStore()
{
    // prepare eeprom for use.
    EEPROM.begin(EEPROMSIZE);
    delay(10);
}

// store the magic word for checking.
void storeMagicStr(int& addr)
{
    storeString(magicEepromWord, addr);
}

// stores a string into eeprom/flas plus nullterminator.
void storeString(String string, int &addr)
{
  // get a reference to the original c string.
  const char *str = string.c_str();
  // get the length of that string.
  int str_len = strlen(str);
  int i;
  // loop over the length of the string.
  // and store the bytes. and zero terminator.
  for(i = 0; i <= str_len; i++)
  {
    // write the bytes into the eeprom (flash)
    EEPROM.write(addr+i, str[i]);
    delay(0);
  }
  addr += i;
}

// stores a int into eeprom/flash
void storeInt(int value, int &addr)
{
  char fmtstr[100];
  int i;
  for(i = 0; i < sizeof(value); i++)
  {
    EEPROM.write(addr+i, (value>>(i*8)&0xff));
    delay(0);
  }
  addr += i;
}

void storeDone()
{
    EEPROM.commit();
}

// loads a string from eeprom/flash
String loadString(int &addr)
{
  String text = "";
  char read = EEPROM.read(addr);
  /* Basicly how you read any string in C. */
  while(read != '\0')
  {
    text += read;
    addr++;
    read = EEPROM.read(addr);
    delay(0);
  }
  //acount for zero terminator.
  addr++;
  return text;
}

// checks for the magic string in eeprom indicating valid settings.
// or that settings have been written before.
bool magicStrPresent(int &addr)
{
  char text[] = "deadbeaf";
  uint8_t magicWordLen = magicEepromWord.length();
  for(int i = 0; i < magicWordLen; i++)
  {
    text[i] = EEPROM.read(addr);
    addr++;
    delay(0);
  }
  //acount for zero terminator
  addr++;
  Serial.printf("read: %s\n", text);
  Serial.printf("should be: %s\n", magicEepromWord.c_str());
  return (String(text) == magicEepromWord);
}

// loads a int from eeprom/flash
int loadInt(int &addr)
{
  int value = 0;
  int i;
  for(i = 0; i < 0x04; i++)
  {
    char byte = EEPROM.read(addr+i);
    value |= (byte << (8*i));
  }
  addr += i;
  return value;
}

void storeStruct(void *data_source, size_t size)
{
  EEPROM.begin(size * 2);
  for(size_t i = 0; i < size; i++)
  {
    char data = ((char *)data_source)[i];
    EEPROM.write(i, data);
  }
  EEPROM.commit();
}

void loadStruct(void *data_dest, size_t size)
{
    EEPROM.begin(size * 2);
    for(size_t i = 0; i < size; i++)
    {
        char data = EEPROM.read(i);
        ((char *)data_dest)[i] = data;
    }
}

/* Testing


void printCmpSettings(settings_t s1, settings_t s2)
{
    Serial.printf("board_name: %s | %s \n"
                  "ssid: %s | %s \n"
                  "pass: %s | %s \n",
                  s1.board_name, s2.board_name,
                  s1.ssid, s2.ssid,
                  s1.pass, s2.pass
                  );
}

void testStoreSettings()
{
    memcpy(&settings_original, &settings, sizeof(settings));
    storeStruct(&settings, sizeof(settings));
    loadStruct(&settings, sizeof(settings));
    if(!memcmp(&settings_original, &settings, sizeof(settings)))
    {
        Serial.println("settings are the same!");
        printCmpSettings(settings_original, settings);
    }
    else
    {
        Serial.println("settings are not the same!");
        printCmpSettings(settings_original, settings);
    }

    memcpy(&settings_original, &settings, sizeof(settings));
    strcpy(settings.board_name, "test_host");
    storeStruct(&settings, sizeof(settings));
    loadStruct(&settings, sizeof(settings));
    if(!memcmp(&settings_original, &settings, sizeof(settings)))
    {
        Serial.println("settings are the same!");
        printCmpSettings(settings_original, settings);
    }
    else
    {
        Serial.println("settings are not the same!");
        printCmpSettings(settings_original, settings);
    }
}
*/
//sid-test.ino
#include <SID.h>

int reg = 0;
bool startfound = false;
int startcount = 0;
SID sid;
SID sid2;

void setup()
{
    Serial.begin(115200);
    sid.begin();
} 

void loop()
{
    while(Serial.available())
    {
        char regval = Serial.read();
        sid.set_register(reg, regval);
        reg++;
        if(reg >= 25)
        {
            reg = 0;
        }
    }
}

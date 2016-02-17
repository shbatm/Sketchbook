//strtok_test.ino

char *somestring;
String command = "command";

void setup()
{
    Serial.begin(115200);
    Serial.println();
    somestring = (char *)malloc(sizeof(char)*30);
    strcpy(somestring, "Hello good day!");
    Serial.printf("start with: %s\n", somestring);
    Serial.printf("put in 'hi there testing'.\n");
    strcpy(somestring, "hi there testing.");
    Serial.printf("result: %s\n", somestring);

    Serial.printf("\nput in: 'command'.\n");
    strcpy(somestring, command.c_str());
    Serial.printf("result: %s\n", somestring);

}

void loop()
{
}

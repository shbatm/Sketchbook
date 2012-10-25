#include <IOlcd.h>
#include <Wire.h>

IOlcd lcd;

int buffSize = 64;
char screenBuf[65];
int arrayIndex = 0;

long previousMillis = 0;
long interval = 1000;

int state = 1;

void setup(){
	Serial.begin(115200);
	if(lcd.init(0x20, MCP23016)){
		Serial.println("Communication with the IOlcd works!");
		}
	else{
		Serial.println("Not connected propperly!");
		}
	lcd.begin(16,2);
	lcd.clear();
	flushBuffer();
	}

void loop(){
	unsigned long currentMillis= millis();
	pinMode(13, OUTPUT);
	if(Serial.available()!=0){
		arrayIndex = 0;
		flushBuffer();
		while(Serial.available() != 0){
			delay(20);
			screenBuf[arrayIndex] = Serial.read();
			arrayIndex++;
			if(arrayIndex == buffSize){
				arrayIndex = 0;
				}
			}
		}
	else if(currentMillis - previousMillis > interval){
			previousMillis = currentMillis;
			//Serial.println("numelements: \n\r");
			//Serial.println(countElements());
			//Serial.println("currentMillis: \n\r");
			//Serial.println(currentMillis);
			//Serial.println("What is in screen buffer: \n\r");
			//Serial.println(screenBuf);
			printToLcd();
		}
	}

void printToLcd(){
	int numElements = countElements();
	if(numElements >32){
		//Serial.println("the state: ");
		//Serial.println(state);
		state = switchLines();
		//digitalWrite(13, state);
		toggleLines(state);
		}
	else{
		singleLineDisp();
		}
	}

void singleLineDisp(void){
	for(int i=0;i<32;i++){
			if(i>=16 && screenBuf[i]!='\0'){
				lcd.setCursor((i-16), 1);
				lcd.print(screenBuf[i]);
				}
			else if(screenBuf[i]!='\0'){
				lcd.setCursor(i,0);
				lcd.print(screenBuf[i]);
				}
			}
	}

void toggleLines(int _state){
	lcd.clear();
	if(state == 1){
		singleLineDisp();
		}
	else if(state == 0){
		for(int i=32;i<64;i++){
			if(i>=48 && screenBuf[i]!='\0'){
				lcd.setCursor((i-48), 1);
				lcd.print(screenBuf[i]);
				}
			else if(screenBuf[i]!='\0'){
				lcd.setCursor((i-32), 0);
				lcd.print(screenBuf[i]);
				}
			}
		}
	}

int switchLines(){
	if(state == 1){
		state = 0;
		}
	else{
		state = 1;
		}
	return state;
	}

int countElements(void){
	int count = 0;
	for(int i = 0;i<buffSize;i++){
		if(screenBuf[i] != '\0'){
			count ++;
			}
		}
	return count;
	}

void flushBuffer(void){
	for(int i = 0;i<buffSize;i++){
		screenBuf[i] = '\0';
		}
	lcd.clear();
	}

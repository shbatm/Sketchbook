#include <IOlcd.h>
#include <Wire.h>

IOlcd lcd;

const int buffSize = 256;
char screenBuf[257];
int arrayIndex = 0;
char incommingByte = 0;

long previousMillis = 0;
long interval = 1500;

int first = 0;
int lineLen = 32;

void setup(){
	pinMode(13, OUTPUT);
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
	delay(1000);
	}

void loop(){
	unsigned long currentMillis = millis();
	if(Serial.available()>0){
		flushBuffer();
		arrayIndex = 0;
		delay(10);
		while((incommingByte = Serial.read())>0){
			//if(arrayIndex >= buffSize){
			//	arrayIndex = buffSize;
			//	}
			//int incommingByte = Serial.read();
			screenBuf[arrayIndex] = incommingByte;
			arrayIndex++;
			}
		}
	else if(currentMillis - previousMillis > interval){
			previousMillis = currentMillis;
			printScreen();
		}
	}

void printScreen(){
	int numElements = countElements();
	Serial.print("numelements: ");
	Serial.println(numElements);
	if(first == buffSize || first >= numElements){
		first = 0;
		}
	//else{
		lineDisp(first, lineLen);
		Serial.print("first before +=");
		Serial.println(first);
		first += 32;
		Serial.print("first after +=: ");
		Serial.println(first);
		Serial.print("what is in the buffer: ");
		Serial.println(screenBuf);
		//}
	}
void lineDisp(int startline ,int lineSize){
	for(int i = startline;i<(startline+lineSize);i++){
		if(i>=(startline+lineSize-16)&&screenBuf[i]=='\0'){
			lcd.setCursor((i-(startline+lineSize-16)),1);
			lcd.print(" ");
			}
		else if(screenBuf[i]=='\0'){
			lcd.setCursor((i-startline),0);
			lcd.print(" ");
			}
		if(i>=(startline+lineSize-16)&&screenBuf[i]!='\0'){
			lcd.setCursor((i-(startline+lineSize-16)),1);
			lcd.print(screenBuf[i]);
			}
		else if(screenBuf[i]!='\0'){
			lcd.setCursor((i-startline),0);
			lcd.print(screenBuf[i]);
			}
		}
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
	for(int i = 0;i<buffSize+1;i++){
		screenBuf[i] = '\0';
		}
	lcd.clear();
	}

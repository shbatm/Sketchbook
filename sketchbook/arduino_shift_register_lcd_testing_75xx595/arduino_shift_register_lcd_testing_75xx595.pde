int latchPin = 8;
int clockPin = 12;
int dataPin = 11;

int count;

int interval = 1000;
int currentMillis = 0;
int previousMillis = 0;

int interVal1 = 0;
int preTimerVal1 = 0;
int curTimerVal1 = 0;

int interVal2 = 0;
int preTimerVal2 = 0;
int curTimerVal2 = 0;

int interVal3 = 0;
int preTimerVal3 = 0;
int curTimerVal3 = 0;

void setup(){
	pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
	
	pinMode(A5, INPUT);
	pinMode(A4, INPUT);
	pinMode(5, INPUT);
	
	Serial.begin(9800);
	
	}
void loop(){
	curTimerVal3 = currentMillis = curTimerVal1 = curTimerVal2 = micros();
	interVal1 = analogRead(A5);
	interVal2 = analogRead(A5);
	interVal3 = analogRead(A4);
	if(digitalRead(5)){
		//Serial.print(digitalRead(5));
		if(count == 255){
			count = 0;
			}
		count++;
	}
	if(currentMillis - previousMillis > interval){
		previousMillis = currentMillis;
		Serial.print("the count: ");
		Serial.println(count);
		Serial.print("analogvalue5: ");
		Serial.println(interVal1);
		Serial.print("analogvalue4: ");
		Serial.println(interVal2);
		}
	if(curTimerVal1 - preTimerVal1 > interVal1){
		preTimerVal1 = curTimerVal1;
		shiftOut(dataPin, clockPin, MSBFIRST, 0xFF^count);
		latching();
		}
	if(curTimerVal2 - preTimerVal2 > interVal2){
		preTimerVal2 = curTimerVal2;
		shiftOut(dataPin, clockPin, MSBFIRST, 0x00^count);
		latching();
		}
	if(curTimerVal3 - preTimerVal3 > interVal3){
		preTimerVal3 = curTimerVal3;
		shiftOut(dataPin, clockPin, MSBFIRST, 0x00);
		latching();
		}
	}

void latching(void){
	digitalWrite(latchPin, HIGH);
	digitalWrite(latchPin, LOW);
	}

void setup(){
}
int i=0;
void loop(){
 writeData(i); 
  if(i>=64){
    i=0;
  }
  else{
    i++;
  }
  delay(100);
}

void writeData(byte Data){
  for(int i=0;i<6;i++){
    pinMode(i+2, OUTPUT);
  }
  for(int i=0;i<6;i++){
    if(Data&1<<i){
      digitalWrite(i+2, HIGH);
    }
    else{
      digitalWrite(i+2, LOW);
    }
  }
}

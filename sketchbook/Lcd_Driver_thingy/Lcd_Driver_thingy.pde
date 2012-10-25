//this is to count elements and is used in void printDisplay();
//also this is to display stuff in the right order on lcd,
//kinda like a lcd driver is thing. <By: Duality>

int countElements(byte_type inputArray[]){
  int count = 0;
  while(inputArray[count]!='\0'){
    count++;
  }
  return count;
}

//cpos as in Char Position/Index
void printDisplay(byte_type inputBuf[]){
  int len = countElements(inputBuf);
  Serial.print("the len of input string; ");
  Serial.println(len);
  int cpos = 0;
  do{
    if(isprint(inputBuf[cpos])){
      lcd.print(inputBuf[cpos]);
    }
    switch(cpos){
      case 20:
      lcd.setCursor(0,1);
      break;
      case 40:
      lcd.setCursor(0,2);
      break;
      case 60:
      lcd.setCursor(0,3);
      break;
      case 80:
      lcd.setCursor(0,0);
      break;
    }
    cpos++;
  }
  while(cpos<=len);
}

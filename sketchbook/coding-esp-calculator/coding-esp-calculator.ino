char inbyte = 0;
#define INPBUFFSIZE 1000
static char inputbuff[INPBUFFSIZE];
unsigned int inp = 0;

enum {CHAR, INT, OP, STR, FUNC};

struct _tok
{
  unsigned int value;
  uint8_t type;
};

typedef struct _tok token_t;

#define STACKSIZE 1000
token_t stack[STACKSIZE];
unsigned int sp = 0;

void push(struct _tok tok)
{
  stack[sp++] = tok;
  if(sp == STACKSIZE)
    Serial.println("stack overflow.");
}

struct _tok pop()
{
  sp--;
  if(sp == 65536)
  {
    Serial.println("stack underflow");
  }
}

bool isoperator(char c)
{
  return false;
}

int readinput(char *buffer, unsigned int limit)
{
  static unsigned int index;
  index = 0;
  if(Serial.available() > 0)
  {
    inbyte = Serial.read();
    while(inbyte != '\n')
    {
      if(Serial.available() > 0)
      {
        inputbuff[index] = inbyte;
        inputbuff[++index] = '\0';
        if(index == limit)
        {
          index = 0;
          break;
        }
        inbyte = Serial.read();
      }
      delay(0);
    }
    Serial.print(">> ");
    Serial.println(inputbuff);
  }
  return index;
}

void tokenizeInput(char *buff, unsigned int len)
{
  static unsigned int num;
  static unsigned int numi;
  static char strnum[20];
  static token_t token;
  
  for(int i = 0; i < len; i++)
  {
    // if it is a digit stor it in a number
    if(isdigit(buff[i]))
    {
      // while reading digits add to temp string holder.
      while(isdigit(buff[i]))
      {
        strnum[numi++] = buff[i++];
      }
      // set numi to 0 to start index at zero again.
      numi = 0;
      // convert to ascii rep to num.
      num = atoi(strnum);
      // clear first byte to make string empty.
      strnum[0] = '\0';
      // fill in token detail.
      token.type = INT;
      token.value = num;
      Serial.print(sizeof(token.value));
      Serial.print(":num: ");
      Serial.println(token.value);
    }
    else if(isoperator(buff[i]))
    {
    }
  }
}

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  inp = readinput(inputbuff, INPBUFFSIZE);
  tokenizeInput(inputbuff, inp);
}

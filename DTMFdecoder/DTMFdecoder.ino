//Standard PWM DC control
int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;    //M1 Direction Control
int M2 = 7;    //M1 Direction Control


int code[] = {0x06,0x01,0x03};
int codeRecieved = 0;
long codeDuration = 0;    //time of when was the first tone of the code recieved 
long maxDuration = sizeof(code)/sizeof(int)*800;


void EMoff(void)                    //EM off
{
  digitalWrite(E1,LOW);   
  digitalWrite(E2,LOW);      
}  

void EMon(char a,char b)          //EM on
{
  analogWrite (E1,a);
  digitalWrite(M1,HIGH);    
  analogWrite (E2,b);    
  digitalWrite(M2,HIGH);
}  


void setup() {
  Serial.begin(9600);

  int i;
  for(i=4;i<=7;i++)
    pinMode(i, OUTPUT);  
  
  pinMode(8, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);

  EMon(255,255);
}


void loop() {
  //Serial.println("still running");

  bool signal ;  
  signal = digitalRead(8);
  
  if(codeRecieved == 0 || (millis()-codeDuration < maxDuration)) { /* If new pin pressed */
    Serial.println("code renew");
    delay(10);
    if(signal == HIGH) {
      delay(250);
      if(readTone() == code[codeRecieved]) {
        if(codeRecieved == 0) {
          codeDuration = millis();
        }
        codeRecieved++;
        Serial.println(codeRecieved);
        Serial.println("code read");
      } 
    }
  } else {
        codeRecieved = 0;
        codeDuration = 0;
        Serial.println("not code");
  }
  
  if(codeRecieved == sizeof(code)/sizeof(int)) {
    Serial.println("EM release");
    EMoff();
    delay(10000);
    EMon(255,255);
  }
}

int readTone() {
  uint8_t number;  
  number = ( 0x00 | (digitalRead(13)<<0) | (digitalRead(12)<<1) | (digitalRead(11)<<2) | (digitalRead(10)<<3) );
    switch (number)
    {
      case 0x01:
      Serial.println("Pin Pressed : 1");
      break;
      case 0x02:
      Serial.println("Pin Pressed : 2");
      break;
      case 0x03:
      Serial.println("Pin Pressed : 3");
      break;
      case 0x04:
      Serial.println("Pin Pressed : 4");
      break;
      case 0x05:
      Serial.println("Pin Pressed : 5");
      break;
      case 0x06:
      Serial.println("Pin Pressed : 6");
      break;
      case 0x07:
      Serial.println("Pin Pressed : 7");
      break;
      case 0x08:
      Serial.println("Pin Pressed : 8");
      break;
      case 0x09:
      Serial.println("Pin Pressed : 9");
      break;
      case 0x0A:
      Serial.println("Pin Pressed : 0");
      break;
      case 0x0B:
      Serial.println("Pin Pressed : *");
      break;
      case 0x0C:
      Serial.println("Pin Pressed : #");
      break;    
    }
  return number;
}

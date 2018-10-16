/**
 * function: 
 *            1. I2c slave to sent PWM to motor control or turn on/off light with PWM
 *            2. Use EEPROM to rewrite its I2c address
 *            3. simple blink on command to check if it's alive
 * 
 * How to write Servo microseconds for channel 1
 *    1. send 0x02
 *    2. send value
 *    
 * How to write Servo microseconds for channel 2
 *    1. send 0x03
 *    2. send value
 *    
 * How to change address
 *    1. send 0xCE
 *    2. send new address, like 0x06
 *    
 * How to use check alive
 *    1. send 0x10
 *    2. send how many times to blink
 */

#include "TinyWireS.h"                  // wrapper class for I2C slave routines
#include <EEPROM.h>


#define ledPin 1
#define servoPinCh1 1
#define servoPinCh2 3



void setup() {
  // put your setup code here, to run once:
  byte I2C_SLAVE_ADDR = 0;
  I2C_SLAVE_ADDR = EEPROM.read(0);         //get its own slave address from EEPROM
  if(I2C_SLAVE_ADDR > 127 || I2C_SLAVE_ADDR < 1) {
    I2C_SLAVE_ADDR = 127;
  }
  TinyWireS.begin(I2C_SLAVE_ADDR);      // init I2C Slave mode
  Blink(2); 
  pinMode(ledPin, OUTPUT);
  pinMode(servoPinCh2, OUTPUT);
}

int servoMicrosCh1 = 0;
int servoMicrosCh2 = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if (TinyWireS.available()){           // got I2C input!
    //Blink(3);
    byte byteCommand = TinyWireS.receive();     //get command to turn motor
    
    if(byteCommand == 0x02) {            //command set value to write Servo microseconds
      servoMicrosCh1 = TinyWireS.receive() << 8;
      servoMicrosCh1 += TinyWireS.receive();
    
    } else if(byteCommand == 0x03) {
      servoMicrosCh2 = TinyWireS.receive() << 8;
      servoMicrosCh2 += TinyWireS.receive();
        
      
    } else if(byteCommand == 0xCE) {            //command to change address
      byte newAddress = TinyWireS.receive();    //new address value
      //Write value
      EEPROM.put(0, newAddress);                //change address value
      Blink(1000);    
      delay(1000);
      
    } else if(byteCommand == 0x10) {            //blink check to see if alive
      byte times = TinyWireS.receive();         //receive how many times to blink
      Blink(times);
    }
  }

  //write Servo microseconds
  digitalWrite(servoPinCh1, HIGH);
  digitalWrite(servoPinCh2, HIGH);
  if(servoMicrosCh1 > servoMicrosCh2) {
    delayMicroseconds(servoMicrosCh2);
    digitalWrite(servoPinCh2, LOW);
    delayMicroseconds(servoMicrosCh1 - servoMicrosCh2);
    digitalWrite(servoPinCh1, LOW);
  } else {
    delayMicroseconds(servoMicrosCh1);
    digitalWrite(servoPinCh1, LOW);
    delayMicroseconds(servoMicrosCh2 - servoMicrosCh1);
    digitalWrite(servoPinCh2, LOW);
  }
  delayMicroseconds(20000 - servoMicrosCh1 - servoMicrosCh2);
}

void Blink(byte times){ 
  for (byte i=0; i< times; i++){
    digitalWrite(ledPin,HIGH);
    delay (175);
    digitalWrite(ledPin,LOW);
    delay (175);
  }
}

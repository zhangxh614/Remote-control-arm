#include <Wire.h>
#include <Servo.h>

Servo s0,s1,s2,s3;//bottom,middle-down,middle-up,top,hand
int pos[4]={90,90,90,90};
int valuex=300,valuey=300,valuez=0;
bool state=false;

#define Register_ID 0  
#define Register_2D 0x2D
#define Register_X0 0x32
#define Register_X1 0x33
#define Register_Y0 0x34
#define Register_Y1 0x35
#define Register_Z0 0x36
#define Register_Z1 0x37

int ADXAddress = 0xA7 >> 1;  // the default 7-bit slave address（转化为7位地址）
int reading = 0;
int val=0;
int Z1,Z0,Z_out;
double Zg;

void setup() {
  // put your setup code here, to run once:
  pinMode(7, INPUT_PULLUP); //注意 Z 轴输入一定要上拉，不然电平不稳。
  Serial.begin(9600);
  delay(100);
  s3.attach(6);
  Wire.begin();
  Wire.beginTransmission(ADXAddress);
  Wire.write(Register_2D);
  Wire.write(8);                //measuring enable
  Wire.endTransmission();     // stop transmitting

}

void loop() {
  // put your main code here, to run repeatedly:
  valuex = analogRead(A0); 
  valuey = analogRead(A1);
  valuez = digitalRead(7);
 //Serial.print("X:"); 
 //Serial.println(valuex); 
  while(valuex>=512&&pos[0]+1<=170) {
    //Serial.println(pos[0]);
      if(!s0.attached()) {
       s0.attach(9);
      }
      s0.write(++pos[0]);
      delay(15);
      valuex = analogRead(A0); 
      valuey = analogRead(A1); 
      valuez = digitalRead(7);
  }
  while(valuex<=128&&pos[0]+1>=10) {
    //Serial.println(pos[0]);
      if(!s0.attached()) {
        s0.attach(9);
      }
      s0.write(--pos[0]);
      delay(15);
      valuex = analogRead(A0); 
      valuey = analogRead(A1); 
      valuez = digitalRead(7);
  }
  if(s0.attached()) {
    s0.detach();
  }
  while(valuey>=512&&pos[1]-1>=10) {
    if(!s1.attached()) {
        s1.attach(10);
      }
      s1.write(--pos[1]);
      delay(15);
      valuex = analogRead(A0); 
      valuey = analogRead(A1); 
      valuez = digitalRead(7);
   }
  while(valuey<=128&&pos[1]+1<=170) {
    if(!s1.attached()) {
        s1.attach(10);
      }
      s1.write(++ vpos[1]);
      delay(15);
      valuex = analogRead(A0); 
      valuey = analogRead(A1); 
      valuez = digitalRead(7);
  }
  if(s1.attached()) {
        s1.detach();
      }
  //Serial.println(valuez);
  //Serial.print('zzzz');
  //Serial.println(state);
  //Serial.println(pos[3]);
  if(valuez==0) {
      if(state==false) {
        state=(!state);
        for(pos[3];pos[3]<=110;pos[3]++) {
          s3.write(pos[3]);
          delay(15);
        }
      }
      else {
        state=(!state);
        for(pos[3];pos[3]>=90;pos[3]--) {
          s3.write(pos[3]);
          delay(15);
        }
      }
      delay(500);
  }

  //------------------Z
  Wire.beginTransmission(ADXAddress); // transmit to device
  Wire.write(Register_Z0);
  Wire.write(Register_Z1);
  Wire.endTransmission();
  Wire.requestFrom(ADXAddress,2); 
  if(Wire.available()<=2)   
  {
    Z0 = Wire.read();
    Z1 = Wire.read(); 
    Z1=Z1<<8;
    Z_out=Z0+Z1;
  }
  
  Zg=Z_out/256.0;
  //Serial.println(Zg);
  while(Zg<=-0.6&&pos[2]-1>=10) {
    if(!s2.attached()) {
        s2.attach(11);
      }
    s2.write(--pos[2]);
    delay(15);
    Wire.beginTransmission(ADXAddress); // transmit to device
    Wire.write(Register_Z0);
    Wire.write(Register_Z1);
    Wire.endTransmission();
    Wire.requestFrom(ADXAddress,2); 
    if(Wire.available()<=2)   
    {
      Z0 = Wire.read();
      Z1 = Wire.read(); 
      Z1=Z1<<8;
      Z_out=Z0+Z1;
    }
  
    Zg=Z_out/256.0;
    Serial.println(Zg);
  }
  while(Zg>=0.5&&pos[2]+1<=170) {
    if(!s2.attached()) {
        s2.attach(11);
      }
    s2.write(++pos[2]);
    delay(15);
    Wire.beginTransmission(ADXAddress); // transmit to device
    Wire.write(Register_Z0);
    Wire.write(Register_Z1);
    Wire.endTransmission();
    Wire.requestFrom(ADXAddress,2); 
    if(Wire.available()<=2)   
    {
      Z0 = Wire.read();
      Z1 = Wire.read(); 
      Z1=Z1<<8;
      Z_out=Z0+Z1;
    }
  
    Zg=Z_out/256.0;
    Serial.println(Zg);
  }
  if(s2.attached()) {
        s2.detach();
      }
}

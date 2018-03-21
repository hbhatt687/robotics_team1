/*Team1 Lab1*/

/*motor.ino includes*/
#include <DualVNH5019MotorShield.h>

/*mpu6050.ino includes*/
#include<Wire.h>

/*sonar.ino global variables*/

// defines pins numbers
const int trigPin = 47;
const int echoPin = 49;
// defines variables
long duration;
int distance;

/*motor.ino global variables*/
DualVNH5019MotorShield md;
int limit=60; 

/*mpu6050.ino global variables*/
const int MPU_addr=0x68;  // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void stopIfFault()
//motor function
{
  if (md.getM1Fault())
  {
    Serial.println("M1 fault");
    while(1);
  }
  if (md.getM2Fault())
  {
    Serial.println("M2 fault");
    while(1);
  }
}

void setup() {
//sonar setup
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
Serial.begin(9600); // Starts the serial communication

//motor setup
Serial.begin(115200);
Serial.println("Dual VNH5019 Motor Shield");
md.init();

//mpu6050 setup
Wire.begin();
Wire.beginTransmission(MPU_addr);
Wire.write(0x6B);  // PWR_MGMT_1 register
Wire.write(0);     // set to zero (wakes up the MPU-6050)
Wire.endTransmission(true);
Serial.begin(9600);
}

void loop() {
  //sonar loop
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  delay(1000);

//motor loop
  for (int i = 0; i <= limit; i++)
    {
      md.setM1Speed(i);
      stopIfFault();
      
      md.setM2Speed(-i);
      stopIfFault();
      
      if (i%200 == 100)
      {
        Serial.print("M1 current: ");
        Serial.println(md.getM1CurrentMilliamps());
        Serial.print("M2 current: ");
        Serial.println(md.getM2CurrentMilliamps());
      }
      delay(5);
    }
  
  delay(200);
   
  for (int i = limit; i >= -limit; i--)
  {
    md.setM1Speed(i);
    stopIfFault();
    
    md.setM2Speed(-i);
    stopIfFault();
     
    if (i%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
      Serial.print("M2 current: ");
      Serial.println(md.getM2CurrentMilliamps());
    }
    delay(5);
  }
  
  delay(200);
  
  for (int i = -limit; i <= 0; i++)
  {
    md.setM1Speed(i);
    stopIfFault();
    
    md.setM2Speed(-i);
    stopIfFault();
    
    if (i%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
      Serial.print("M2 current: ");
      Serial.println(md.getM2CurrentMilliamps());
    }
    delay(5);
  }
  exit(0);
  
  //mpu6050 loop
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
  AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcZ);
  Serial.print(" | Tmp = "); Serial.print(Tmp/340.00+36.53);  //equation for temperature in degrees C from datasheet
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);
  delay(333);
}

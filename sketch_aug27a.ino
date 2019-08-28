//imports
#include <Wire.h> //for I2C, needed for shields and most breakouts
#include <SparkFun_MMA8452Q.h> 

//variables go here
float temp;
MMA8452Q accel;

// tri-color LED info
// http://wiring.org.co/learning/basics/rgbled.html

//startup goes here
void setup() {
  Serial.begin(9600);
  while (!Serial); //do nothing till serial is avalible
  Serial.println("Initializing...");

  accel.init(SCALE_8G, ODR_6);
  //fastest readings of 800hz of values between -8g and 8g

  pinMode(A0,INPUT);
  /*  the small 3 pin semi circle shaped thing is temperature sensor,
  connect middle pin to A0, flat side of sensor is "front",
  left is 5v, right is ground (GND) */  

  Serial.println("Finished startup!");
}

//running goes here
void loop() {
  if (accel.available()) {
    accel.read();
    /*  accel reads x, y, and z as signed 12 bit ints
    accel auto calculates floats of changed from previous
    as cx, cy, and cz      */
    printAccels();
  }
  temp = analogRead(A0);
  Serial.print("Temperature is ... ");
  Serial.println(temp, 3); //3 decimal places
}

void printAccels()
{
  Serial.print("x: ");
  Serial.print(accel.x, 3);
  Serial.print("\t y: ");
  Serial.print(accel.y, 3);
  Serial.print("\t z: ");
  Serial.println(accel.z, 3);
}

#include <Servo.h>
//#include <math.h>

//for 50K thermistor
int ThermistorPin = 0; //analog pin
int Vo;
float R1 = 10000; /*You only need to know the magnitude of your thermistor. For example, if your thermistor resistance is 34,000 Ohms, it is a 10K thermistor. 
                   If it’s 340,000 Ohms, it’s a 100K thermsitor. */
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07; //constants for resitance to temperature conversion

const int fanPin = 3;

Servo lifter;
Servo turner;

void setup() {
  Serial.begin(9600);
  lifter.attach(8);
  turner.attach(9);
  lifter.write(90); // spoon is almost at 180 degrees
  turner.write(0); // turner faces spoon toward bowl
  pinMode(fanPin, OUTPUT);
  digitalWrite(fanPin, LOW);
}

void loop() {
  //lower arm into bowl
  lifter.write(90);
  delay(1000);
  lifter.write(45);
  delay(1000);

  //calculate resistance of thermistor
  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  //translate resistance to temperature.
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15; //kelvins to celsius
  T = (T * 9.0)/ 5.0 + 32.0; //celsius to fahrenheit

  Serial.print("Temperature: "); 
  Serial.print(T);
  Serial.println(" F"); 

  //slowly lift arm
  for(int n=25; n <= 90; n+=5){
    lifter.write(n);
    delay(300);
  }


  turner.write(10);
  delay(2000);
  
  //if food is hot turn fan on
  if(T > 78){
    digitalWrite(fanPin,HIGH);
    delay(4000);
    digitalWrite(fanPin,LOW);
  }

  //turn spoon towards user
  for(int n=10; n <= 70; n+=5){
    turner.write(n);
    delay(300);
  }

  //reset position
  delay(3000);
  turner.write(0);
  delay(1000);
}

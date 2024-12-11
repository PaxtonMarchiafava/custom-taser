#include <Wire.h>

const int i2cAddress = 10;

 // pins
const int LEDpin = 13;
const int SwitchPin = A2;
const int ReadPin = A6;

 // boost parameters
const int R1 = 1000000; // measure these before soldering, I used nominal and number was off by a full volt
const int R2 = 33000;
const float maxVoltage = (5 * (R1 + R2)) / R2; // max voltage of boost converter based on the voltage divider resistors, and the analog read pin to be @ 5V // 156.51
const float maxAnalogReadValue = 1023; // 2 ^ 10 because it's an arduino nano

int TotalTime = 10000; // us
int ChargeTime = 0;
int Target = 10; // in volts

void setup() {
  pinMode(LEDpin, OUTPUT);
  pinMode(SwitchPin, OUTPUT);
  pinMode(ReadPin, INPUT);
  Serial.begin(115200);

  Wire.begin(i2cAddress);
  Wire.onReceive(receiveEvent);

  digitalWrite(SwitchPin, HIGH);
  delay(50);
}


void loop () {
  
  if (ChargeTime > 0) { // don't even turn on if there is no time to do so
    digitalWrite(SwitchPin, HIGH);
  }
  delayMicroseconds(ChargeTime);
  
  
  digitalWrite(SwitchPin, LOW);
  delayMicroseconds(TotalTime - ChargeTime);

  Serial.print("Current Voltage: ");
  Serial.println(getVoltage());

  if (Target > getVoltage()) { //  if current voltage is above what we want
    if (ChargeTime > (TotalTime * 0.02) * -1) {
      ChargeTime -= (TotalTime * 0.02);
    }
    
    digitalWrite(LEDpin, LOW);
  
  } else if (Target < getVoltage()) {
    ChargeTime += (TotalTime * 0.02);
    digitalWrite(LEDpin, LOW);
  
  } else {
    digitalWrite(LEDpin, HIGH);
  }
}

float getVoltage () { // get current read voltage of output
  return (analogRead(ReadPin) * (maxVoltage / maxAnalogReadValue));
}

void receiveEvent() { // receiver for i2c data
  
  Target = Wire.read();

}

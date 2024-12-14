#include <Wire.h>

const int i2cAddress = 10;

 // pins
const int LEDpin = 13;
const int SwitchPin = A2;
const int ReadPin = A6;

// boost parameters
const int MOSFET_VDS = 55; // drain to source breakdown voltage to keep from burning FETs IRLZ44N:55V
const int maxHardwareVoltage = MOSFET_VDS * 0.85; // the absolute max voltage before components break
const long R1 = 1000000; // measure these before soldering, I used nominal and number was off by a full volt
const long R2 = 33330;
const float maxVoltage = float ((5 * (R1 + R2)) / R2); // max voltage of boost converter based on the voltage divider resistors, and the analog read pin to be @ 5V // 156.51
const float maxAnalogReadValue = 1023; // 2 ^ 10 because it's an arduino nano

int TotalTime = 10000; // us
int ChargeTime = 0;
int Target = 20; // in volts
String text = "";


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

  while (Serial.available()) { // if number comes through serial
    text += char (Serial.read());
    delayMicroseconds(86); // 87 because 115200 baud rate
  }
  if (text != "") {
    Target = text.toInt();
    text = "" ;
  }

  
  if (Target > maxHardwareVoltage) { // cap max value
    Target = maxHardwareVoltage;
  }
  

  // do the boosting
  if (ChargeTime > 0 && getVoltage() < maxVoltage) { // don't even turn on if there is no time to do so or if voltage too high
    digitalWrite(SwitchPin, HIGH);
  }
  delayMicroseconds(ChargeTime);

  digitalWrite(SwitchPin, LOW);
  delayMicroseconds(TotalTime - ChargeTime);


  // debug values
  Serial.print(" Target: ");
  Serial.print(Target);
  Serial.print(" Current Voltage: ");
  Serial.print(getVoltage());
  Serial.print(" Current ChargeTime: ");
  Serial.println(ChargeTime);


  // adjust boost timer
  if (getVoltage() < Target) { //  if current voltage is below what we want
    if (ChargeTime <= TotalTime * 0.9) {
      ChargeTime += (TotalTime * 0.02);
    }
    digitalWrite(LEDpin, LOW);

  } else if (Target < getVoltage()) { // if above
    if (ChargeTime > 0) {
      ChargeTime -= (TotalTime * 0.02);
    }
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

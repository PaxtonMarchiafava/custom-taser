
#include <Wire.h>

#define SerialBaudRate 115200
 // shift register
const int OE = 9;
const int STCP = 10;
const int SHCP = 11;
const int dataPin = 8;
byte shiftData [] = {0, 0}; // bytes to push into shift registers

const byte P1 [] [2] = {{1, B00000010}, {1, B00001000}, {1, B00100000}, {1, B10000000}}; // Register, pin
const byte P2 [] [2] = {{1, B00000001}, {1, B00000100}, {1, B00010000}, {1, B01000000}};
const byte N1 [] [2] = {{0, B00000001}, {0, B00000100}, {0, B00010000}, {0, B01000000}};
const byte N2 [] [2] = {{0, B00000010}, {0, B00001000}, {0, B00100000}, {0, B10000000}};


boolean Shock_Player [] = {false, false, false, false}; // used to control who gets shocked
String command = ""; // data from computer


void setup() {

  Wire.begin(); // join i2c bus (address optional for master)

  pinMode(STCP, OUTPUT);
  pinMode(SHCP, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(OE, OUTPUT);
  digitalWrite(OE, LOW); // enable outputs
  // digitalWrite(OE, HIGH); // enable outputs

	Serial.begin(SerialBaudRate);
  // shiftData[0] |= N2[0][1];
  // shiftData[0] |= N2[3][1];
  // Serial.println(shiftData[0]);
  // pushUpdate();
  reset(); // just make sure
}

void loop() {
    
  while (Serial.available()) {
    command += char (Serial.read());
    delayMicroseconds(86); // 87 because 115200 baud rate
  }
  if (command != "") {
    
    
    
    /* Figure out who to shock here based on serial data

    if (command == "1") {
      Shock_Player[0] = true;
    }
    Serial.println(command);
    
    */
    
    
    
    command = "";
  }


  if (Shock_Player[0] || Shock_Player[1] || Shock_Player[2] || Shock_Player[3]) { // if a player is guilty, shock
    Shock(0.25, 6000); // low freq to see on multimeter. change to 40
  }
  

}

// do the shocking
void Shock (double freq, double time) { // hz, ms
  Serial.println("shocking");

  double cycles = freq * (time / 1000); // find how many times to fluctuate shock

  for (int i = 0; i < cycles; i++) {
    reset(); // reset so you dont short
    for (int j = 0; j < 4; j++) { // for all players
      if(Shock_Player[j] == true) { // if deserves it

        shiftData[N1[j][0]] |= N1[j][1];
        shiftData[P2[j][0]] |= P2[j][1];

        pushUpdate();
      
      }
    }
    delay(((1/freq) / 2) * 1000);

    reset();
    for (int j = 0; j < 4; j++) { // for all players
      if(Shock_Player[j] == true) { // if deserves it
          
        shiftData[N2[j][0]] |= N2[j][1];
        shiftData[P1[j][0]] |= P1[j][1];
        pushUpdate();
      
      }
    }
    delay(((1/freq) / 2) * 1000);
    reset();


  }

  for (int j = 0; j < 4; j++) { // reset all values
    Shock_Player[j] = false;
  }

  reset();
  Serial.println("done");

}

void reset () { // turn everything off

  shiftData[0] = 0;
  shiftData[1] = 0;

  pushUpdate();
}

void pushUpdate () {

  digitalWrite(STCP, LOW);
  shiftOut(dataPin, SHCP, MSBFIRST, shiftData[1]);
  shiftOut(dataPin, SHCP, MSBFIRST, shiftData[0]);
  digitalWrite(STCP, HIGH);

  delayMicroseconds(1); // delay to let update (it does not take anywhere close to 1us but im too lazy to figure out how long it actually takes)
}

void ChangeShockVoltage (byte device, byte NewVoltage) { // device 10
  Wire.beginTransmission(device);
  Wire.write(NewVoltage);
  Wire.endTransmission();
}


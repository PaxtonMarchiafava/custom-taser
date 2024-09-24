const int LEDpin = 13;
const int SwitchPin = A5;
const int ReadPin = A6;

int TotalTime = 100;
int ChargeTime = 0;


const int Target = 200;
/*
|Analog reading | Voltage |
|---------------|---------|
|       0       |    0    |
|      135      |         |
|      150      |   24    |
|      200      |   30    |
|      250      |   37    |
|      300      |         |
|      350      |         |
*/

void setup() {
	pinMode(LEDpin, OUTPUT);
	pinMode(SwitchPin, OUTPUT);
	pinMode(ReadPin, INPUT);
	Serial.begin(115200);

    digitalWrite(SwitchPin, HIGH);
    delay(50);
}

void loop () {

    Serial.print(analogRead(ReadPin));
    Serial.print(", ");
    Serial.print(ChargeTime);
    Serial.print(", ");
    Serial.println(TotalTime);

    //delay(1);

    digitalWrite(SwitchPin, HIGH);
    delayMicroseconds(ChargeTime);

    digitalWrite(SwitchPin, LOW);
    delayMicroseconds(TotalTime - ChargeTime);

    if (analogRead(ReadPin) > Target && ChargeTime > 1) { //  if current voltage is above what we want
        ChargeTime--;
    } else if (analogRead(ReadPin) < Target && ChargeTime < TotalTime - (TotalTime / 20)) {
        ChargeTime++;
    }

    if (analogRead(ReadPin) > Target - 4 && analogRead(ReadPin) < Target + 4) { // if in acceptable range, light on
        digitalWrite(LEDpin, HIGH);
    } else {
        digitalWrite(LEDpin, LOW);
    }
}


const int P1 [] = {A3, 2, 6, 10};
const int P2 [] = {A2, 3, 7, 11};
const int N1 [] = {A4, A0, 5, 9};
const int N2 [] = {A1, 4, 8, 12};
boolean Shock_Player [] = {false, false, false, false}; // used to control who gets shocked
String command = ""; // data from computer


void setup() {

    for (int i = 0; i < 4; i++) { // for all players
	    pinMode(P1[i], OUTPUT); // init output
        digitalWrite(P1[i], LOW); // dont shock people premature
	    pinMode(P2[i], OUTPUT);
        digitalWrite(P2[i], LOW);
	    pinMode(N1[i], OUTPUT);
        digitalWrite(N1[i], LOW);
	    pinMode(N2[i], OUTPUT);
        digitalWrite(N2[i], LOW);
    }

	Serial.begin(115200);
}

void loop() {

	while (Serial.available()) { // read data through usb port from python code
		command += (char) Serial.read();
	}

    delay(1);
    while (Serial.available()) { // read data through usb port from python code
		command += (char) Serial.read();
	}

    

    if (command != 0) {
        Serial.print(command);

        if (command == "7") { Shock_Player [1] = true; }
        
        /*
        if (command.indexOf("#") != -1) { // If the player was not the winner, shock
            Shock_Player [1] = true;
        }
        if (command.indexOf("#") == -1) {
            Shock_Player [0] = true;
        }
        if (command.indexOf("#") == -1) {
            Shock_Player [2] = true;
        }
        if (command.indexOf("#") == -1) {
            Shock_Player [3] = true;
        }
        */

        command = "";
        Shock(40, 2000);
    }
}

void Shock (double freq, double time) {
    double cycles = freq * (time / 1000); // find how many times to fluctuate shock

    for (int i = 0; i < cycles; i++) {
        reset(); // reset so you dont short
        for (int j = 0; j < 4; j++) { // for all players
            if(Shock_Player[j] == true) { // if deserves it
                digitalWrite(N1[j], HIGH); // first half of shock
                digitalWrite(P2[j], HIGH);
            }
        }
        delay(((1/freq) / 2) * 1000);

        reset();
        for (int j = 0; j < 4; j++) { // for all players
            if(Shock_Player[j] == true) { // if deserves it
                digitalWrite(N2[j], HIGH); // second half of shock
                digitalWrite(P1[j], HIGH);
            }
        }
        delay(((1/freq) / 2) * 1000);
        reset();


    }
    for (int j = 0; j < 4; j++) { // reset all values
        Shock_Player[j] = false;
    }

    reset();
    delay(1);

}

void reset () { // turn everything off
    delay(1);
    for (int j = 0; j < 4; j++) {
        digitalWrite(P1[j], LOW);
        digitalWrite(P2[j], LOW);
        digitalWrite(N1[j], LOW);
        digitalWrite(N2[j], LOW);
    }
    delay(1); // delay to let update (it does not take anywhere close to 1ms but im too lazy to figure out how long it actually takes)
}
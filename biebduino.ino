/*
*   Biebduino
*   Graham Ballantyne <hello@grahamballantyne.com>
*   License: WTFPL v2 (see LICENSE)
*/

const String version = "1.0.0";
const boolean off = LOW;
const boolean on = HIGH;

// PIN ASSIGNMENTS
// Eyes -- PWM pins
const int eyeRed = 3;
const int eyeGreen = 5;
const int eyeBlue = 6;
const int eyePins[] = {3, 5, 6};


// Sonar
const int sonar = A1;
int distance;
const int angerThreshold = 24;
int getDistance() {
    return analogRead(sonar) / 2;
}

// LCD
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
void lcdClearLine(int line) {
    lcd.setCursor(0, line);
    lcd.print("                ");
    lcd.setCursor(0,line);
}

// EYE LEDs
const boolean red[] = {on, off, off};
const boolean green[] = {off, on, off};
const boolean blue[] = {off, off, on};
const boolean yellow[] = {on, on, off};
const boolean cyan[] = {off, on, on};
const boolean magenta[] = {on, off, on};
const boolean white[] = {on, on, on};
const boolean black[] = {off, off, off};
const boolean* colours[] = {red, green, blue, yellow, cyan, magenta, white, black};
const String coloursStr[] = {"red", "green", "blue", "yellow", "cyan", "magenta", "white", "black"};
void setEyeColour(const boolean* colour) {
  digitalWrite(3, colour[0]);
  digitalWrite(5, colour[1]);
  digitalWrite(6, colour[2]);

//  for(int i = 0; i < 3; ++i) {
//    digitalWrite(eyePins[i], colour[i]);
//  }
}

// TEST ROUTINE
void post() {
    // runs during setup() and before loop() starts
    // tests the lcd screen, rgb leds and sonar

    String out;
    out = "Biebduino " + version;
    Serial.println(out);
    lcd.setCursor(0,0);
    lcd.print(out);
    out = "Starting tests";
    Serial.println(out);
    lcd.setCursor(0,1);
    lcd.print(out);

    delay(2000);

    // run through the eye colours
    lcd.clear();
    out = "Testing eye LEDs";
    Serial.println(out);
    lcd.setCursor(0,0);
    lcd.print(out);
    lcdClearLine(1);
    for(int i = 0; i < 8; ++i) {
        out = coloursStr[i];
        Serial.println(out);
  	    lcd.print(out);
        setEyeColour(colours[i]);
    	delay(500);
    	lcdClearLine(1);
    }

    // check the sonar
    lcd.clear();
    out = "Maxsonar EZ1";
    Serial.println(out);
    lcd.print(out);
    lcd.setCursor(0,1);

    for(int i=0; i < 20; i++) {
        distance = getDistance();
        lcdClearLine(1);
        out = "Distance: " + String(distance);
        Serial.println(out);
        lcd.print(out);
        delay(500);
    }

    lcd.clear();
    lcd.print("Tests complete");

}


// MAIN
void setup() {
    Serial.begin(9600);

    // pin setup
    for (int i=0; i < 3; i++) {
      pinMode(eyePins[i], OUTPUT);
    }

    // LCD
    lcd.begin(16, 2);

    // run the POST routune
    post();
}

void loop() {
}
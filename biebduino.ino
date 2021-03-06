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
const int sonar = A0;
int distance;
const int angerThreshold = 24;
int getDistance() {
    int sum=0;
    int max = 20;
    for (int i=0; i < max; i++) {
        sum += analogRead(sonar);
        delay(10);
    }
    return (sum/max) / 2;
}

// LCD
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
void lcdClearLine(int line) {
    lcd.setCursor(0, line);
    lcd.print("                ");
    lcd.setCursor(0,line);
}
void lcdPrintMood(String moodString) {
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Bieb is");
    lcd.setCursor((16-moodString.length())/2, 1);
    lcd.print(moodString);
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
 for(int i = 0; i < 3; ++i) {
   digitalWrite(eyePins[i], colour[i]);
 }
}

// MOODS
unsigned long nextMoodChangeTime = 0;
unsigned long minMoodInterval = (10000);
unsigned long maxMoodInterval = (90001); //3600000;
unsigned int currentMood = 666;
unsigned int lastAngerLevel = 666;
unsigned int maxAngerThresholdDistance = 60;
const String moods[] = {"angry", "horny", "sad", "mellow", "calm", "blissful", "spooked", "sleepy"};

void moodSwing() {
    lastAngerLevel = 666;
    Serial.print("moodswing @ ");
    Serial.println(millis());
    int nextMood = random(0,8);
    Serial.println(nextMood);
    if (nextMood == currentMood || nextMood == 0) {
        Serial.println("Oops, picked mood " + moods[nextMood]);
        nextMoodChangeTime = millis() + 500;
        Serial.print("next mood swing in 500 millis @ "); Serial.println(nextMoodChangeTime);
        return;
    }

    String moodString = moods[nextMood];

    setEyeColour(colours[nextMood]);

    lcdPrintMood(moodString);
    Serial.println(moodString);

    currentMood = nextMood;

    // pick the next mood swing time
    // maximum of 3600000ms (one hour)
    unsigned long interval = random(minMoodInterval, maxMoodInterval);
    nextMoodChangeTime = millis() + interval;
    Serial.print("next mood swing in "); Serial.print(interval); Serial.print(" millis @ "); Serial.println(nextMoodChangeTime);
}

void soAngry(int distance) {
    Serial.println("*****GETTING ANGRY*****");
    Serial.println("Distance: " + String(distance));
    String angerLevelsStr[] = {/*"ANNOYED",*/ "GETTING MAD", "SO ANGRY", "GONNA EAT YOU"};
    int pwmLevels[] = {/*2, */ 20, 60, 255};
    int angerLevel = map(distance, 5, maxAngerThresholdDistance+1, 2, -1);
    Serial.println("Anger Level: " + angerLevelsStr[angerLevel] + " " +String(angerLevel));
    currentMood = 0;
    Serial.println("Last Anger Level: " + String(lastAngerLevel));
    if (angerLevel == lastAngerLevel) {
        Serial.println("unchanged anger");
        return;
    }
    Serial.println("Set Eyes To PWM " + pwmLevels[angerLevel]);
    setEyeColour(colours[7]);
    analogWrite(eyeRed, pwmLevels[angerLevel]);
    lastAngerLevel = angerLevel;
    lcdPrintMood(angerLevelsStr[angerLevel]);
    Serial.println(angerLevelsStr[angerLevel]);
    nextMoodChangeTime = millis() + 6000;
    delay(3000);
}


// POST ROUTINE
void post() {
    // runs during setup() and before loop() starts
    // tests the lcd screen, rgb leds and sonar

    String out;
    out = "Biebduino " + version;
    Serial.println(out);
    lcd.setCursor(0,0);
    lcd.print(out);
    out = "Starting POST";
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

    for(int i=0; i < 10; i++) {
        distance = getDistance();
        lcdClearLine(1);
        out = "Distance: " + String(distance);
        Serial.println(out);
        lcd.print(out);
        delay(500);
    }

    lcd.clear();
    lcd.print("POST Complete");
}

// MAIN
void setup() {
    Serial.begin(9600);
    randomSeed(analogRead(1));

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
    unsigned long now = millis();
    unsigned int distance = getDistance();
    if (distance <= maxAngerThresholdDistance) {
      soAngry(distance);
    } else if ((currentMood == 0 && distance > maxAngerThresholdDistance && now >= nextMoodChangeTime) || (now >= nextMoodChangeTime)) {
        moodSwing();
    }
}

/*
  This projects is a theremin created with two ultrasonic sensors, one controls the frequency (tone) and the other controls the amplitude (volume).
  This project is a fork of two projects:
    - https://create.arduino.cc/projecthub/opus9/theremino-f72d32 (this project had library conflicts)
    - https://create.arduino.cc/projecthub/jvmccall/ultrasonic-theremin-033d6f (this project had typo errors in the code and a mistake in the schema)
  Resource on controlling similar potentiometer using SPI protocol, provides example code and electrical setup: http://www.learningaboutelectronics.com/Articles/MCP4131-digital-potentiometer-circuit.php
*/

/*
  Potentiometer conecctions:
  1 - Arduino pin ~9
  2 - Speaker +
  3 - GND
  4 - 5V
  5 - Arduino pin ~10
  6 - Arduino pin 13
  7 - Arduino pin ~11
  8 - GND
  More info https://app.ultralibrarian.com/details/23D04BE7-10A0-11E9-AB3A-0A3560A4CCCC/Microchip/MCP4161-103E-P
*/

// NewTone is used to avoid the timer incompatibiliy of Tone with NewPing. Download https://bitbucket.org/teckel12/arduino-new-tone/downloads/ and import .zip
#include <NewTone.h>
#include <NewPing.h>
#include <SPI.h>

#define MIN_DISTANCE 35

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

// NewPing sonar(5, 6, 35); // the first and the second number are the pins of the sensor of volume, the third is the maximum distance
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// For SPI
byte address = 0x00;
int CS = 10;

int echo = 3;
int trigger = 4;

int distance;
int PotValue;

unsigned long TimeResponse;
float distance2;
float tone1;

int last_distance = 0;

void setup() {
  // set all the pins
  Serial.begin(9600);
  /*
    pinMode(trigger, OUTPUT); // tone sensor
    pinMode(echo, INPUT); // tone sensor
    pinMode (CS, OUTPUT);
    SPI.begin();

    // this has the speaker emit a start up sound from Low to High to Low
    digitalPotWrite(255);

    for (int i = 50; i <= 400; i++)
    {
    pinMode(9, OUTPUT);
    NewTone(9,i);
    delay(10);
    }

    delay(500);

    for (int i = 400; i >= 50; i--)
    {
    pinMode(9, OUTPUT);
    NewTone(9,i);
    delay(10);
    }
  */
}

void loop() {
  /*
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);   // creates a 10 microsecond pulse
    digitalWrite(trigger, LOW);
    TimeResponse = pulseIn(echo, HIGH);  // records how long it took to receive echo of the pulse
    distance2 = TimeResponse/58;  // calculates distance in centimeters

    if (distance2 < MIN_DISTANCE) {
    // conversion of distance into a value for a sound
    tone1 = 50.0*pow(2,(distance2/12.0));  // calculate frequency, range of about 50-360 Hz
    pinMode(9, OUTPUT);
    NewTone(9,tone1);
    } else {
    pinMode(9, OUTPUT); // this sets the sound to a high pitched noise when no distance is detected
    NewTone(9,0);
    }
  */

  distance = sonar.ping_cm(); // uses the library NewPing to calculate distance
  if ( distance != last_distance )  {
    Serial.println(distance);
    last_distance=distance;
  }
  /*
    // distance is converted to Potentiometer relevant values
    int PotValue = map(distance, 0, 35, 240, 255); // only use a range of resistance which is very low otherwise the speaker is too quiet because it is only a 8 ohm speaker

    if (distance == 0) // sets volume to max when no hand is detected or if hand is too far away
    {
    PotValue = 255;
    }

    digitalPotWrite(PotValue);
    delay(10);
  */
}

int digitalPotWrite(int value) {
  digitalWrite(CS, LOW); // this uses SPI protocol to communicate with the potentiometer and sets the resistance
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS, HIGH);
}

/******************************************
 * Depending on the distance, a certain note
 * is played
 *
 ******************************************/
#include <Tone.h>
#include <NewPing.h>
#include "pitches.h"

// Sensor
#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
unsigned int current_note=0;
Tone tone1;

void setup() {
  Serial.begin(9600);
  tone1.begin(13);
}

void loop() {
  delay(500);  // Wait 500ms between pings (about 2 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  float dCm = uS / US_ROUNDTRIP_CM; // Convert ping time to distance 

  if ( dCm > 100 ) {
    current_note=NOTE_E5;
  } else {
    current_note=0;
  }
  Serial.print("Ping: ");
  Serial.print(dCm);
  Serial.print("Note: ");
  Serial.print(current_note);
  Serial.println("");
  
  if ( current_note>0 ) {
    Serial.print("Playing note : ");
    Serial.println(current_note);
    tone1.play(current_note);
  } else {
    Serial.println("Silent note");
    tone1.stop();
  }
}
/* 
// notes in the melody:
int melody[] = {
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6};
int duration = 500;  // 500 miliseconds
 
void setup() {
 
}
 
void loop() {  
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    // pin8 output the voice, every scale is 0.5 sencond
    tone(12, melody[thisNote], duration);
     
    // Output the voice after several minutes
    delay(1000);
  }
   
  // restart after two seconds 
  delay(2000);
}
*/

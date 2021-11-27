#include <SoftwareSerial.h>

SoftwareSerial BT(10,11);
String state;

void setup() {
  BT.begin(9600);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  while (BT.available() ) {
    delay(10);
    char c=BT.read();
    state += c;
  }

  if ( state.length()>0 ) {
    Serial.println(state);
    if ( state=="On" || state=="switch on" ) {
      digitalWrite(2, HIGH);
    } else if ( state=="Off" || state=="switch off" ) {
      digitalWrite(2, LOW);
    }
    state="";
  }

}

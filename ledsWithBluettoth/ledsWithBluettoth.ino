/**
 * Control the matrix of LEDS from the Mobile
 */

#include "utils.h"
#include "leds.h"
// #include "bluetooth.h"
#include "tales.h"

// #define __DEBUG__

void setup() {
  Serial.begin(9600);

  mx.begin();
  mx.clear();

  #ifdef __BLUETOOTH__
  BT.begin(9600);
  #endif
  
  #ifdef __TALES__
  START_TIME = millis();
  // Total of tales
  char** tale = MY_TALES;
  TOT_TALES = 0;
  while ( **tale ) {
    ++TOT_TALES;
    ++tale;
  }
  #endif
}

void loop() {
  #ifdef __BLUETOOTH__
  while (BT.available() ) {
    delay(10);
    char c = BT.read();
    state += c;
  }

  if ( state.length() > 0 ) {
    processMessage(state);
    state = "";
  } else {
  #endif
    #ifdef __TALES__
    // We have waited enough, let's render a new line of the tale
    // or pick a tale if we are not rendering one
    if ( (millis() - START_TIME ) > WAIT_MILIS ) {
      if ( !RENDERING_TALE || IND_LINE_CURR_TALE == getNumLines(MY_TALES[IND_CURR_TALE])) {
        RENDERING_TALE = true;
        IND_CURR_TALE = getRandom(TOT_TALES);
        IND_LINE_CURR_TALE = 0;
        // PRINTLN(">>> Pick a new tale : ", IND_CURR_TALE);
      }

      // char* line=getLine(MY_TALES[IND_CURR_TALE], IND_LINE_CURR_TALE);
      String line=getLine(MY_TALES[IND_CURR_TALE], IND_LINE_CURR_TALE);
      PRINT("Show the line : [", line);
      PRINTLN("]", "");
      
      processMessage(line);
      ++IND_LINE_CURR_TALE ;

      START_TIME = millis();
    }
    #endif
  #ifdef __BLUETOOTH__  
  }
  #endif
}

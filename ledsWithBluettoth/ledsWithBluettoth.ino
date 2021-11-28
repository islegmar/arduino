/*
   Control the matrix of LEDS from the Mobile
*/

#include <MD_MAX72xx.h >
#include <SoftwareSerial.h>

#define __DEBUG__

// OK, in te future all those strings will be in an external file so they do not take place but in the meantime....
#define MY_TALE1 \
  "- Te devorare -dijo la Pantera\0" \
  "- Peor para ti -dijo la\0" \
  "D;F#Espada\0"\
  "\0"

#define MY_TALE2 \
  "La mujer que ame se ha convertido en fantasma\0" \
  "Yo soy el lugar de las apariciones\0" \
  "\0"

#define MY_TALE3 \
  "No olvide usted, senyora, las noches en que nuestras almas lucharon\0" \
  "D;F#cuerpo\0"\
  "D;F#a\0"\
  "D;F#cuerpo\0"\
  "\0"

#define MY_TALE4 \
  "Traedeme el caballo mas veloz\0" \
  "pidio el hombre mas honrado\0"\
  "Acabo de decirle la verdad al rey\0"\
  "\0"

#define MY_TALE5 \
  "Cuando desperto el dinosaurio\0" \
  "todavia estaba\0"\
  "D;S#alli\0"\
  "\0"

#define MY_TALE6 \
  "Mi madre me ajusta el cuello del abrigo,\0" \
  "no porque empieza a nevar,\0"\
  "sino para que empiece a nevar\0"\
  "\0"
  
#define MY_TALE7 \
  "Esas sirenas enloquecidas que aullan\0" \
  "recorriendo la ciudad en busca de\0"\
  "D;M#Ulises\0"\
  "\0"

#define MY_TALE8 \
  "Estabas a ras de tierra y no te vi\0" \
  "Tuve que cavar hasta el fondo de mi para encontrarte\0"\
  "\0"

#define MY_TALE9 \
  "Mientras subia y subia,\0"\
  "el globo lloraba al ver que se le escapaba el ninyo\0"\
  "\0"

char* MY_TALES[] = { MY_TALE1, MY_TALE2, MY_TALE3, MY_TALE4, MY_TALE5, MY_TALE6, MY_TALE7, MY_TALE8, MY_TALE9, "\0"};
// char* MY_TALES[] = { MY_TALE6, "\0"};

#define WAIT_MILIS 2000    // How many milis we wait for messages from Serial before continue with the tale

// We have a "problem" here : how to we know if actually we are showing a tale? One way
// wold be using the value of IND_CURR_TALE but then I have to assign (there are no NULLs here) an special
// value that could be
// - -1 => but then we have to pass from uint16_t  => int (more space)
// 0 and the index start counting from 1 => that means problems
// So the clearest solutiob is the use of the auxilliary boolean REMDERING_TALE
uint8_t TOT_TALES = 0;
bool RENDERING_TALE = false;
uint16_t IND_CURR_TALE = 0;
uint16_t IND_LINE_CURR_TALE = 0;
uint16_t TOT_LINES_CURR_TALE = 0;
unsigned long START_TIME = 0; // How many milis we're waiting


// PINS
#define CLK_PIN 13   // LED : or SCK
#define DATA_PIN 11  // LED : or MOSI
#define CS_PIN 10    // LED : or SS
#define RX_PIN 2     // BLUE 
#define TX_PIN 3     // BLUE


#ifdef __DEBUG__
#define PRINT(a)    { Serial.print(a); }
#define PRINTLN(a)  { Serial.println(a); }
#define PRINT(x, y) { Serial.print(x); Serial.print(y); }
#define PRINTLN(x, y) { Serial.print(x); Serial.println(y); }
#else
#define PRINT(a)
#define PRINTLN(a)
#define PRINT(x, y)
#define PRINTLN(x, y)
#endif

// LED
#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW
#define MAX_DEVICES 4
#define MAX_COLS_PER_DEVICE 8

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Bluetooth
SoftwareSerial BT(RX_PIN, TX_PIN);

// ====================================================================
//
// FUNCTIONS : Utilities
//
// ====================================================================

// ====================================================================
// TALE
// A TALE is a char* composed by several lines where:
// - Every line ends with \0
// - A line only with \0 is the end of the Tale
// TODO : we repeat portions of the same code several times, we should
//        optimize if creating a generic function
// ====================================================================

uint16_t getNumLines(char* tale) {
  uint16_t num_lines = 0;
  uint16_t length_line = 0;

  while ( *tale || length_line != 0 ) {
    if ( !*tale ) {
      ++ num_lines;
      length_line = 0;
    } else {
      ++length_line;
    }
    ++tale;

  }

  return num_lines;
}

/**
   Get the length of a certain line
*/
uint16_t getLineLength(char* tale, uint16_t ind_line) {
  uint16_t curr_line_ind = 0;
  uint16_t curr_line_length = 0;

  // While it is not the end of the tale (that is, a line with only \0)
  while ( *tale || curr_line_length != 0 ) {
    // End of a line
    if ( !*tale ) {
      // We have found the line we where asking for
      if ( curr_line_ind == ind_line ) {
        break;
        // Next line
      } else {
        ++ curr_line_ind ;
        curr_line_length = 0;
      }
      // Normal character
    } else {
      ++curr_line_length;
    }
    ++tale;
  }

  // TODO : return null in case they ask for a line non existing
  return curr_line_length;
}

String getLine(char* tale, uint16_t ind_line) {
  // char* line = (char *)malloc(getLineLength(tale, ind_line) + 1);
  String line="";
  uint16_t curr_line_ind = 0;
  uint16_t curr_line_length = 0;

  //char* p_line = line;
  // While it is not the end of the tale (that is, a line with only \0)
  while ( *tale || curr_line_length != 0 ) {
    // End of a line
    if ( !*tale ) {
      // We have found the line we where asking for
      if ( curr_line_ind == ind_line ) {
        break;
        // Next line
      } else {
        ++ curr_line_ind ;
        curr_line_length = 0;
      }
      // Normal character
    } else {
      if ( curr_line_ind == ind_line ) {
        //*p_line++ = *tale;
        line += *tale;
      }
      ++curr_line_length;
    }
    ++tale;
  }
  //*p_line = '\0';

  return line;
}


/**
    Count the number of ones this number has
*/
uint8_t numberOfOnes(uint8_t value) {
  uint8_t tot = 0;
  while ( value > 0 ) {
    if ( value & 1 ) {
      ++tot;
    }
    value = value >> 1;
  }

  return tot;
}

/**
   The mask represents the values in a column
*/
uint8_t getNextMask(uint8_t final_mask, uint8_t curr_mask ) {
  uint8_t value = final_mask ^ curr_mask; // XOR
  uint8_t one_to_find = getRandom(numberOfOnes(value)) + 1;
  uint8_t my_pos = 0;
  while ( value ) {
    // If we find a 1, let's see if it is the one that we were looking for
    if ( value & 1 ) {
      --one_to_find;
    }
    if ( one_to_find == 0 ) {
      break;
    } else {
      value = value >> 1;
      ++my_pos;
    }
  }

  return curr_mask | ( 1 << my_pos );
}

/**
   Return a value [0, max)
*/
uint8_t getRandom(uint8_t max) {
  return rand() % max;
}

/**
   Add a number representing a column in buffer, that contains all the columns displayed.
*/
uint8_t addColInBuffer(uint8_t *buffer, uint8_t col, uint8_t curr_length, uint8_t max_length) {
  // If there is no room, we have to shift the values
  if ( curr_length == max_length ) {
    for (uint8_t ind = 0; ind < max_length; ++ind) {
      buffer[ind] = buffer[ind + 1];
    }
    --curr_length;
  }
  // Now we can safely add the new value
  buffer[curr_length] = col;

  return curr_length + 1;
}

/*
   Length | Quotient| Display
   1-8    |   0     | 3
   9-16   |   1     | 2
   17-24  |   2     | 1
   25-32  |   3     | 0
*/
uint8_t getDisplay(uint8_t curr_length, uint8_t num_displays, uint8_t num_cols_per_display) {
  return (num_displays - 1) - ( (curr_length - 1) / num_cols_per_display);
}

/*
   Modulus | Column
   0       | 7
   1       | 0
   2       | 1
   3       | 2
   4       | 3
   5       | 4
   6       | 5
   7       | 6
*/
uint8_t getColumn(uint8_t curr_length, uint8_t num_displays, uint8_t num_cols_per_display) {
  uint8_t col = curr_length % num_cols_per_display;

  return col == 0 ? num_cols_per_display - 1 : col - 1;
}

// ====================================================================
//
// FUNCTIONS : Main functions
//
// ====================================================================

/**
   Show a text dot my dot
*/
void showDotByDot(String my_text, uint8_t p_delay) {
  uint8_t charWidth;
  uint8_t cBuf[8]; // this should be ok for all built-in fonts

  mx.clear();
  delay(10);

  // 1> First compute text with the value of all the columns
  uint8_t text[MAX_DEVICES * MAX_COLS_PER_DEVICE];
  int text_length = 0;
  for (uint8_t ind_my_text = 0; ind_my_text < my_text.length(); ++ind_my_text) {
    charWidth = mx.getChar(my_text[ind_my_text], sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
    for (uint8_t i = 0; i <= charWidth; i++) {
      uint8_t value = i < charWidth ? cBuf[i] : 0;
      text_length = addColInBuffer(text, value, text_length, MAX_DEVICES * MAX_COLS_PER_DEVICE);
    }
  }

  // 1.1> Let's center the text adding empty cols
  uint8_t cols_to_add = (MAX_DEVICES * MAX_COLS_PER_DEVICE - text_length) / 2;
  for (uint8_t i = 0; i < cols_to_add; i++) {
    text_length = addColInBuffer(text, 0, text_length, MAX_DEVICES * MAX_COLS_PER_DEVICE);
  }

  // 2> Compute how many dots we have to show
  uint8_t tot_dots = 0;
  for (uint8_t i = 0; i < text_length; i++) {
    tot_dots += numberOfOnes(text[i]);
  }

  PRINT(">>> text_length : ", text_length);
  PRINTLN(", tot_dots : ", tot_dots);

  // 3> Now show dot a dot
  uint8_t text_shown[MAX_DEVICES * MAX_COLS_PER_DEVICE] = {0};
  uint8_t rnd_col = 0;
  uint8_t ind_display_to_show = 0;
  uint8_t ind_col_to_show = 0;


  while ( tot_dots ) {
    // Serial.println("Looking for a free column...");
    // Pick a random column that still has points to be shown
    while ( true ) {
      rnd_col = getRandom(text_length);
      if ( text[rnd_col] != text_shown[rnd_col] ) {
        break;
      }
    }
    /*
    PRINT("[", tot_dots);
    PRINT("] rnd_col : ", rnd_col);
    PRINT(", text_to_be_shown : ", text[rnd_col]);
    PRINT(", text_shown_until_now: ", text_shown[rnd_col]);
    */
    
    // Now get the next mask
    text_shown[rnd_col] = getNextMask(text[rnd_col], text_shown[rnd_col]);

    // PRINT(", text_shown_now : ", text_shown[rnd_col]);

    ind_display_to_show = getDisplay(text_length - rnd_col, MAX_DEVICES, MAX_COLS_PER_DEVICE);
    ind_col_to_show = getColumn (text_length - rnd_col, MAX_DEVICES, MAX_COLS_PER_DEVICE);

    // PRINT(", display : ", ind_display_to_show );
    // PRINTLN(", column : ", ind_col_to_show);

    mx.setColumn(
      ind_display_to_show,
      ind_col_to_show,
      text_shown[rnd_col]
    );

    --tot_dots;
    delay(p_delay);
  }
}

/**
   Scroll a text from LEFT -> RIGT
*/
void scrollText(String my_text, uint8_t p_delay) {
  uint8_t charWidth;
  uint8_t cBuf[8]; // this should be ok for all built-in fonts
  // This represents all the text that can be shown in the display
  uint8_t text[MAX_DEVICES * MAX_COLS_PER_DEVICE];
  // This represents the cuurent length of text
  int text_length = 0;

  mx.clear();
  delay(10);

  // Loop over the characters
  for (uint8_t ind_my_text = 0; ind_my_text < my_text.length(); ++ind_my_text) {
    charWidth = mx.getChar(my_text[ind_my_text], sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
    // Loop over all the numbers that represent the columns for that character
    for (uint8_t i = 0; i <= charWidth; i++) {
      uint8_t value = i < charWidth ? cBuf[i] : 0;
      text_length = addColInBuffer(text, value, text_length, MAX_DEVICES * MAX_COLS_PER_DEVICE);

      // Ok, now display 'text' from LEFT -> RIGHT
      // We have to compute in which display and col the first character
      // will be shown and then move to the right
      uint8_t indDisplay = getDisplay(text_length, MAX_DEVICES, MAX_COLS_PER_DEVICE);
      uint8_t indCol = getColumn(text_length, MAX_DEVICES, MAX_COLS_PER_DEVICE);
      for (uint8_t indText = 0; indText < text_length; indText++) {
        mx.setColumn(indDisplay, indCol, text[indText]);
        // Move to the rigth but if we were already in the
        // rightermost column of that display (indCol=0) we
        // have to move to the next display
        if ( indCol == 0  ) {
          indCol = MAX_COLS_PER_DEVICE - 1;
          ++indDisplay;
        } else {
          --indCol;
        }
      }
      delay(p_delay);
    }
  }
}

// ====================================================================
//
// MAIN
//
// ====================================================================

void setup() {
  Serial.begin(9600);

  mx.begin();
  mx.clear();

  BT.begin(9600);
  START_TIME = millis();

  // Total of tales
  char** tale = MY_TALES;
  TOT_TALES = 0;
  while ( **tale ) {
    ++TOT_TALES;
    ++tale;
  }
}

String state = "";
void loop() {
  while (BT.available() ) {
    delay(10);
    char c = BT.read();
    state += c;
  }


  if ( state.length() > 0 ) {
    processMessage(state);
    state = "";
  } else {
    // We have waited enough, let's render a new line of the tale
    // or pick a tale if we are not rendering one
    if ( (millis() - START_TIME ) > WAIT_MILIS ) {
      if ( !RENDERING_TALE || IND_LINE_CURR_TALE == getNumLines(MY_TALES[IND_CURR_TALE])) {
        RENDERING_TALE = true;
        IND_CURR_TALE = getRandom(TOT_TALES);
        IND_LINE_CURR_TALE = 0;
        PRINTLN(">>> Pick a new tale : ", IND_CURR_TALE);
      }

      // char* line=getLine(MY_TALES[IND_CURR_TALE], IND_LINE_CURR_TALE);
      String line=getLine(MY_TALES[IND_CURR_TALE], IND_LINE_CURR_TALE);
      PRINTLN(">>> Let's show the line : ", line);
      processMessage(line);
      ++IND_LINE_CURR_TALE ;

      START_TIME = millis();
    }
  }
}

/**
  message has the form:
  [command#]text
  where command is optional y can have the form:
  effect;speed
  where (all are optional):
  - effect:
   + S : Scroll (default)
   + D : Dot by dot
  - speed:
   + I : Instantaneus
   + S : Slow
   + M : Medium (default)
   + F : Fast
*/
void processMessage(String message ) {
  PRINT(">>> Message : [", message);
  PRINTLN("]", "");

  // Get the effect, the configuration and the text
  String s_text = "";
  String s_speed = "";
  String s_effect = "";

  bool process_text = false;
  bool process_speed = false;
  bool process_effect = true;

  for (uint8_t ind = 0; ind < message.length(); ++ind) {
    char ch = message[ind];

    if ( process_text ) {
      s_text += ch;
    } else {
      if ( ch == '#' ) {
        process_text = true;
      } else {
        if ( ch == ';' ) {
          if ( process_effect ) {
            process_speed = true;
          }
        } else {
          if ( process_speed ) {
            s_speed += ch;
          } else if ( process_effect ) {
            s_effect += ch;
          }
        }
      }
    }
  }
  // It the message did not contain an # means all the message is text
  if ( !process_text ) {
    process_effect = false;
    s_text = message;
  }

  if ( !process_effect || !s_effect.length()) s_effect = 'S';
  if ( !process_speed || !s_speed.length()) s_speed = 'I';

  PRINT("Effect : ", s_effect);
  PRINT(", Speed : ", s_speed);
  PRINT(", Text : ", s_text);

  // Get the delay based on the speed
  uint8_t i_delay = 0;
  if ( s_speed == "I" ) {
    i_delay = 0;
  } else if ( s_speed == "S" ) {
    i_delay = 60;
  } else if ( s_speed == "F" ) {
    i_delay = 10;
  } else {
    i_delay = 30;
  }

  PRINTLN(", i_delay : ", i_delay);

  if ( s_effect == "D" ) {
    showDotByDot(s_text, i_delay);
  } else  {
    scrollText(s_text, i_delay);
  }
}

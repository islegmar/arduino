#include <MD_MAX72xx.h >

/*

  F(x) is a macro that stores “x” string
  in flash memory, not SRAM. Flash memory is a memory in the
  microcontrollers where programs are stored, and SRAM memory is a
  type of RAM memory. We decrease limit of memory for our programs
  when using F(x) macro, which increases SRAM memory (very
  important for running programs on microcontrollers). If you do not use
  F(x) macro, most likely that your program will not work as designed.
  That is because, in the sketch, there are too many strings, which in the C
  programming language are char arrays and that eats up SRAM memory
  easily.
*/
#define DEBUG 1

#if DEBUG
#define PRINT(s, x) { Serial.print(F(s)); Serial.print(x); }
#define PRINTS(x) Serial.print(F(x))
#define PRINTD(x) Serial.println(x, DEC)
#else
#define PRINT(s, x)
#define PRINTS(x)
#define PRINTD(x)
#endif

#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW

#define MAX_DEVICES 4
#define MAX_COLS_PER_DEVICE 8

#define CLK_PIN 13 // or SCK
#define DATA_PIN 11 // or MOSI
#define CS_PIN 10 // or SS

MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

#define DELAYTIME 30 // in milliseconds

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
  return (num_displays - 1) - ( (curr_length-1) / num_cols_per_display);
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

/**
   Show a text dot my dot
*/
void showDotByDot(char *p) {
  uint8_t charWidth;
  uint8_t cBuf[8]; // this should be ok for all built-in fonts
  // This represents all the text that can be shown in the display

  mx.clear();
  delay(10);

  // 1> First compute text with the value of all the columns
  uint8_t text[MAX_DEVICES * MAX_COLS_PER_DEVICE];
  int text_length = 0;
  while (*p != '\0') {
    charWidth = mx.getChar(*p++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
    for (uint8_t i = 0; i <= charWidth; i++) {
      uint8_t value = i < charWidth ? cBuf[i] : 0;
      text_length = addColInBuffer(text, value, text_length, MAX_DEVICES * MAX_COLS_PER_DEVICE);
    }
  }
  /*
  Serial.print(", text_length : ");
  Serial.print(text_length);
  Serial.print(", MAX_DEVICES * MAX_COLS_PER_DEVICE : ");
  Serial.print(MAX_DEVICES * MAX_COLS_PER_DEVICE);
  Serial.print(", (MAX_DEVICES * MAX_COLS_PER_DEVICE-text_length)/2 : ");
  Serial.println((MAX_DEVICES * MAX_COLS_PER_DEVICE-text_length)/2);
  */
 
  // 1.1> Let's center the text adding empty cols
  uint8_t cols_to_add=(MAX_DEVICES * MAX_COLS_PER_DEVICE-text_length)/2;
  for (uint8_t i = 0; i < cols_to_add; i++) {
    text_length = addColInBuffer(text, 0, text_length, MAX_DEVICES * MAX_COLS_PER_DEVICE);
  }

  // 2> Compute how many dots we have to show
  uint8_t tot_dots = 0;
  for (uint8_t i = 0; i < text_length; i++) {
    tot_dots += numberOfOnes(text[i]);
  }

  // 3> Now show dot a dot
  uint8_t text_shown[MAX_DEVICES * MAX_COLS_PER_DEVICE] = {0};
  uint8_t rnd_col = 0;
  uint8_t ind_display_to_show = 0;
  uint8_t ind_col_to_show = 0;

  /*
  Serial.print(">>> text_length : ");
  Serial.print(text_length);
  Serial.print(", tot_dots : ");
  Serial.println(tot_dots);
  */
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
    Serial.print("[");
    Serial.print(tot_dots);
    Serial.print("] rnd_col : ");
    Serial.print(rnd_col);
    Serial.print(", text : ");
    Serial.print(text[rnd_col]);
    Serial.print(", text_before : ");
    Serial.print(text_shown[rnd_col]);
    */
    
    // Now get the next mask
    text_shown[rnd_col] = getNextMask(text[rnd_col], text_shown[rnd_col]);
    //Serial.print(", text_after : ");
    //Serial.print(text_shown[rnd_col]);

    ind_display_to_show = getDisplay(text_length - rnd_col, MAX_DEVICES, MAX_COLS_PER_DEVICE);
    ind_col_to_show = getColumn (text_length - rnd_col, MAX_DEVICES, MAX_COLS_PER_DEVICE);
    /*
    Serial.print(", display : ");
    Serial.print(ind_display_to_show );
    Serial.print(", column : ");
    Serial.println(ind_col_to_show);
    */
    mx.setColumn(
      ind_display_to_show,
      ind_col_to_show,
      text_shown[rnd_col]
    );

    --tot_dots;
    delay(DELAYTIME);
  }
  Serial.println("DONE!");
}

/*
  First function is called scrollingText(), which is used for scrolling
  text on display. The scrollingText() function use the function from
  the “MD_MAX72XX.h” library called setColumn(). This function turns
  ON or OFF one column array of LEDs on the screen. The rest of the
  function is algorithm for the scrolling effect. We take string “p” from flash
  memory, and convert it into an array of unsigned integers which
  represents the LEDs in the column array. Then we display that array on
  the screen. At the end of the function we specify the delay time, which is
  the speed of the scrolling text.
  Let's understand
    charWidth = mx.getChar(*p++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);

  charWidth is a number with the number of cols the character will have when
  displayed. Some examples:
  - G : 5
  - i : 1
  In the meantime cBuf is an array of numbers that im binary form represents
  the on/off status of the leds in the Column.
  For example the G will be shown as (just the 2 columns in the left)
   X
  X
  X
  X
  X
  X
  X
   X
  and
  - cBuf[0] = 62 = 0111110
  - cBuf[1] = 65 = 1000001
*/
void scrollText(char *p) {
  uint8_t charWidth;
  uint8_t cBuf[8]; // this should be ok for all built-in fonts
  // This represents all the text that can be shown in the display
  uint8_t text[MAX_DEVICES * MAX_COLS_PER_DEVICE];
  // This represents the cuurent length of text
  int text_length = 0;

  mx.clear();
  delay(10);

  // Loop over the characters
  while (*p != '\0') {
    //Serial.print(">>> DISPLAY NEW CHAR : '");
    //Serial.print(*p);

    charWidth = mx.getChar(*p++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);

    //Serial.print("' that has ");
    //Serial.print(charWidth);
    //Serial.println(" columns +1 that we add to have the space.");

    // Loop over all the numbers that represent the columns for that character
    for (uint8_t i = 0; i <= charWidth; i++) {
      uint8_t value = i < charWidth ? cBuf[i] : 0;
      text_length = addColInBuffer(text, value, text_length, MAX_DEVICES * MAX_COLS_PER_DEVICE);
      //Serial.print(">> [length=");
      //Serial.print(text_length);
      //Serial.print("] Added a new column with value : ");
      //Serial.println(value);

      // Ok, now display 'text' from LEFT -> RIGHT
      // We have to compute in which display and col the first character
      // will be shown and then move to the right
      uint8_t indDisplay = getDisplay(text_length, MAX_DEVICES, MAX_COLS_PER_DEVICE);
      uint8_t indCol = getColumn(text_length, MAX_DEVICES, MAX_COLS_PER_DEVICE);
      for (uint8_t indText = 0; indText < text_length; indText++) {
        /*Serial.print("IndText : ");
          Serial.print(indText);
          Serial.print(", Display : ");
          Serial.print(indDisplay);
          Serial.print(", Col : ");
          Serial.print(indCol);
          Serial.print(", Value : ");
          Serial.print(text[indText]);
          Serial.println();*/
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
      delay(DELAYTIME);
    }
  }
}
void rows() {
  mx.clear();
  for (uint8_t row = 0; row < ROW_SIZE; row++) {
    mx.setRow(row, 0xff); delay(2 * DELAYTIME);
    mx.setRow(row, 0x00);
  }
}
void columns() {
  mx.clear();
  for (uint8_t col = 0; col < mx.getColumnCount(); col++) {
    mx.setColumn(col, 0xff); delay(DELAYTIME / MAX_DEVICES);
    mx.setColumn(col, 0x00);
  }
}
void stripe() {
  const uint16_t maxCol = MAX_DEVICES * ROW_SIZE;
  const uint8_t stripeWidth = 10;
  mx.clear();
  for (uint16_t col = 0; col < maxCol + ROW_SIZE + stripeWidth; col++) {
    for (uint8_t row = 0; row < ROW_SIZE; row++) {
      mx.setPoint(row, col - row, true);
      mx.setPoint(row, col - row - stripeWidth, false);
    }
    delay(DELAYTIME);
  }
}
void spiral() {
  int rmin = 0, rmax = ROW_SIZE - 1;
  int cmin = 0, cmax = (COL_SIZE * MAX_DEVICES) - 1;
  mx.clear();
  while ((rmax > rmin) && (cmax > cmin)) {
    for (int i = cmin; i <= cmax; i++) { // do row
      mx.setPoint(rmin, i, true); delay(DELAYTIME / MAX_DEVICES);
    }
    rmin++;
    for (uint8_t i = rmin; i <= rmax; i++) { // do column
      mx.setPoint(i, cmax, true); delay(DELAYTIME / MAX_DEVICES);
    }
    cmax--;
    for (int i = cmax; i >= cmin; i--) { // do row
      mx.setPoint(rmax, i, true); delay(DELAYTIME / MAX_DEVICES);
    }
    rmax--;
    for (uint8_t i = rmax; i >= rmin; i--) { // do column
      mx.setPoint(i, cmin, true); delay(DELAYTIME / MAX_DEVICES);
    }
    cmin++;
  }
}

void bounce() {
  const int minC = 0;
  const int maxC = mx.getColumnCount() - 1;
  const int minR = 0;
  const int maxR = ROW_SIZE - 1;
  int nCounter = 0;
  int r = 0, c = 2;
  int8_t dR = 1, dC = 1; // delta row and column
  mx.clear();
  while (nCounter++ < 200) {
    mx.setPoint(r, c, false);
    r += dR; c += dC;
    mx.setPoint(r, c, true);
    delay(DELAYTIME / 2);
    if ((r == minR) || (r == maxR)) {
      dR = -dR;
    }
    if ((c == minC) || (c == maxC)) {
      dC = -dC;
    }
  }
}
void intensity() {
  uint8_t row;
  mx.clear();
  for (int8_t i = 0; i <= MAX_INTENSITY; i++) {
    mx.control(MD_MAX72XX::INTENSITY, i);
    mx.setRow(0, 0xff); delay(DELAYTIME * 10);
  }
  mx.control(MD_MAX72XX::INTENSITY, 8);
}

void transformation() {
  uint8_t arrow[COL_SIZE] = {
    0b00001000,
    0b00011100,
    0b00111110,
    0b01111111,
    0b00011100,
    0b00011100,
    0b00111110,
    0b00000000
  };
  MD_MAX72XX::transformType_t t[] = {
    MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
    MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
    MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
    MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL, MD_MAX72XX::TSL,
    MD_MAX72XX::TFLR,
    MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR, MD_MAX72XX::TSR,
    MD_MAX72XX::TRC,
    MD_MAX72XX::TSD, MD_MAX72XX::TSD, MD_MAX72XX::TSD, MD_MAX72XX::TSD,
    MD_MAX72XX::TSD, MD_MAX72XX::TSD, MD_MAX72XX::TSD, MD_MAX72XX::TSD,
    MD_MAX72XX::TFUD,
    MD_MAX72XX::TSU, MD_MAX72XX::TSU, MD_MAX72XX::TSU, MD_MAX72XX::TSU,
    MD_MAX72XX::TSU, MD_MAX72XX::TSU, MD_MAX72XX::TSU, MD_MAX72XX::TSU,
    MD_MAX72XX::TINV,
    MD_MAX72XX::TRC, MD_MAX72XX::TRC, MD_MAX72XX::TRC, MD_MAX72XX::TRC,
    MD_MAX72XX::TINV
  };
  mx.clear();
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
  for (uint8_t j = 0; j < mx.getDeviceCount(); j++) {
    mx.setBuffer(((j + 1)*COL_SIZE) - 1, COL_SIZE, arrow);
  }
  mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
  delay(DELAYTIME);
  mx.control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::ON);

  // one tab
  for (uint8_t i = 0; i < (sizeof(t) / sizeof(t[0])); i++) {
    mx.transform(t[i]); delay(DELAYTIME * 4);
  }
  mx.control(MD_MAX72XX::WRAPAROUND, MD_MAX72XX::OFF);
}
void showCharset() {
  mx.clear();
  mx.update(MD_MAX72XX::OFF);
  for (uint16_t i = 0; i < 256; i++) {
    mx.clear(0);
    mx.setChar(COL_SIZE - 1, i);
    if (MAX_DEVICES >= 3) {
      char hex[3];
      sprintf(hex, "%02X", i);
      mx.clear(1); mx.setChar((2 * COL_SIZE) - 1, hex[1]);
      mx.clear(2); mx.setChar((3 * COL_SIZE) - 1, hex[0]);
    }
    mx.update();
    delay(DELAYTIME * 2);
  }
  mx.update(MD_MAX72XX::ON);
}
void setup() {
  mx.begin();
  mx.clear();
  /*
  Serial.begin(9600);
  Serial.println("START");
  mx.setPoint(0, 0, 1);
  mx.setPoint(7, 0, 1);
  // 22 = 10110
  Serial.println(22);
  for (int ind = 0; ind < 10; ++ind) {
    uint8_t mask = 0;

    mask = getNextMask(22, mask);
    Serial.print(mask);
    Serial.print(" - ");

    mask = getNextMask(22, mask);
    Serial.print(mask);
    Serial.print(" - ");

    mask = getNextMask(22, mask);
    Serial.print(mask);
    Serial.println(" - ");
  }

  for (uint8_t ind = 1; ind <= 32; ++ind) {
    uint8_t display = getDisplay(ind, 4, 8);
    uint8_t col = getColumn(ind, 4, 8);
    Serial.print("Length : ");
    Serial.print(ind);
    Serial.print(", Display : ");
    Serial.print(display);
    Serial.print(", Column : ");
    Serial.print(col);
    Serial.println();
  }
  */
  scrollText("Habia una");
  showDotByDot("vez");
  scrollText("un gran");
  showDotByDot("dragon");
}
int doLoop = 0;

void loop() {
  if ( doLoop ) {
    //showCharset();
    //scrollText("Holo");
    //showDotByDot("H");
    /*showDotByDot("o");
      showDotByDot("l");
      showDotByDot("o");
    */showDotByDot("Holo");



    /*rows();
      columns();
      stripe();
      bounce();
      spiral();
      intensity();
      transformation();
      showCharset();
      delay(3000);
    */
    doLoop = 0;
  }
}

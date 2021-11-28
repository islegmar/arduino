#ifndef __LEDS__
#define __LEDS__

#include <MD_MAX72xx.h>

// PINS
#define CLK_PIN 13   // or SCK
#define DATA_PIN 11  // or MOSI
#define CS_PIN 10    // or SS

#define HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW
#define MAX_DEVICES 4
#define MAX_COLS_PER_DEVICE 8

extern MD_MAX72XX mx;

extern void showDotByDot(String my_text, uint8_t p_delay);
extern void scrollText(String my_text, uint8_t p_delay);

extern void processMessage(String message );
extern uint8_t numberOfOnes(uint8_t value);
extern uint8_t getNextMask(uint8_t final_mask, uint8_t curr_mask );
extern uint8_t addColInBuffer(uint8_t *buffer, uint8_t col, uint8_t curr_length, uint8_t max_length);
extern uint8_t getDisplay(uint8_t curr_length, uint8_t num_displays, uint8_t num_cols_per_display);
extern uint8_t getColumn(uint8_t curr_length, uint8_t num_displays, uint8_t num_cols_per_display);
 
#endif

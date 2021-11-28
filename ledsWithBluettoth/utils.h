#ifndef __UTILS__
#define __UTILS__

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

extern uint8_t getRandom(uint8_t max);

#endif

#ifndef __TALES__
#define __TALES__

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

#define WAIT_MILIS 2000    // How many milis we wait for messages from Serial before continue with the tale

extern char* MY_TALES[];
extern uint8_t TOT_TALES;
extern bool RENDERING_TALE;
extern uint16_t IND_CURR_TALE;
extern uint16_t IND_LINE_CURR_TALE;
extern uint16_t TOT_LINES_CURR_TALE;
extern unsigned long START_TIME;

extern uint16_t getNumLines(char* tale);
extern uint16_t getLineLength(char* tale, uint16_t ind_line);
extern String getLine(char* tale, uint16_t ind_line);

#endif

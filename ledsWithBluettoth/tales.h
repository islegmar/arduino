#ifndef __TALES__
#define __TALES__

// OK, in te future all those strings will be in an external file so they do not take place but in the meantime....
#define MY_TALE1 \
  "- Te devorare -dijo la Pantera\0" \
  "- Peor para ti -dijo la Espada\0" \
  "\0"

#define MY_TALE2 \
  "La mujer que ame\0"\
  "se ha convertido en fantasma\0" \
  "Yo soy el lugar\0"\
  "de las apariciones\0" \
  "\0"

#define MY_TALE3 \
  "No olvide usted, senyora\0"\
  "las noches en que nuestras almas\0"\
  "lucharon cuerpo a cuerpo\0"\
  "\0"

#define MY_TALE4 \
  "Traedeme el caballo mas veloz\0" \
  "pidio el hombre mas honrado\0"\
  "Acabo de decirle la verdad al rey\0"\
  "\0"

#define MY_TALE5 \
  "Cuando desperto\0"\
  "el dinosaurio\0" \
  "todavia estaba alli\0"\
  "\0"

#define MY_TALE6 \
  "Mi madre me ajusta el cuello del abrigo,\0" \
  "no porque empieza a nevar,\0"\
  "sino para que empiece a nevar\0"\
  "\0"
  
#define MY_TALE7 \
  "Esas sirenas enloquecidas\0"\
  "que aullan recorriendo la ciudad\0"\
  "en busca de Ulises\0"\
  "\0"

#define MY_TALE8 \
  "Estabas a ras de tierra\0"\
  "y no te vi\0" \
  "Tuve que cavar hasta el fondo de mi\0"\
  "para encontrarte\0"\
  "\0"

#define MY_TALE9 \
  "Mientras subia y subia,\0"\
  "el globo lloraba\0"\
  "al ver que se le escapaba el ninyo\0"\
  "\0"

#define MY_TALE10 \
  "Noche sin luna.\0"\
  "La tempestad estruja\0"\
  "los viejos cedros.\0"

#define MY_TALE11 \
  "Paso el ayer,\0"\
  "paso tambien el hoy;\0"\
  "se va la primavera.\0"

#define MY_TALE12 \
  "Al Fuji subes\0"\
  "despacio —pero subes,\0"\
  "caracolito.\0"

#define MY_TALE13 \
  "Corte una rama\0"\
  "y clareo mejor\0"\
  "por la ventana.\0"

#define MY_TALE14 \
  "Es un imperio\0"\
  "esa luz que se apaga\0"\
  "o una luciernaga?\0"

#define MY_TALE15 \
  "Hecho de aire\0"\
  "entre pinos y rocas\0"\
  "brota el poema.\0"

#define MY_TALE16 \
  "Tiembla el rocío\0"\
  "y las hojas moradas\0"\
  "y un colibri\0"

#define MY_TALE17 \
  "Ladron cazado:\0"\
  "con los bolsillos llenos\0"\
  "de setas frescas.\0"


#define WAIT_MILIS 2000    // How many milis we wait for messages from Serial before continue with the tale

extern char* MY_TALES[];
extern uint8_t TOT_TALES;
extern bool RENDERING_TALE;
extern bool CAN_TALES_BE_RENDERED;
extern uint16_t IND_CURR_TALE;
extern uint16_t IND_LINE_CURR_TALE;
extern uint16_t TOT_LINES_CURR_TALE;
extern unsigned long START_TIME;

extern uint16_t getNumLines(char* tale);
extern uint16_t getLineLength(char* tale, uint16_t ind_line);
extern String getLine(char* tale, uint16_t ind_line);

#endif

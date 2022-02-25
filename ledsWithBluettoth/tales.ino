/**
 * TALE
 * A TALE is a char* composed by several lines where:
 * - Every line ends with \0
 * - A line only with \0 is the end of the Tale
 * TODO : we repeat portions of the same code several times, we should
 *        optimize if creating a generic function
 */
#ifdef __TALES__

// char* MY_TALES[] = { MY_TALE1, MY_TALE2, MY_TALE3, MY_TALE4, MY_TALE5, MY_TALE6, MY_TALE7, MY_TALE8, MY_TALE9, MY_TALE10, MY_TALE11, MY_TALE12, MY_TALE13, MY_TALE14, MY_TALE15, MY_TALE16, MY_TALE17, "\0"};
char* MY_TALES[] = { MY_TALE1, MY_TALE2, MY_TALE3, MY_TALE4, MY_TALE5, MY_TALE6, MY_TALE7, MY_TALE8, MY_TALE9, MY_TALE10, MY_TALE11, MY_TALE12, MY_TALE13, MY_TALE14, MY_TALE15, "\0"};
uint8_t TOT_TALES = 0;
uint16_t IND_CURR_TALE = 0;
uint16_t IND_LINE_CURR_TALE = 0;
uint16_t TOT_LINES_CURR_TALE = 0;
unsigned long START_TIME = 0; // How many milis we're waiting
// TODO : probably those two variables can ba just one
bool RENDERING_TALE = false;
bool CAN_TALES_BE_RENDERED = false;

/**
 * Get the number of lines this tale has.
 * 
 * @param tale : the tale with all the lines
 */
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
 * Get the length of a certain line in tale.
 * 
 * @param tale : the tale
 * @param ind_tale : the number of the line (starting by 0)
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

/**
 * Get the content of a certain line in a tale.
 * 
 * @param tale : the tale
 * @param ind_tale : the position of the line in the tale (starting by 0)
 */
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
#endif

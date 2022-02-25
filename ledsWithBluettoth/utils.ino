/**
 * Return a random integer value in the range [0, max-1]
 */
uint8_t getRandom(uint8_t max) {
  return rand() % max;
}
/*
float average ( char *str, ... ){
    va_list arguments;                     

    // Initializing arguments to store all values after num
    va_start ( arguments, str );           
    //Sum all the inputs; we still rely on the function caller to tell us how
    // many there are 
    for ( int x = 0; x < num; x++ ) {
        sum += va_arg ( arguments, char *); 
    }
    va_end ( arguments );                  // Cleans up the list

    return sum / num;                      
}
*/

// void debug(char *b,...)
// {  char *r = b, *s = b;                /* Pointers to buffer                  */
//    va_list ap;                         /* Parameter list reference            */
//    while (*s) ++s;                     /* Find buffer string terminator       */
//    va_start(ap,b);                     /* Initialize ap                       */
//    while (b = va_arg(ap,char *))       /* If the next parameter is non-NULL   */
//       while (*b) *s++ = *b++;          /* Concatenate string to buffer        */
//    va_end(ap);                         /* Done with parameter list            */
//    *s = '\0';                          /* Terminate the concatenated string   */
//    return r;                           /* Return pointer to buffer            */
// }  

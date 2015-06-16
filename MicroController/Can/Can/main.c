
#include <stdlib.h> 
#include <stdio.h>

#include <avr/io.h>

#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <util/delay.h>
#include <math.h>

#include "PinsConfig.h"


#include "Can.h"


int main( void )
{
    setOutput(LED_DDR, LED_PIN);

    
    sei();

   
    return 0;
    
}





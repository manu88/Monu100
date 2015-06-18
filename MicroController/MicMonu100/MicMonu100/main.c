
#include <stdlib.h> 
#include <stdio.h>

#include <avr/io.h>

#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>

#include "PinsConfig.h"

#include "Sensors.h"

#include "Display.h"
//#include "Chars.h"
#include "Coms.h"


Display _display;
Sensors _sensors;

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*
 
 TODO : 
  - defilement vertical
  - 2 tailles  : 1/2 plaque + plaque entiere
  - trace avec LUMIERE qui se décalle dans le même sens que la typo + temps de vie avec fade out
  - colision avec les OMBRES

 */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void AnimCircle(void)
{
    uint8_t r = 0;
    int dir = 1;
    while (1)
    {
        display_drawCircle( &_display, 15, 15, r);
        
        _delay_ms(5);
        
        r+=dir;
        
        if (r>15)
        {
            dir*=-1;
            display_setFillColor( &_display, 0);
        }
        if ( r<=0)
        {
            dir*=-1;
            display_setFillColor( &_display, 255);
        }
        
        
    }

}


void testLines(void)
{
    display_setFillColor( &_display, 255);
    display_drawLine(&_display,  4, 0 , 25 ,29);
    while (1)
    {
    }
}



void userCall()
{
    toggle(LED_PORT, LED_PIN);
//    _delay_ms( 10 );
}
int main( void )
{

    /* *** *** *** *** *** *** *** *** *** */
    
    serial_init();



    
    display_init( &_display);
    
    sensors_init( &_sensors);
    
    _sensors.display = &_display;
//    serial_init();

    
    sei();

    display_clear( &_display );
    
    
    
    display_setFillColor( &_display, 255);
    display_fillZone( &_display, 0, 0, 4, 4);
    
//    sensors_calibration( &_sensors , 10 );

    display_clearZone( &_display, 0, 0, 4, 4);
    
    const char name[] = "hello";
    const uint8_t nameLen = strlen( name)* getCharWidth();
    
    const char name2[] = "world";
    const uint8_t name2Len = strlen( name2)* getCharHeight();
    int xPos = -nameLen;
    
    int yPos = name2Len;

    display_setFontColor( &_display, 255);
    

    _sensors.thresholdHigh = 400; //200
    _sensors.thresholdLow =  5;
    
    display_setFillColor( &_display, 255);

    
    for (;;)
    {
        readAll( &_sensors);
        toggle(LED_PORT, LED_PIN);
        _delay_ms( 20 );
        
        
//        display_clear( &_display );
        



         // HORIZONTAL
        display_clearZone(&_display, xPos-1, 3, nameLen, 3+getCharHeight() );
        display_write( &_display, name, xPos, 4,0);

        
        // VERTICAL
        display_clearZone(&_display, 3 ,yPos, 3+getCharWidth(), name2Len );
        display_write( &_display, name2, 4 ,yPos,1);

        
//        _delay_ms( 30 );
        
        xPos +=1;
        yPos -=1;
        
        if ( xPos > 30)
            xPos = -nameLen;
        
        if ( yPos < -name2Len)
            yPos = name2Len*2;

        
        
  
    } // end endless for
    

    return 0;
    
}





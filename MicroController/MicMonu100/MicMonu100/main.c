
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
    

    const char name2[] = "hello world";
    const int name2Len = strlen( name2)* getCharHeight();
    
    
    
    int yPos = 30;

    display_setFontColor( &_display, 255);
    

    _sensors.thresholdHigh = 400; //200
    _sensors.thresholdLow =  5;
    
    display_setFillColor( &_display, 255);

    
    int count = 0;
    const int countMax = 10;
    
    uint8_t animStarted = 0;
    for (;;)
    {
        _delay_ms( 20 );

        readAll( &_sensors);
//        sensors_drawInter( &_sensors );

        
        /*
        count++;
        
        if (count >=countMax)
        {

            if (!animStarted)
            {
                animStarted = 1;

            }

        }
        
        if (animStarted)
        {
            setHigh( LED_PORT, LED_PIN );
            
            for (int y = 14; y>=0; y--)
            {
                _delay_ms(5);
                for (int x = 0; x<15; x++)
                {
                    const uint8_t val = y>0? _sensors.values[y-1][x] : 0;
                    _sensors.values[y][x] = 2*val/animStarted;
                }
            }
            
            animStarted++;
            
            if (animStarted > 14)
            {
                animStarted = 0;
                count = 0;
            }
         
//            animStarted = 0;
//            count = 0;


        }
        
        else //if (!animStarted)
        {
            setLow(LED_PORT, LED_PIN);
            readAll( &_sensors);
        }
        
        sensors_drawInter( &_sensors );
        
        */
        
        // TEXT
        /*

        
        // VERTICAL
        display_clearZone(&_display, 3 ,yPos, 3+getCharWidth(), name2Len );
        display_write( &_display, name2, 4 ,yPos,1);

        yPos -=1;
        
        if ( yPos < -name2Len)
            yPos = 30;// name2Len*2;

        */
  
    } // end endless for
    

    return 0;
    
}






#include <stdlib.h> 
#include <stdio.h>

#include <avr/io.h>

#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <util/delay.h>
#include <math.h>

#include "PinsConfig.h"

#include "Sensors.h"

#include "Display.h"
#include "Images.h"

/*
 Low  0xA6 High 0x99 ext 0XFD

 */

/* **** **** **** **** **** **** **** **** **** **** **** **** */
/*
    Global constants defines
 */





Display _display;
Sensors _sensors;



/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
// move to Sensors.h
 inline uint16_t adc_read(uint8_t ch)
{

    ch &= 0b00000111;  // AND operation with 7
    ADMUX = (ADMUX & 0xF8)|ch; // clears the bottom 3 bits before ORing
    
    // start single convertion
    // write ’1′ to ADSC
    ADCSRA |= (1<<ADSC);
    
    // wait for conversion to complete
    // ADSC becomes ’0′ again
    // till then, run loop continuously
    while(ADCSRA & (1<<ADSC));
    
    return (ADC);
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void testAllLetters(void)
{

    display_clear( &_display );
    display_write( &_display,"abcdef", 0, 0);
    display_write( &_display,"ghijkl", 0, 10);
    display_write( &_display,"mnopq", 0, 20);
    
    _delay_ms( 2000 );
    
    display_clear( &_display );
    display_write( &_display,"rstuv", 0, 0);
    display_write( &_display,"wxyz", 0, 10);

    
    _delay_ms( 2000  );

    
    display_clear( &_display );
    display_write( &_display,"ABCDEF", 0, 0);
    display_write( &_display,"GHIJKL", 0, 10);
    display_write( &_display,"MNOPQ", 0, 20);
    
    _delay_ms( 2000 );
    
    display_clear( &_display );
    display_write( &_display,"RSTUV", 0, 0);
    display_write( &_display,"WXYZ", 0, 10);
    
    
    _delay_ms( 2000  );
    
}

void writeCanId(void)
{
    display_clear( &_display);


     display_write( &_display,"can ID", 0, 0);
     
     char str[3];
     sprintf(str, "%i" , 16);
     display_write( &_display,str, 0, 8);
    

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

#define NAME_COUNT 2

const char names[NAME_COUNT][24] =
{
    "Jean\nMarie",
    "Leo\nRossy",
    /*
    "Michel\nDurand",
    "Robert\nJeanne",
    "Claire\nEliot",
    "Leo\nRossy",
    "Marie\nHubert",
     */
};

void writeNext(void)
{

    static uint8_t index=0;
    
//    drawFade(255 , 0);
    
    display_clear( &_display);

    
    
    
    /**/
    const uint8_t from  = 0;
    const uint8_t to    = 254;
    const uint8_t steps = 150;
    
    const uint8_t dV = (to-from)/steps;
    
    display_setFontColor( &_display, from);
    
    
    for (int i =0; i<steps ; i++)
    {
        
        display_write( &_display, names[index] ,1, 4);
        display_setFontColor( &_display, _display.fontColor+=dV );
        
        _delay_ms(5);
    }
    
    /**/
    
    index++;
    
  
    
    if (index == NAME_COUNT)
        index = 0;
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void bigTypo(void)
{
    // img Col = 131 ; Lignes = 20
    
    static int offset = 0;
    
    //   2620
    // - 2518
    // ------
    //   102
    
    for (int x = 0; x<X_TLC_MAX; x++)
    {

        for (int y = 0; y<20; y++)
        {
            const unsigned int pos = x+offset +131*y;
            
            uint8_t val = 0;
            
            if (pos < 131*20 )
                val = test[ pos];
            
            display_setPixel( &_display, x, y+2, val);
            
        }
    }
    
    offset+=1;
    
    if (offset>=101)
        offset = 0;
}
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void test2(void)
{
    
    for (int x = 0; x<X_TLC_MAX; x++)
    {
        
        for (int y = 0; y<Y_MIC_MAX; y++)
        {
            uint8_t val = jean_pix[x + 30*y];
            
            _display.buff_A[y][x] = val;
        }
    }
    
    
    display_setPixel( &_display, 29, 29, 100);
    _delay_ms( 1000 );
    
    /* **** **** **** **** **** **** **** **** **** */
    
    const uint8_t from  = 0;
    const uint8_t to    = 100;
    const uint8_t steps = 100;
    
    const uint8_t dV = (to-from)/steps;
    
    uint8_t curent = from;
    
    
    for (int i =0; i<steps ; i++)
    {
        
        for (int x = 0; x<X_TLC_MAX; x++)
        {
            
            for (int y = 0; y<Y_MIC_MAX; y++)
            {
                const uint8_t val = jean_name[x + 30*y];

                if ( ( _display.buff_A[y][x] <= 150 ) && ( val ) )
                    display_setPixel( &_display, x, y, curent );
   
            }
        }
        
        _delay_ms(5);
        
        curent+= dV;
    }
    
    /**/

    display_setPixel( &_display, 29, 29, 0);
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

int main( void )
{
    /* Hardware configuration */
    
    display_init( &_display);
    
    sensors_init( &_sensors);

    sei();

    /**/
    /*
    display_writeImage( &_display, catImage );
    
    _delay_ms( 1000 );
    */
    /**/
    
    //testAllLetters();
    
    /**/
    
    display_clear( &_display );


    
    for (;;)
    {
//        writeNext();
     //   bigTypo();
        test2();
        _delay_ms(2000);
    }
    
/*
    uint8_t ldr_index = MIC_SENCOR_COUNT;
    
    float prevVal = 0.f;
    float dVal    =0.f;
    
    uint8_t debouceMax  = 2;
    static uint8_t debounceCount = 0;

    for (;;)
    {
        if ( ldr_index == MIC_SENCOR_COUNT)
        {
            setHigh( LDR_DATA_PORT , LDR_DATA_PIN );
            ldr_index = 0;
            
            // matrice entiere
            
            dVal /= MIC_SENCOR_COUNT*SENSOR_COUNT;
            
            float res = fabs( dVal -prevVal );
            
            
            
            if ((res >= 10) )
            {
                if ( debounceCount >= debouceMax)
                {
                    writeNext();
                    
                    _display.buff_A[29][29] = 255;
                    _delay_ms(100);
                    _display.buff_A[29][29] = 0;
                    
                    debounceCount = 0;
                }
                else
                    debounceCount++;
                

            }
            
            prevVal = dVal;
        }

        else
            setLow( LDR_DATA_PORT , LDR_DATA_PIN );
        
        
        pulse( LDR_CLOCK_PORT , LDR_CLOCK_PIN);
        
        for (int i=0; i< SENSOR_COUNT ; i++)
        {
            // ldr 0 -> pix0 + pix 1
            // ldr 1 -> pix2 + pix 3
            // ldr 2 -> pix4 + pix 5
            // ldr i -> pix i*2 + pix (i*2)+1
            
            //const uint8_t index = ldr_index*2;
            
            //mat_sensors[i] = (uint8_t) adc_read(i);
            
            dVal += adc_read(i);

        }

        

        
        _delay_ms(20);
        
        ldr_index++;
        
    } // end endless for
    


*/
    
    return 0;
    
}





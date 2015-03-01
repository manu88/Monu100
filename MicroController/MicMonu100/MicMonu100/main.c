
#include <stdlib.h> 
#include <stdio.h>

#include <avr/io.h>

#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <util/delay.h>
#include <math.h>

#include "PinsConfig.h"


#include "Display.h"



#include "Images.h"

/* **** **** **** **** **** **** **** **** **** **** **** **** */
/*
    Global constants defines
 */



#define SENSOR_COUNT     15
#define MIC_SENCOR_COUNT 15

Display _display;

uint8_t mat_sensors[ SENSOR_COUNT ] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

// initialize adc
void adc_init(void)
{
    // AREF = AVcc
    ADMUX = (1<<REFS0);
    
    // ADC Enable and prescaler of 128
    // 16000000/128 = 125000
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

 inline uint16_t adc_read(uint8_t ch)
{
    // select the corresponding channel 0~7
    // ANDing with ’7′ will always keep the value
    // of ‘ch’ between 0 and 7
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

void writeNext(void)
{
    const  uint8_t count = 2;
    static uint8_t index=0;

    display_clear( &_display);
    
    if (index == 0)
    {

        display_write( &_display,"Michel\nDurand",0,0);
        
        _display.fillColor = 0b10101010;
        display_fillZone( &_display, 5, 22, 5, 5);
    }
    
    else if (index == 1)
    {

        display_write( &_display,"chelMi\nranDdu", 0,0);
        
        _display.fillColor = 0b00010000;
        display_fillZone( &_display, 5, 22, 5, 5);
    }
//        writeName2();
    
    index++;
    
  
    
    if (index == count)
        index = 0;
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

int main( void )
{
    /* Hardware configuration */
    
    display_init( &_display);
    
    /* sensor array */
    
    adc_init();
    

    
    // MICs LDR

    setOutput( LDR_DATA_DDR       , LDR_DATA_PIN ); // data
    setOutput( LDR_STROBE_DDR     , LDR_STROBE_PIN ); // strobe
    setOutput( LDR_OUT_ENABLE_DDR , LDR_OUT_ENABLE_PIN ); // out enable
    setOutput( LDR_CLOCK_DDR      , LDR_CLOCK_PIN ); // clock

    
    setHigh( MIC_STROBE_PORT , MIC_STROBE_PIN ); // strobe mic leds
    setHigh( LDR_STROBE_PORT , LDR_STROBE_PIN ); // strobe mic LDR
    
    
    // reset mic ldr
    
    setLow( LDR_DATA_PORT , LDR_DATA_PIN ); // data to low
    
    for (int i =0;i< 16;i++)
        pulse( LDR_CLOCK_PORT , LDR_CLOCK_PIN );
    

    /* sensor array */
    
    sei();


    /* **** Splash wait **** */

   display_writeImage( &_display, catImage);

    
    _delay_ms(1000);

    /* **** END OF Splash wait **** */

    /* WRITE CAN ID */
    
    display_clear( &_display);
    
    display_write( &_display,"can ID", 0, 0);
    
    char str[3];
    sprintf(str, "%i" , 16);
    display_write( &_display,str, 0, 8);
    
    /* END OF WRITE CAN ID */
    
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
            
            
            
            if ((res >= 100) )
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
            
            /*
            pixels[i][index] = mat_sensors[i];
            pixels[i][index+1] = mat_sensors[i];
             */
        }

        

        
        _delay_ms(20);
        
        ldr_index++;
        
    } // end endless for
    
    /*
     _delay_ms(80);
     //        translateBuffer(dir);
     i++;
     
     if (i>X_TLC_MAX)
     {
     dir*=-1;
     i=0;
     }
     */
    
    
    return 0;
    
}





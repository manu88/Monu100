
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

/* **** **** **** **** **** **** **** **** **** **** **** **** */




/* **** **** **** **** **** **** **** **** **** **** **** **** */

uint8_t mat_sensors[ SENSOR_COUNT ] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

/* **** **** **** **** **** **** **** **** **** **** **** **** */
// move to Display.h
void translateBuffer( const uint8_t dX)
{

    for (int i = X_TLC_MAX ;i>=0;i--)
    {
        
        const int pos = i+dX;//(i+dX) %X_TLC_MAX;

        for (int j=0;j<Y_MIC_MAX;j++)
        {
            _display.buff_A[pos][j] = _display.buff_A[i][j];
        }

            
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void TLC5940_Init(void)
{

    setOutput(SCLK_DDR, SCLK_PIN);

    setOutput(XLAT_DDR, XLAT_PIN);
    setOutput(BLANK_DDR, BLANK_PIN);
    setOutput(SIN_DDR, SIN_PIN);

    setLow(SCLK_PORT, SCLK_PIN);

    setLow(XLAT_PORT, XLAT_PIN);
    
    setHigh(BLANK_PORT, BLANK_PIN);
    
    // Enable SPI, Master, set clock rate fck/2
    SPCR = (1 << SPE) | (1 << MSTR);
    SPSR = (1 << SPI2X);
    
    // CTC with OCR0A as TOP
    TCCR0A = (1 << WGM01);
    // clk_io/1024 (From prescaler)
    TCCR0B = ((1 << CS02) | (1 << CS00));
    // Generate an interrupt every 4096 clock cycles
    OCR0A = 3; // 3
    // Enable Timer/Counter0 Compare Match A interrupt
    TIMSK0 |= (1 << OCIE0A);
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/*
 
 ADC part
 
 */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
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

inline void sendSPI( const uint8_t val)
{
    SPDR = val;
    while (!(SPSR & (1 << SPIF)));
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
// display's interupt

ISR(TIMER0_COMPA_vect)
{
    static uint16_t rowIndex = Y_MIC_MAX;
    uint8_t firstCycleFlag = 1;// 0;
    static uint8_t xlatNeedsPulse = 0;
    
    
    
    if ( rowIndex == Y_MIC_MAX ) // matrix ok. return col0;
    {
        setHigh( DATA0_PORT , DATA0_PIN);
        setHigh( DATA1_PORT , DATA1_PIN);
        rowIndex = 0;
        
    }
    else
    {
        setLow( DATA0_PORT , DATA0_PIN);
        setLow( DATA1_PORT , DATA1_PIN);
    }
    
    
    setLow(BLANK_PORT, BLANK_PIN);

    
    /**** FILL TLC BUFFERS *****/
    
    // col 0 -> send dumm values ( not displayed)
    sendSPI(0b00000000); // p0
    
    for (int x=0;x<14;x+=2)
    {
        const uint8_t pixA = _display.buff_A/* pixels*/[x][rowIndex];
        const uint8_t pixB =  _display.buff_A/*pixels*/[x+1][rowIndex];

        sendSPI( pixA >> 4); // p1
        sendSPI( (uint8_t )(pixA << 4) ); // p1
        sendSPI( pixB ); // p2
    }

    //  col 15 a la mano
    sendSPI( _display.buff_A/* pixels*/[14][rowIndex] >> 4); // p15
    sendSPI( (uint8_t )(  _display.buff_A/*pixels*/[14][rowIndex] << 4) ); // p15
    
    /**/
    
    // col 16 -> send dumm values ( not displayed)
    sendSPI(0b00000000); // p16
    
    for (int x = 15;x<28;x+=2)
    {
        const uint8_t pixA =  _display.buff_A/*pixels*/[x][rowIndex];
        const uint8_t pixB =  _display.buff_A/*pixels*/[x+1][rowIndex];
        
        sendSPI( pixA >> 4); // p1
        sendSPI( (uint8_t )(pixA<< 4) ); // p1
        sendSPI( pixB ); // p2
    }

    // col 29 a la mano
    sendSPI(  _display.buff_A/*pixels*/[29][rowIndex] >> 4); // p15
    sendSPI( (uint8_t )( _display.buff_A/*pixels*/[29][rowIndex] << 4) ); // p15

    /***** END OF FILL TLC BUFFERS ****/


    xlatNeedsPulse = 1;

    pulse( MIC_CLOCK_PORT , MIC_CLOCK_PIN );
    
    /**/
    // additionnal pulse for non-wired outs of mics.
    
    if ( (rowIndex == 15) ||
        (rowIndex == 22)
        )
    {
        setLow( DATA0_PORT , DATA0_PIN);
        setLow( DATA1_PORT , DATA1_PIN);

        pulse( MIC_CLOCK_PORT , MIC_CLOCK_PIN );
    }
    
    /**/
    
    setHigh(BLANK_PORT, BLANK_PIN);
    
    
    if (xlatNeedsPulse)
    {
        pulse(XLAT_PORT, XLAT_PIN);
        xlatNeedsPulse = 0;
        
        
    }
    
    if (firstCycleFlag)
    {
        pulse(SCLK_PORT, SCLK_PIN);
        
        
    }


    rowIndex++;
    

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
    
    adc_init();
    
    // init MICs leds
    setOutput( DATA0_DDR , DATA0_PIN         ); // data row1
    setOutput( DATA1_DDR , DATA1_PIN         ); // data row2
    setOutput( MIC_CLOCK_DDR , MIC_CLOCK_PIN ); // clock
    
    setOutput( MIC_STROBE_DDR , MIC_STROBE_PIN ); // strobe
    
    setOutput( MIC_OUT_ENABLE_DDR , MIC_OUT_ENABLE_PIN ); // utile?
    
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
    
    
    // reset mic buffers
    

    setLow( DATA0_PORT , DATA0_PIN); // val 0 prévue -> pulse après écrit la valeur dans le buffer
    setLow( DATA1_PORT , DATA1_PIN);
    
    for (int i=0; i< Y_MIC_MAX ; i++)
    {
        pulse( MIC_CLOCK_PORT , MIC_CLOCK_PIN );
        
    }
    
    

    display_init( &_display);

    TLC5940_Init();
    
    /* Vars configuration */    
    
    

    



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

        

     //   fontColor -=2; //(255 - mat_sensors[0]);
       // initPixels();
        

        
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





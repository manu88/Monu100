#include <avr/io.h>

#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <util/delay.h>

#include "PinsConfig.h"
#include "Square15.h"
#include "Chars.h"

/* **** **** **** **** **** **** **** **** **** **** **** **** */
/*
    Global constants defines
 */

#define X_TLC_MAX 30
#define Y_MIC_MAX 30

#define SENSOR_COUNT     15
#define MIC_SENCOR_COUNT 15

/* **** **** **** **** **** **** **** **** **** **** **** **** */

uint8_t pixels[ X_TLC_MAX ][ Y_MIC_MAX ];

volatile uint8_t backgroundColor = 0b00000011;
volatile uint8_t fontColor       = 0b11111111;

/* **** **** **** **** **** **** **** **** **** **** **** **** */

uint8_t mat_sensors[ SENSOR_COUNT ] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

/* **** **** **** **** **** **** **** **** **** **** **** **** */

void translateBuffer( const uint8_t dX)
{
    for (int i = X_TLC_MAX ;i>=0;i--)
    {
        
        const int pos = i+dX;//(i+dX) %X_TLC_MAX;

        for (int j=0;j<Y_MIC_MAX;j++)
        {
            pixels[pos][j] = pixels[i][j];
        }

            
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** */

void writeLetter(const uint8_t *letter , const uint8_t xPos , const uint8_t yPos)
{
    for (int i= 0;i<CHAR_HEIGHT ;i++)
    {
        
        for (int j= 0;j<CHAR_WIDTH ;j++)
        {
            if ( letter[i] & (1<<(7-j) ))
                pixels[xPos+i][yPos+j] = fontColor;
            else
                pixels[xPos+i][yPos+j] = backgroundColor;
        }
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** */

void writeSquareLetter(const uint8_t *letter , const uint8_t xPos , const uint8_t yPos)
{
    for (int i= 0;i<SQUARE_HEIGHT ;i++)
    {
        
        for (int j= 0;j<SQUARE_WIDTH ;j++)
        {
            if ( letter[i] & (1<<(7-j) ))
                pixels[xPos+i][yPos+j] = fontColor;
            else
                pixels[xPos+i][yPos+j] = backgroundColor;
        }
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** */

void initPixels(void)
{

    for (int x = 0;x< X_TLC_MAX;x++)
    {
        for( int y = 0; y<Y_MIC_MAX;y++)
        {

            pixels[x][Y_MIC_MAX-y-1] = backgroundColor;

            
        }

    }
    

    writeLetter( letter_M, 0, 0);
    writeLetter( letter_i, 0, CHAR_WIDTH);
    writeLetter( letter_c, 0, CHAR_WIDTH*2);
    writeLetter( letter_h, 0, CHAR_WIDTH*3);
    writeLetter( letter_e, 0, CHAR_WIDTH*4);
    writeLetter( letter_l, 0, CHAR_WIDTH*5);
    
    writeLetter( letter_D, CHAR_HEIGHT, 0);
    writeLetter( letter_u, CHAR_HEIGHT, CHAR_WIDTH);
    writeLetter( letter_r, CHAR_HEIGHT, CHAR_WIDTH*2);
    writeLetter( letter_a, CHAR_HEIGHT, CHAR_WIDTH*3);
    writeLetter( letter_n, CHAR_HEIGHT, CHAR_WIDTH*4);
    writeLetter( letter_d, CHAR_HEIGHT, CHAR_WIDTH*5);
    
    writeSquareLetter(square_a, 18, SQUARE_WIDTH*0);
    writeSquareLetter(square_b, 18, SQUARE_WIDTH*1);
    writeSquareLetter(square_c, 18, SQUARE_WIDTH*2);
    writeSquareLetter(square_d, 18, SQUARE_WIDTH*3);
    writeSquareLetter(square_e, 18, SQUARE_WIDTH*4);
    writeSquareLetter(square_f, 18, SQUARE_WIDTH*5);
    writeSquareLetter(square_g, 18, SQUARE_WIDTH*6); // lim
    
    writeSquareLetter(square_h, 24, SQUARE_WIDTH*0);
    writeSquareLetter(square_i, 24, SQUARE_WIDTH*1);
    writeSquareLetter(square_j, 24, SQUARE_WIDTH*2);
    writeSquareLetter(square_k, 24, SQUARE_WIDTH*3);
    writeSquareLetter(square_l, 24, SQUARE_WIDTH*4);
    writeSquareLetter(square_m, 24, SQUARE_WIDTH*5);
    writeSquareLetter(square_n, 24, SQUARE_WIDTH*6); // lim
    
    
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

uint16_t adc_read(uint8_t ch)
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
        const uint8_t pixA = pixels[x][rowIndex];
        const uint8_t pixB = pixels[x+1][rowIndex];

        sendSPI( pixA >> 4); // p1
        sendSPI( (uint8_t )(pixA << 4) ); // p1
        sendSPI( pixB ); // p2
    }

    //  col 15 a la mano
    sendSPI( pixels[14][rowIndex] >> 4); // p15
    sendSPI( (uint8_t )(pixels[14][rowIndex] << 4) ); // p15
    
    /**/
    
    // col 16 -> send dumm values ( not displayed)
    sendSPI(0b00000000); // p16
    
    for (int x = 15;x<28;x+=2)
    {
        const uint8_t pixA = pixels[x][rowIndex];
        const uint8_t pixB = pixels[x+1][rowIndex];
        
        sendSPI( pixA >> 4); // p1
        sendSPI( (uint8_t )(pixA<< 4) ); // p1
        sendSPI( pixB ); // p2
    }

    // col 29 a la mano
    sendSPI( pixels[29][rowIndex] >> 4); // p15
    sendSPI( (uint8_t )(pixels[29][rowIndex] << 4) ); // p15

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
    
    for (int i =0;i< MIC_SENCOR_COUNT;i++)
        pulse( LDR_CLOCK_PORT , LDR_CLOCK_PIN );
    
    
    // reset mic buffers
    

    setLow( DATA0_PORT , DATA0_PIN); // val 0 prévue -> pulse après écrit la valeur dans le buffer
    setLow( DATA1_PORT , DATA1_PIN);
    
    for (int i=0; i< Y_MIC_MAX ; i++)
    {
        pulse( MIC_CLOCK_PORT , MIC_CLOCK_PIN );
        
    }
    
    
    setHigh( LDR_DATA_PORT , LDR_DATA_PIN );
    pulse( LDR_CLOCK_PORT , LDR_CLOCK_PIN);

    TLC5940_Init();
    
    /* Vars configuration */    
    
    
    initPixels();


    sei();



    for (;;)
    {

        
        for (int i=0; i< SENSOR_COUNT ; i++)
        {
            mat_sensors[i] = (uint8_t) adc_read(i);
            
        }

        

     //   fontColor -=2; //(255 - mat_sensors[0]);
       // initPixels();
        
        _delay_ms(80);
    }
    
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





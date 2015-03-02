//
//  Display.c
//  MicMonu100
//
//  Created by Manuel Deneu on 01/03/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "PinsConfig.h"

#include "Display.h"

#include "Chars.h"


extern Display _display;

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
    
    _display.isDrawing = 1;
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
    
    _display.isDrawing = 0;
    
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

void TLC5940_Init(void)
{
    // init MICs leds
    setOutput( DATA0_DDR , DATA0_PIN         ); // data row1
    setOutput( DATA1_DDR , DATA1_PIN         ); // data row2
    setOutput( MIC_CLOCK_DDR , MIC_CLOCK_PIN ); // clock
    
    setOutput( MIC_STROBE_DDR , MIC_STROBE_PIN ); // strobe
    
    setOutput( MIC_OUT_ENABLE_DDR , MIC_OUT_ENABLE_PIN ); // utile?
    
    
    // reset mic buffers
    
    
    setLow( DATA0_PORT , DATA0_PIN); // val 0 prévue -> pulse après écrit la valeur dans le buffer
    setLow( DATA1_PORT , DATA1_PIN);
    
    for (int i=0; i< Y_MIC_MAX ; i++)
    {
        pulse( MIC_CLOCK_PORT , MIC_CLOCK_PIN );
        
    }
    
     setHigh( MIC_STROBE_PORT , MIC_STROBE_PIN ); // strobe mic leds

    
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
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

inline uint8_t clipVal( uint8_t val)
{
    return val>=PIXEL_MAX_VALUE ? PIXEL_MAX_VALUE : val;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */



void display_init( Display *display)
{
    display_clear( display);

    display->pos.x = 0;
    display->pos.y = 0;
    
    display->backgroundColor = 0b00000000;
    display->fontColor = PIXEL_MAX_VALUE;
    display->fillColor = PIXEL_MAX_VALUE;
    
    display->isDrawing = 0;
    
    TLC5940_Init();    
}

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_swapbuffers( Display *display)
{
    
}

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_setNeedsUpdate( Display *display)
{
    display->needsDisplay+=(1 << 0);
}

/* **** **** **** **** **** **** **** **** **** **** **** */

uint8_t display_needsUpdate( Display *display)
{
    return display->needsDisplay & 1 <<0;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void display_setFontColor( Display *display ,uint8_t color)
{
    display->fontColor = clipVal(color);
}

void display_setFillColor( Display *display ,uint8_t color)
{
    display->fillColor = clipVal( color);
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void display_clearZone( Display *display , const uint8_t x , const uint8_t y, const uint8_t w , const uint8_t h )
{
    if (display->isDrawing == 1)
        return;
    
    
// inv x<->y
    for (uint8_t xx=0; xx< w; xx++)
        
        for (uint8_t yy=0; yy<h; yy++)
            
            display->buff_A[y+yy][x+xx] = display->backgroundColor;
}

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_clear( Display *display)
{
    display_clearZone( display ,0,0 , X_TLC_MAX ,Y_MIC_MAX  );

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void display_translate( Display *display , int8_t dX , int8_t dY)
{
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void display_write(Display *display , const char* text, uint8_t x , uint8_t y)
{
    if (display->isDrawing == 1)
        return;
    
    // inv x<->y
    int i =0;
    
    
    uint8_t xx =y;
    uint8_t yy =x;
    
    while ( text[i] != '\0' )
    {
        if ( text[i] == '\n' )
        {
            xx+= CHAR_HEIGHT;
            yy = x;
        }
        
        else if ( text[i] == ' ' )
            yy+=CHAR_WIDTH;
        
        else
        {
            const Glyphe* ch = getChar( text[i] );
            const uint8_t* buf = ch->buf;
            
            for (int i= 0;i<CHAR_HEIGHT ;i++)
            {
                
                for (int j= 0;j<ch->advance ;j++)
                {
                    if ( buf[i] & (1<<(7-j) ))
                        display->buff_A[xx+i][yy+j] = display->fontColor;

                }
            }
            
            //
            
            yy+=ch->advance+1;
        }
        i++;
        
        
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_writeImage( Display *display,const  uint8_t *image )
{
    if (display->isDrawing == 1)
        return;
    
    for (int x = 0; x<X_TLC_MAX;x++)
    {
        for (int y = 0; y<Y_MIC_MAX;y++)
        {
            display->buff_A[x][y] = clipVal( image[y + x*30] );
        }
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_fillZone  ( Display *display , const uint8_t x , const uint8_t y, const uint8_t w , const uint8_t h )
{
    /*
    if (display->isDrawing == 1)
        return;
    */
    
    // inv x<->y
    for (uint8_t xx=0; xx< w; xx++)
        
        for (uint8_t yy=0; yy<h; yy++)
            
            display->buff_A[y+yy][x+xx] = display->fillColor;
}

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_setPixel( Display *display , const uint8_t x , const uint8_t y, const uint8_t value)
{
    if (display->isDrawing == 1)
        return;
    
    // inversion x<->y
    if ( (y < X_TLC_MAX) && ( x <Y_MIC_MAX) )
        display->buff_A[y][x] = clipVal( value );
}

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_addPixel( Display *display , const uint8_t x , const uint8_t y, const uint8_t value)
{
    // inversion x<->y
    if ( (y < X_TLC_MAX) && ( x <Y_MIC_MAX) )
        display->buff_A[y][x] = clipVal( value );
}



/* **** **** **** **** **** **** **** **** **** **** **** */





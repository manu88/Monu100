//
//  Display.c
//  MicMonu100
//
//  Created by Manuel Deneu on 01/03/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//
#include <stdio.h>
#include <avr/io.h>
#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include <util/delay.h>
#include "MCP2515.h"
#include "PinsConfig.h"

#include "Display.h"

#include "Chars.h"


extern Display _display;

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

uint8_t getCharWidth()
{
    return CHAR_WIDTH;
}
uint8_t getCharHeight()
{
    return CHAR_HEIGHT;
}

inline uint8_t clipVal( uint8_t val)
{
    return val>=PIXEL_MAX_VALUE ? PIXEL_MAX_VALUE : val;
}

inline uint8_t getPixel( const uint8_t x , const uint8_t y)
{
    return _display.buff_text[x][y] +_display.buff_draw[x][y] ;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
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
    static uint8_t firstCycleFlag = 1;// 0;
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
    
    for (int x=0;x<14;x+=2)
    {
        const uint8_t pixA = getPixel(x   , rowIndex);
        const uint8_t pixB = getPixel(x+1 , rowIndex);

        sendSPI( pixA >> 4); // p1
        sendSPI( (uint8_t )(pixA << 4) ); // p1
        sendSPI( pixB ); // p2

    }
    
    //  col 15 a la mano
    {
    const uint8_t  p = getPixel(14, rowIndex);
    sendSPI( p >> 4 ); // p15
    sendSPI( p << 4 ); // p15
    }
    /**/
    
    // col 16 -> send dumm values ( not displayed)
    sendSPI(0b00000000); // p16
    
    for (int x = 15;x<28;x+=2)
    {
        const uint8_t pixA =  getPixel(x   , rowIndex);
        const uint8_t pixB =  getPixel(x+1 , rowIndex);
        
        sendSPI( pixA >> 4); // p1
        sendSPI( (uint8_t )(pixA<< 4) ); // p1
        sendSPI( pixB ); // p2
    }
    
    // col 29 a la mano
    {
        const uint8_t p = getPixel(29,rowIndex);
    sendSPI(  p >> 4 ); // p15
    sendSPI(  p << 4 ); // p15
    }
    /***** END OF FILL TLC BUFFERS ****/
    
    _display.isDrawing = 0;
    
    xlatNeedsPulse = 1;
    
    pulse( MIC_CLOCK_PORT , MIC_CLOCK_PIN );
    
    
    /**/
    // additionnal pulse for non-wired outs of mics.
    /*
    if ( (rowIndex == 15) ||
         (rowIndex == 22)
        )
    {
        setLow( DATA0_PORT , DATA0_PIN);
        setLow( DATA1_PORT , DATA1_PIN);
        
        pulse( MIC_CLOCK_PORT , MIC_CLOCK_PIN );
        
    }
    */
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

        firstCycleFlag = 0;
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
    
    
    setLow(MIC_OUT_ENABLE_PORT, MIC_OUT_ENABLE_PIN);
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



void display_init( Display *display)
{
    display_clearAll( display);


    
    display->backgroundColor = 0b00000000;
    display->fontColor = PIXEL_MAX_VALUE;
    display->fillColor = PIXEL_MAX_VALUE;
    
    display->isDrawing = 0;
    
    TLC5940_Init();    
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


void display_clearZoneDraw     ( Display *display , const int8_t x , const int8_t y, const uint8_t w , const uint8_t h )
{
    for (uint8_t xx=0; xx< w; xx++)
        
        for (uint8_t yy=0; yy<h; yy++)
        {
            const int8_t defX =x+xx;
            const int8_t defY =y+yy;
            
            if( inBouds(defX, defY) )
            {
                display->buff_draw[defX][defY] = display->backgroundColor;
            }
        }
}

void display_clearZoneText     ( Display *display , const int8_t x , const int8_t y, const uint8_t w , const uint8_t h )
{
    for (uint8_t xx=0; xx< w; xx++)
        
        for (uint8_t yy=0; yy<h; yy++)
        {
            const int8_t defX =x+xx;
            const int8_t defY =y+yy;
            
            if( inBouds(defX, defY) )
            {
                display->buff_text[defX][defY] = display->backgroundColor;
            }
        }
}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void display_translate( Display *display , int8_t dX , int8_t dY)
{
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void display_write(Display *display , const char* text, int8_t x , int8_t y ,
                   uint8_t dir /* 0 : hori , 1 : verti*/,
                   uint8_t mask /* 0 : none , 1 left , 2 right*/)
{
    // inv x<->y
    int i =0;
    
    
    int8_t xx =y;
    int8_t yy =x;
    
    while ( text[i] != '\0' )
    {
        if ( !dir && ( text[i] == '\n' ) )
        {
            xx+= CHAR_HEIGHT;
            yy = x;
        }
        
        else if ( text[i] == ' ' )
        {
            if (!dir)
                yy+=CHAR_WIDTH;
            else
                xx+=CHAR_HEIGHT;
        }
        
        else
        {
            const Glyphe* ch = getChar( text[i] );
            const uint8_t* buf = ch->buf;
            
            for (int i= 0;i<CHAR_HEIGHT ;i++)
            {
                
                for (int j= 0;j<ch->advance ;j++)
                {
                    if ( buf[i] & (1<<(7-j) ))
                    {
                        const int8_t defX = xx+i;
                        const int8_t defY = yy+j;
                        
                        // ][ Y_MIC_MAX
                        if( inBouds(defX, defY) )
                        {
                            if (    (mask == 0)
                                || ((mask == 1 ) && (j<4))
                                || ((mask == 2 ) && (j>3))
                                )
                                display->buff_text[ defX ][ defY ] = display->fontColor;
                        }
                    }
                    
                }
            }
            
            //
            if( dir == 0 )
                yy+=ch->advance;
            else
                xx+= CHAR_HEIGHT;
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
            display->buff_draw[x][y] = clipVal( image[y + x*X_TLC_MAX] );
        }
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_fillZone  ( Display *display , const int8_t x , const int8_t y, const int8_t w , const int8_t h )
{
    /*
    if (display->isDrawing == 1)
        return;
    */
    
    // inv x<->y
    for (uint8_t xx=0; xx< w; xx++)
        
        for (uint8_t yy=0; yy<h; yy++)
        {
            const int8_t dX = y+yy;
            const int8_t dY = x+xx;
            
            if( inBouds(dX, dY))
                display->buff_draw[dX][dY] = display->fillColor;
        }
}

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_setPixel( Display *display , const uint8_t x , const uint8_t y, const uint8_t value)
{
//    if (display->isDrawing == 1)
//        return;
    
    // inversion x<->y
    if ( (y < X_TLC_MAX) && ( x <Y_MIC_MAX) )
        display->buff_draw[y][x] = clipVal( value );
}

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_addPixel( Display *display , const uint8_t x , const uint8_t y, const uint8_t value)
{
    // inversion x<->y
    if ( (y < X_TLC_MAX) && ( x <Y_MIC_MAX) )
        display->buff_draw[y][x] = clipVal( value );
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

/*
 
    Drawing routines
 
 */
/* **** **** **** **** **** **** **** **** **** **** **** */

void display_drawCircle( Display *display , const uint8_t x0 , const uint8_t y0 , const uint8_t r)
{
    int x = r;
    int y = 0;
    int decisionOver2 = 1 - x;   // Decision criterion divided by 2 evaluated at x=r, y=0
    
    while(x >= y)
    {
        display_setPixel( display , x + x0,  y + y0  , display->fillColor);
        display_setPixel( display , x + x0,  y + y0  , display->fillColor);
        display_setPixel( display , y + x0,  x + y0  , display->fillColor);
        display_setPixel( display , -x + x0,  y + y0 , display->fillColor);
        display_setPixel( display , -y + x0,  x + y0 , display->fillColor);
        display_setPixel( display , -x + x0, -y + y0 , display->fillColor);
        display_setPixel( display , -y + x0, -x + y0 , display->fillColor);
        display_setPixel( display ,  x + x0, -y + y0 , display->fillColor);
        display_setPixel( display ,  y + x0, -x + y0 , display->fillColor);
        y++;
        
        if (decisionOver2<=0)
        {
            decisionOver2 += 2 * y + 1;   // Change in decision criterion for y -> y+1
        }
        else
        {
            x--;
            decisionOver2 += 2 * (y - x) + 1;   // Change for y -> y+1, x -> x-1
        }
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_drawLine( Display *display , uint8_t x0 , uint8_t y0 , uint8_t x1 , uint8_t y1 )
{
    int dx = abs(x1-x0), sx = x0<x1 ? 1 : -1;
    int dy = abs(y1-y0), sy = y0<y1 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;
    
    for(;;)
    {
        display_setPixel( display , x0,  y0  , display->fillColor);

        if (x0==x1 && y0==y1)
            break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}



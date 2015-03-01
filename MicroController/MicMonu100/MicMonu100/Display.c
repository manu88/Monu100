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

#include "Display.h"

#include "Chars.h"

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_init( Display *display)
{
    display_clear( display);

    display->pos.x = 0;
    display->pos.y = 0;
    
    display->backgroundColor = 0b00000000;
    display->fontColor = 0b11111111;
    display->fillColor = 0b11110000;
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

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_clearZone( Display *display , const uint8_t x , const uint8_t y, const uint8_t w , const uint8_t h )
{
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

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_write(Display *display , const char* text, uint8_t x , uint8_t y)
{
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
            const uint8_t* ch = getChar( text[i] );
            //writeLetter( ch, /*xPOs*/xx,/*yPos*/ yy);
            
            for (int i= 0;i<CHAR_HEIGHT ;i++)
            {
                
                for (int j= 0;j<CHAR_WIDTH ;j++)
                {
                    if ( ch[i] & (1<<(7-j) ))
                        display->buff_A[xx+i][yy+j] = display->fontColor;
                    /*
                     else
                     _display.buff_A[xx+i][yy+j] = backgroundColor;
                     */
                }
            }
            
            //
            
            yy+=CHAR_WIDTH;
        }
        i++;
        
        
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_writeImage( Display *display,const  uint8_t *image )
{
    for (int x = 0; x<X_TLC_MAX;x++)
    {
        for (int y = 0; y<Y_MIC_MAX;y++)
        {
            display->buff_A[x][y] = image[y + x*30];
        }
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_fillZone  ( Display *display , const uint8_t x , const uint8_t y, const uint8_t w , const uint8_t h )
{
    // inv x<->y
    for (uint8_t xx=0; xx< w; xx++)
        
        for (uint8_t yy=0; yy<h; yy++)
            
            display->buff_A[y+yy][x+xx] = display->fillColor;
}

/* **** **** **** **** **** **** **** **** **** **** **** */

void display_setPixel( Display *display , const uint8_t x , const uint8_t y, const uint8_t value)
{
    // inversion x<->y
    if ( (y < X_TLC_MAX) && ( x <Y_MIC_MAX) )
        display->buff_A[y][x] = value;
}

/* **** **** **** **** **** **** **** **** **** **** **** */




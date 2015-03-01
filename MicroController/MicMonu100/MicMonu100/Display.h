//
//  Display.h
//  MicMonu100
//
//  Created by Manuel Deneu on 01/03/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#ifndef MicMonu100_Display_h
#define MicMonu100_Display_h


#define X_TLC_MAX 30
#define Y_MIC_MAX 30

struct _Point
{
    uint8_t x;
    uint8_t y;
};

typedef struct _Point Point;


/* **** **** **** **** **** **** **** **** **** **** **** */
/*
 
 inversion x<->y entre le buffer et la matrice :
    Pour l'instant l'inversion est compensée dans l'interface ( fonctions display_*** )
 */
struct _Display
{
    uint8_t buff_A[ X_TLC_MAX ][ Y_MIC_MAX ]; // -> to display
    uint8_t buff_B[ X_TLC_MAX ][ Y_MIC_MAX ]; // -> image is constructed in this one

    Point pos; // -> drawing pos


    
    /* This color will be used when clearing the display */
    uint8_t backgroundColor;
    
    uint8_t fillColor;
    
    uint8_t fontColor;
    
    
    uint8_t needsDisplay; // byte 0
    
    /* This flag will be set to 1 during the drawing interupt */
    volatile uint8_t isDrawing;
    
};

typedef struct _Display Display;



void display_init( Display *display);

void display_swapbuffers   ( Display *display);

void display_setNeedsUpdate( Display *display);
uint8_t display_needsUpdate( Display *display);

// inv x<->y
void display_clearZone     ( Display *display , const uint8_t x , const uint8_t y, const uint8_t w , const uint8_t h );

void display_clear         ( Display *display);

void display_translate( Display *display , int8_t dX , int8_t dY);

/* Drawing ops */

// inv x<->y
void display_write     ( Display *display , const char* text, uint8_t x , uint8_t y);

void display_writeImage( Display *display , const uint8_t *image );

// inv x<->y
void display_fillZone  ( Display *display , const uint8_t x , const uint8_t y, const uint8_t w , const uint8_t h );

// inv x<->y 
void display_setPixel( Display *display , const uint8_t x , const uint8_t y, const uint8_t value);




#endif

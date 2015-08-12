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
#define Y_MIC_MAX 20

#define PIXEL_MAX_VALUE  100 //112




/* **** **** **** **** **** **** **** **** **** **** **** */
/*
 
 inversion x<->y entre le buffer et la matrice :
    Pour l'instant l'inversion est compensée dans l'interface ( fonctions display_*** )
 */
struct _Display
{
    uint8_t buff_text[ X_TLC_MAX ][ Y_MIC_MAX ];
    uint8_t buff_draw[ X_TLC_MAX ][ Y_MIC_MAX ];



    /* This color will be used when clearing the display */
    uint8_t backgroundColor;
    uint8_t fillColor;
    uint8_t fontColor;
    
    
    uint8_t needsDisplay; // byte 0
    
    /* This flag will be set to 1 during the drawing interupt */
    volatile uint8_t isDrawing;
    
};

typedef struct _Display Display;

/* **** **** **** **** **** **** **** **** **** **** **** **** **** */

inline uint8_t inBouds( int8_t x , int8_t y)
{
    return    ( x >=0 && x <X_TLC_MAX )
           && ( y >=0 && y <Y_MIC_MAX );
}

//uint8_t strLen( const char *s);

uint8_t getCharWidth( void );
uint8_t getCharHeight( void );

void update(void);
/* **** **** **** **** **** **** **** **** **** **** **** **** **** */

void display_init( Display *display);

/* getters/setters */

void display_setFontColor( Display *display ,uint8_t color);
void display_setFillColor( Display *display ,uint8_t color);

// inv x<->y
void display_clearZoneDraw     ( Display *display , const int8_t x , const int8_t y, const uint8_t w , const uint8_t h );
void display_clearZoneText     ( Display *display , const int8_t x , const int8_t y, const uint8_t w , const uint8_t h );

inline void display_clearAll         ( Display *display)
{
    display_clearZoneDraw( display ,0,0 , X_TLC_MAX ,Y_MIC_MAX  );
    display_clearZoneText( display ,0,0 , X_TLC_MAX ,Y_MIC_MAX  );
}

inline void display_clearText( Display *display)
{
    display_clearZoneText( display ,0,0 , X_TLC_MAX ,Y_MIC_MAX  );
}

inline void display_clearDraw( Display *display)
{
    display_clearZoneDraw( display ,0,0 , X_TLC_MAX ,Y_MIC_MAX  );
}

void display_translate( Display *display , int8_t dX , int8_t dY);

/* Drawing ops */

// inv x<->y

/*
  x y can be <0 or > max , text will be croped
 */
void display_write     ( Display *display ,
                         const char* text,
                         int8_t x ,
                         int8_t y ,
                         uint8_t dir /* 0 : hori , 1 : verti*/ ,
                         uint8_t mask /* 0 : none , 1 left , 2 right*/
//                         int8_t charStart
);

void display_writeImage( Display *display , const uint8_t *image );

// inv x<->y
void display_fillZone  ( Display *display , const int8_t x , const int8_t y, const int8_t w , const int8_t h );

// inv x<->y 
void display_setPixel( Display *display , const uint8_t x , const uint8_t y, const uint8_t value);
void display_addPixel( Display *display , const uint8_t x , const uint8_t y, const uint8_t value);

void display_drawCircle( Display *display , const uint8_t x0 , const uint8_t y0 , const uint8_t r);
void display_drawLine( Display *display , uint8_t x0 , uint8_t y0 , uint8_t x1 , uint8_t y1 );

#endif

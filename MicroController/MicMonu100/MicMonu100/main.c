
#include <stdlib.h> 
#include <stdio.h>

#include <avr/io.h>

#define __PROG_TYPES_COMPAT__
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <math.h>
#include <string.h>

#include "PinsConfig.h"

#include "Sensors.h"

#include "Display.h"
//#include "Chars.h"
#include "Coms.h"

#include "mcp2515.h"
#include "CanMessages.h"
/*
 LEDS
 MIC 20 
 TLC 30
 
 CAPTEURS
 10 MIC * 15 ADC
 
 */


/* *** *** *** *** *** *** *** *** *** */

uint8_t canID = 0;

uint8_t offX = 0;
uint8_t offY = 0;

/* *** *** *** *** *** *** *** *** *** */

uint8_t errorCode = 0;
uint8_t mcusr_mirror __attribute__ ((section (".noinit")));

void get_mcusr(void) \
__attribute__((naked)) \
__attribute__((section(".init3")));

void get_mcusr(void)
{
    mcusr_mirror = MCUSR;
    MCUSR = 0;
    wdt_disable();
}

/* *** *** *** *** *** *** *** *** *** */

void initDip(void)
{
    DIP_DDR = 0x00;
    DIP_PORT =0xFF; // pullup
}

uint8_t readCanID(void)
{
    return (~PING) &0b00111111;
}

void computeGeometry(void)
{
    if( canID == 1)
    {
        offX = 0;
        offY = 0;
    }
    else if( canID == 2)
    {
        offX = 20;
        offY = 0;
    }
}

inline int getRealX( uint8_t x)
{
    return x - offX;
}

inline int getRealY( uint8_t y)
{
    return y - offY;
}

inline int getMainX(uint8_t x)
{
    return  x+offX;
}

inline int getMainY(uint8_t y)
{
    return  y+offY;
}

/* *** *** *** *** *** *** *** *** *** */

void sendState( void )
{
    CANMessage m;
    m.id = CAN_STATE_ID;
    m.length = 3;
    m.rtr = 0;
    m.data[0] = canID;
    m.data[1] = mcusr_mirror;
    m.data[2] = errorCode;
    can_send_message( &m);
}


/* *** *** *** *** *** *** *** *** *** */

Display _display;
Sensors _sensors;



void checkErrors(void)
{
    /*
     #define JTRF    4
     WDRF  bit  3 : WDRF: Watchdog System Reset Flag
     This bit is set if a Watchdog System Reset occurs. The bit is reset by a Power-on Reset, or by writing a logic zero to the flag.
     
     BORF  bit  2 : BORF: Brown-out Reset Flag
     This bit is set if a Brown-out Reset occurs. The bit is reset by a Power-on Reset, or by writing a logic zero to the flag.
     
     EXTRF bit 1  : EXT external Reset Flag
     This bit is set if an External Reset occurs. The bit is reset by a Power-on Reset, or by writing a logic zero to the flag.
     
     PORF bit  0 : PORF: Power-on Reset Flag
     This bit is set if a Power-on Reset occurs. The bit is reset only by writing a logic zero to the flag.
    */
}



/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

int16_t textX = 0;
int16_t textY = 0;

uint8_t sep = 0;

char text[16];

void updateScreen(void)
{
    display_clearDraw(&_display);
    display_clearText( &_display);

    display_setFillColor(&_display, 255);

    display_write(&_display, text, textX-sep, textY, 1 , 1);
    display_write(&_display, text, textX+sep, textY, 1 , 2);
/*
 hory
    display_write(&_display, text, textX-sep, textY, 0 , 1);
    display_write(&_display, text, textX+sep, textY, 0 , 2);
 */
}

void call(void)
{

    static uint8_t acc = 0;
    
    
//    if (acc++>4)
//    {
        CANMessage message;
        message.id = CAN_BLOB_ID;
        message.rtr = 0;
        message.length = 5;
        message.data[0] = canID;
        message.data[1] = getMainX( _sensors.shadowX );
        message.data[2] = getMainY( _sensors.shadowY );
        message.data[3] = getMainX( _sensors.lightX  );
        message.data[4] = getMainY( _sensors.lightY  );

        can_send_message(&message);
    
        display_setFillColor(&_display, 255);
        display_fillZone(&_display, _sensors.lightX, _sensors.lightY, 2, 2);


//        acc = 0;
//    }
    
    updateScreen();
}

/* **** **** **** **** **** **** **** **** */



void serviceCall(void)
{
    
    wdt_reset();
    
    CANMessage m;
    if (can_get_message(&m) )
    {

        if (m.id == 0)
            sendState();
        
        /**/
        
        else if (m.id == 4)
        {

            if (m.length !=3)
            {
                errorCode = 1;
                sendState();
            }
            textX = getRealX( m.data[0]);
            textY = getRealY( m.data[1]);
            sep   = m.data[2];
            updateScreen();
        }
        
        
        // change text
        else if ( m.id == 5)
        {
            
            strcpy(text, "test");

            updateScreen();
        }
        // clear draw
        else if (m.id == 6)
        {
            display_clearDraw( &_display);
            updateScreen();
        }
        else if( m.id == 7)
        {

            display_setFillColor(&_display, 255);
            display_fillZone(&_display, getRealX( m.data[0])-2, getRealY( m.data[1])-2, 4, 4);
            updateScreen();
        }

        //Tresholds
        else if (m.id == 8 )
        {
            _sensors.lowTreshold = m.data[0];
            _sensors.highTreshold = m.data[1];
        }

    }
}

/* **** **** **** **** **** **** **** **** */

int main( void )
{

    cli();
    wdt_enable(WDTO_8S);
    
    initDip();
    canID = readCanID();
    
    computeGeometry();
    
    sensors_init( &_sensors );
    _sensors.display = &_display;
    
    display_init( &_display);



    setOutput(LED_DDR, LED_PIN);
    setOutput( CAN_CS_DDR, CAN_CS_PIN );

    wdt_reset();
    
    if(mcp2515_init())
        setHigh(LED_PORT, LED_PIN);
    else
        setLow(LED_PORT, LED_PIN);

    _delay_ms(200);
    sei();
    
    /* END OF INIT */
    wdt_reset();
    
    display_clearAll(&_display);
    
    char name[7] = "A";

    sprintf(name, "%i", (int)canID );
    display_write(&_display, name, 2, 10, 0 , 0);

    _delay_ms(1000);
    sendState();
    
    /* *** *** *** *** *** */

    wdt_reset();

    strcpy(text, "");

    
    display_clearAll(&_display);

    updateScreen();
    
    
    //readFrame( &_sensors );
    
    while (1)
    {
        serviceCall();
        if(readOne( &_sensors) == 1)
        {
            // une trame entière lue
            toggle(LED_PORT, LED_PIN);
            for (uint8_t x = 0; x<X_TLC_MAX; x++)
            {
                //y = 1 passe en y = 0
                // y = 2 -> y =1
                for (uint8_t y = 0; y<Y_MIC_MAX; y++)
                {
                    if (y!= Y_MIC_MAX-1)
                        _display.buff_draw[x][y] = _display.buff_draw[x+1][y]/2;
                    else
                        _display.buff_draw[x][y] = 0;
                }
            }
            
            /* EFFET COULE !
            for (uint8_t y = 0; y<Y_MIC_MAX; y++)
            {
                //y = 1 passe en y = 0
                // y = 2 -> y =1
                for (uint8_t x = 0; x<X_TLC_MAX; x++)
                {
                    if (x!= X_TLC_MAX-1)
                        _display.buff_draw[x][y] = _display.buff_draw[x][y+1]/5;
                    else
                        _display.buff_draw[x][y] = 0;
                }
            }
             */
        }

    }
    
    return 0;
    
}





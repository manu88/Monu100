
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
#include "Constants.h"
#include "Errors.h"
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

uint8_t sep   = 0;
int8_t  angle = 0;
char text[TEXT_LENGTH_MAX];
char tempText[TEXT_LENGTH_MAX];

volatile uint8_t textChanging = 0;
volatile uint8_t textLength   = 0;

void updateScreen(void)
{
    display_clearDraw(&_display);
    display_clearText( &_display);

    display_setFillColor(&_display, 255);

    //display_write(&_display, text, textX-sep, textY, 1 , 1 /* ,-angle*/);
    //display_write(&_display, text, textX+sep, textY, 1 , 2 /*,angle*/);
/*
 hory
    display_write(&_display, text, textX-sep, textY, 0 , 1);
    display_write(&_display, text, textX+sep, textY, 0 , 2);
 */
}

void call(void)
{


    
    

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

//    display_setFillColor(&_display, 255);
//    display_fillZone(&_display, _sensors.lightX, _sensors.lightY, 2, 2);


    
    updateScreen();
}

/* **** **** **** **** **** **** **** **** */

inline uint8_t checkMsgLength( CANMessage *msg, const uint8_t l )
{
    if (msg->length !=l)
    {
        errorCode = ERROR_LENGTH_MSG;
        sendState();
        return 0;
    }
    return 1;
}

inline void sendError( const uint8_t err)
{
    errorCode = err;
    sendState();
}

inline void sendTest( const uint8_t val1 , const uint8_t val2)
{
    CANMessage m;
    m.id = 30;
    m.length = 3;
    m.data[0] = canID;
    m.data[1] = val1;
    m.data[2] = val2;
    can_send_message( &m);
    
}

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

            if (checkMsgLength( &m, 4) == 0)
                return;
            
            textX = getRealX( m.data[0]);
            textY = getRealY( m.data[1]);
            sep   = m.data[2];
            angle = m.data[3];
            updateScreen();
        }
        
        
        // change text
        else if ( m.id == 5)
        {
            
            strcpy(text, DEFAULT_NAME);

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
/*
            display_setFillColor(&_display, 255);
            display_fillZone(&_display, getRealX( m.data[0])-2, getRealY( m.data[1])-2, 4, 4);
 */
            updateScreen();
        }

        //Tresholds
        else if (m.id == 8 )
        {
            _sensors.lowTreshold = m.data[0];
            _sensors.highTreshold = m.data[1];
        }
        
        /* TEXT CHANGE */

        else if (m.id == TEXT_CHANGE_START_ID)
        {
            /*
            if (checkMsgLength( &m, 1) == 0)
                return;
            */
            

            
            textLength = m.data[0];
            sendTest(1 , textLength);
            
            textChanging =1;
//            tempText[0] = '\0';
            setLow(LED_PORT, LED_PIN);
            
            //sendTest(1 , textLength);
        }
        else if (m.id == TEXT_CHANGE_END_ID)
        {
            /*
            if (checkMsgLength( &m, 1) == 0)
                return;
            */
            sendTest(10 , textLength);
            textChanging =0;
            //tempText[textLength] = '\0';
            textLength = 0;
            //strcpy(text, tempText);
            //sendTest(20 , textLength);
            updateScreen();
            setHigh(LED_PORT, LED_PIN);
        }
        

        else if (m.id == TEXT_CHANGE_CHAR_ID)
        {
            /*
            if (checkMsgLength( &m, 3) == 0)
                return;
            */
            /*
            toggle(LED_PORT, LED_PIN);
            if( m.data[0] != textLength)
                sendError( ERROR_LENGTH_STR );
            
            if( m.data[1] >= textLength )
                sendError( ERROR_CHAR_POS);
            

            sendTest(12 , m.data[1]);
             */
            const char c = m.data[2];
            text[m.data[1] ] = c;
            

            
        }
        
        else if( m.id == SET_PIX_ID)
        {
            display_setPixel(&_display , m.data[0] , m.data[1], m.data[2]);
        }
    }
}

/* **** **** **** **** **** **** **** **** */

int main( void )
{

    cli();
//    wdt_enable(WDTO_8S);
    
    initDip();
    canID = readCanID();
    
    computeGeometry();
    
    for( uint8_t i =0; i<TEXT_LENGTH_MAX ; i++)
    {
        text[i] = 0;
        tempText[i] = 0;
    }
    
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

    strcpy(text, DEFAULT_NAME );

    
    display_clearAll(&_display);

    updateScreen();
    
    
    //readFrame( &_sensors );
    
    uint8_t xx = 0;
    uint8_t yy = 0;
    uint8_t val  = 0;
    
    uint8_t a = 0;
    while (1)
    {
//        serviceCall();
        
/*
        for (int i = 0; i < 40; i++ )
        {
            for (int j = 0 ; j < 30; j++)
            {
                display_setPixel(&_display ,i, j, 255);
                _delay_ms(10);
                
                display_setPixel(&_display ,i, j, 0);
                _delay_ms(10);
            }
        }
*/
        
        display_setPixel(&_display, xx, yy, val);
        
        if(readOne( &_sensors) == 1)
        {

            if (a++ >2)
            {
                for (uint8_t x = 0; x<X_TLC_MAX; x++)
                {
                    //y = 1 passe en y = 0
                    // y = 2 -> y =1
                    for (uint8_t y = 0; y<Y_MIC_MAX; y++)
                    {

                            int c =_display.buff_draw[x+1][y]-10;
                            
                            if (c <0)
                                c = 0;
                            _display.buff_draw[x][y] = c;

                    }
                }
                a =0;
            }

        }

    }
    
    return 0;
    
}





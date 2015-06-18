//
//  Sensors.c
//  MicMonu100
//
//  Created by Manuel Deneu on 01/03/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "Sensors.h"

#include "PinsConfig.h"
#include "quickblob.h"

#include "Coms.h"


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void adc_init(void)
{
    // MICs LDR
    
    setOutput( LDR_DATA_DDR       , LDR_DATA_PIN ); // data
    setOutput( LDR_STROBE_DDR     , LDR_STROBE_PIN ); // strobe
    setOutput( LDR_OUT_ENABLE_DDR , LDR_OUT_ENABLE_PIN ); // out enable
    setOutput( LDR_CLOCK_DDR      , LDR_CLOCK_PIN ); // clock

    
    /* *** */
    
    
    setLow(LDR_STROBE_PORT , LDR_STROBE_PIN ); // strobe mic LDR
    
    setLow( LDR_CLOCK_PORT , LDR_CLOCK_PIN);
    
    setLow( LDR_OUT_ENABLE_PORT , LDR_OUT_ENABLE_PIN );
    
    //
    
    setLow( LDR_DATA_PORT , LDR_DATA_PIN );
    
    for (int i=0; i<16; i++)
        pulse( LDR_CLOCK_PORT , LDR_CLOCK_PIN);
    
    
    /*
     setHigh( LDR_DATA_PORT , LDR_DATA_PIN );
     
     pulse( LDR_CLOCK_PORT , LDR_CLOCK_PIN);
     */
    
    
    pulse(LDR_STROBE_PORT, LDR_STROBE_PIN);

    
    /* *** */
    
    ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
    
    ADMUX |= (1<<REFS0);       //Set Voltage reference to Avcc (5v)
    
    ADCSRA |= (1<<ADEN);       //Turn on ADC
    
    ADCSRA |= (1<<ADSC);
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

inline uint16_t adc_read(uint8_t channel)
{
    
    ADMUX &= 0xE0;           //Clear bits MUX0-4
    ADMUX |= channel&0x07;   //Defines the new ADC channel to be read by setting bits MUX0-2
    ADCSRB = channel&(1<<3); //Set MUX5
    ADCSRA |= (1<<ADSC);      //Starts a new conversion
    
    while( ADCSRA & (1<<ADSC) );  //Wait until the conversion is done
    
    return ADCW;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void sensors_init( Sensors *sensors)
{
    adc_init();    
    sensors_resetCalibration( sensors );
    
    sensors->thresholdHigh = 0;
    
    sensors->thresholdLow  = 0;
    
    for (int j = 0; j<MIC_SENSOR_COUNT ; j++) // mics
    {
        
        for (int i = 0; i < MIC_SENSOR_COUNT ; i++)
        {
            sensors->values[i][j] = 0;
        }
    }
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void sensors_resetCalibration( Sensors *sensors )
{
    for (int j = 0; j<MIC_SENSOR_COUNT ; j++) // mics
    {
        
        for (int i = 0; i < MIC_SENSOR_COUNT ; i++)
        {
            sensors->calibValues[i][j] = 0;
        }
    }
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void sensors_calibration( Sensors *sensors , uint8_t count)
{
    sensors_resetCalibration( sensors );
    
    float temp[ SENSOR_COUNT ][ MIC_SENSOR_COUNT ];
    
    
    for ( uint8_t iter = 0; iter < count ; iter++)
    {
        for (int j = 0; j<MIC_SENSOR_COUNT ; j++) // mics
        {
            
            for (int i = 0; i < MIC_SENSOR_COUNT ; i++)
            {

                
                if (iter == 0)
                    temp[i][j] = 0;
                
                temp[i][j] += adc_read( i );// >> ADC_SHIFT_MULT;
                
                if( iter == count-1)
                {
                    temp[i][j] /= count;
                    sensors->calibValues[i][j] =  (uint16_t)temp[i][j];
                }
            }
            
            
            if (j== (MIC_SENSOR_COUNT-1) )
            {
                setHigh( LDR_DATA_PORT , LDR_DATA_PIN );
            }
            else
            {
                setLow( LDR_DATA_PORT , LDR_DATA_PIN );
            }
            
            pulse( LDR_CLOCK_PORT , LDR_CLOCK_PIN);
            
            pulse( LDR_STROBE_PORT, LDR_STROBE_PIN);
            
        }
        
    }
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void readFrame(Sensors *sensors)
{
    extract_image( sensors );
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void readRow( Sensors *sensors , uint8_t *buffer)
{
    
    for (int i = 0; i < MIC_SENSOR_COUNT ; i++)
    {
        uint8_t x = 0;
        uint8_t y = 0;
        
        int val = ( adc_read( i ) );// - sensors->calibValues[i][ sensors->currentRow ];


        /*if ( val < sensors->thresholdLow )
            val = BLOB_SHADOW;
        
        else */if ( val > sensors->thresholdHigh )
            val = BLOB_LIGHT;
        
        
        else
            val = 0;

        x = sensors->currentRow*2;
        y = (14-i)*2;

        if ( buffer != NULL)
            buffer[i] =(uint8_t) val ;
        
        sensors->values[i][ sensors->currentRow ] = val;

        display_setFillColor( sensors->display, val );
        display_fillZone( sensors->display , x - 2, y - 2 , 4, 4 );


    }
    
    
    if (sensors->currentRow == ( MIC_SENSOR_COUNT - 1) )
    {
        setHigh( LDR_DATA_PORT , LDR_DATA_PIN );
    }
    else
    {
        setLow( LDR_DATA_PORT , LDR_DATA_PIN );
    }
    
    pulse( LDR_CLOCK_PORT , LDR_CLOCK_PIN);
    pulse( LDR_STROBE_PORT, LDR_STROBE_PIN);

}


void readAll( Sensors *sensors)
{
    sensors->currentRow = 0;
    display_clear( sensors->display );
    for ( int i=0; i< MIC_SENSOR_COUNT;i++ )
    {
        readRow( sensors, NULL );
        sensors->currentRow++;
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void log_blob_hook(void* user_struct, struct blob* b)
{

    
    const uint8_t x = b->center_y*2;
    const uint8_t y = (14-b->center_x)*2;
    
    /*
    if ( ( x <-BLOB_MARGIN ) || ( y <-BLOB_MARGIN ))
        return;
    
    if ( (x >(30+BLOB_MARGIN)) || (y>(30+BLOB_MARGIN) ) )
        return;
    


    if (( b->size < MIN_BLOB_SIZE ) || ( b->size > MAX_BLOB_SIZE ))
        return;
    
     */


    const Sensors* sensors = ( const Sensors* ) user_struct;


    display_clear(sensors->display);
    display_setFillColor( sensors->display, 255 );


    if ( b->color == BLOB_SHADOW )
    {
        const uint8_t x1 = b->bb_x1*2;
        const uint8_t y1 = (14-b->bb_y1)*2;
        
        const uint8_t x2 = b->bb_x2*2;
        const uint8_t y2 = (14-b->bb_y2)*2;
        
        display_fillZone( sensors->display , x1, y1, x2 , y2 );
    }
    
    else if (b->color == BLOB_LIGHT )
    {
        display_drawCircle(sensors->display, x, y, b->size);
    }



}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

int init_pixel_stream_hook(void* user_struct, struct stream_state* stream)
{

    Sensors* sensors = ( Sensors* ) user_struct;
    sensors->currentRow = 0;
    
    stream->w = SENSOR_COUNT;
    stream->h = SENSOR_COUNT;
    stream->x = 0;
    stream->y = 0;
    
    return 0;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

int close_pixel_stream_hook(void* user_struct, struct stream_state* stream)
{

    return 0;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

int next_row_hook(void* user_struct, struct stream_state* stream)
{
    Sensors* sensors = ( Sensors* ) user_struct;

    if ( sensors->currentRow == SENSOR_COUNT)
    {
        
        return -1;
    }
    
    readRow( sensors , stream->row );

    sensors->currentRow++;
    

    
    return 0;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

int next_frame_hook(void* user_struct, struct stream_state* stream)
{
    
    Sensors* sensors = ( Sensors*) user_struct;
    sensors->currentRow = 0;
    
    
    userCall();
    
    if ( sensors->currentRow == SENSOR_COUNT)
    {
        sensors->currentRow = 0;
        return -1;
    }


    

    return 0;
}






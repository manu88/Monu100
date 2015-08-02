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
#include "mcp2515.h"
#include "CanMessages.h"

uint8_t analyze(Sensors *sensors);

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
    
    
    for (int j = 0; j<MIC_SENSOR_COUNT ; j++) // mics
    {
        
        for (int i = 0; i < SENSOR_COUNT ; i++)
        {
            sensors->values     [i][j] = 0;
        }
    }
    
    sensors->moyenne     = 1000.0f;
    sensors->prevMoyenne = 1000.0f;
    
    sensors->lowTreshold  = SEUIL_SHADOW;
    sensors->highTreshold = SEUIL_LIGHT;
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void sensors_resetCalibration( Sensors *sensors )
{

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
                
                temp[i][j] += (float) adc_read( i );// >> ADC_SHIFT_MULT;
                
                if( iter == count-1)
                {
                    temp[i][j] /= count;
//                    sensors->calibValues[i][j] =  (uint16_t)temp[i][j];
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
    const uint8_t j = sensors->currentRow;
    

    
    if (sensors->currentRow == 0 )
        setHigh( LDR_DATA_PORT , LDR_DATA_PIN );
    else
        setLow( LDR_DATA_PORT , LDR_DATA_PIN );
    
    
    pulse( LDR_CLOCK_PORT , LDR_CLOCK_PIN);
    pulse( LDR_STROBE_PORT, LDR_STROBE_PIN);

    _delay_us( 1600 );    // 1500
    
    for (int i = 0; i < SENSOR_COUNT ; i++)
//    for (int i = SENSOR_COUNT-1; i >=0 ; i--)
    {

        const int read = adc_read( i );


        uint8_t val =0;// = read< SEUIL_SHADOW?BLOB_SHADOW: (read>SEUIL_LIGHT?BLOB_LIGHT:0);

        if (read < sensors->lowTreshold )
            val = BLOB_SHADOW;
        
        else if ( read>sensors->highTreshold)
            val = BLOB_LIGHT;
        
        sensors->values[i][j] = val ;
        
        if( buffer != NULL)
            buffer[i] = val;


        if (val == BLOB_LIGHT && (i!= SENSOR_COUNT-1))
        {
            display_setFillColor(sensors->display, val);
            display_fillZone(sensors->display,mapXFromSensors(j) ,mapYFromSensors(i), 2, 2);
        }

    }


}




/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void log_blob_hook(void* user_struct, struct blob* b)
{
    Sensors* sensors = ( Sensors* ) user_struct;

    toggle(LED_PORT, LED_PIN);


    call();
    if(( b->color == BLOB_SHADOW) && (b->size <=4))
    {
        sensors->shadowX    = mapXFromSensors( b->center_y );
        sensors->shadowY    = mapYFromSensors( b->center_x );
        sensors->shadowSize = b->size;

    }
    else if (( b->color == BLOB_LIGHT) && (b->size <=10))
    {
        sensors->lightX = mapXFromSensors( b->center_y );
        sensors->lightY = mapYFromSensors( b->center_x );
        sensors->lightSize = b->size;

    }

}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

int init_pixel_stream_hook(void* user_struct, struct stream_state* stream)
{

    Sensors* sensors = ( Sensors* ) user_struct;
    sensors->currentRow = 0;
    
    stream->w = SENSOR_COUNT;
    stream->h = MIC_SENSOR_COUNT;
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
    
    serviceCall();
    
    if ( sensors->currentRow == MIC_SENSOR_COUNT)
    {
        sensors->currentRow = 0;
        return 0;//-1;
    }
    
    readRow( sensors , stream->row );

    sensors->currentRow++;

    return 0;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

int next_frame_hook(void* user_struct, struct stream_state* stream)
{
    /*
    Sensors* sensors = ( Sensors*) user_struct;

    if ( sensors->currentRow == MIC_SENSOR_COUNT)
    {
        sensors->currentRow = 0;
        return -1;
    }
     */

//    call();

    return 0;
}






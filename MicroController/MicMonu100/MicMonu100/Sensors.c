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


// initialize adc
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

void sensors_init( Sensors *sensors)
{
    adc_init();    
    sensors_setValuesTo(sensors, 0);
    sensors_resetCalibration( sensors );
    
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void sensors_setValuesTo( Sensors *sensors , uint8_t val)
{
    for (uint8_t i =0; i<SENSOR_COUNT ; i++)
    {
        for(uint8_t j = 0 ; j< MIC_SENSOR_COUNT ; j++)
            sensors->values[i][j] = val;
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
    
    int temp[ SENSOR_COUNT ][ MIC_SENSOR_COUNT ];
    
    
    for ( uint8_t iter = 0; iter < count ; iter++)
    {
        for (int j = 0; j<MIC_SENSOR_COUNT ; j++) // mics
        {
            
            for (int i = 0; i < MIC_SENSOR_COUNT ; i++)
            {
                if (i == 0) adc_read( 15 );
                
                if (iter == 0)
                    temp[i][j] = 0;
                
                temp[i][j] += adc_read( i ) >> 4;
                
                if( iter == count-1)
                {
                    temp[i][j] /= count;
                    sensors->calibValues[i][j] = (uint8_t) temp[i][j];
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

void sensors_cleanDetected( Sensors *sensors)
{
    sensors->num_detected = 0;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void test(Sensors *sensors)
{
    extract_image( sensors );
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void sensors_reccordState( Sensors *sensors , uint16_t thresholdLow , uint16_t thresholdHigh  ,uint8_t mode )
{

    uint8_t x = 0;
    uint8_t y = 0;
    
    for (int j = 0; j<MIC_SENSOR_COUNT ; j++) // mics
    {
        _delay_us( 10);
        for (int i = 0; i < MIC_SENSOR_COUNT ; i++)
        {
            
            _delay_us( 10);

            if (i == 0)
                adc_read( 15 );

            
            const uint8_t read = (adc_read( i ) >> 4);
            
            uint8_t val = read - sensors->calibValues[i][j];
            
            /*
            if( mode == 0)
                val*=2;
            else if( mode == 1)
                val =  read >= threshold ? 127 :0 ;
            
            else if( mode == 2)
                val =  read >= threshold ? 0:255 ;
            */

            if ( read < thresholdLow)
                val = 127;
            
            else if ( read > thresholdHigh )
                val = 255;
            
            else
                val = 0;

            /*
            if ( val< (sensors->calibValues[i][j]*0.7) )
            {
                // ombre
                val = 127;
            }
            
            else if (val >  (sensors->calibValues[i][j]*1.1) ) //threshold)
            {
                //lum
                val = 255;
                
            }
            else
                val = 0;
             */
            
            x = j*2;
            y = (14-i)*2;
            
            if ( (i != 0) && (i != 1) && (i != 2))
            //if ( val != sensors->values[i][j] )
            {
                sensors->values[i][j] = val;
                display_setFillColor( sensors->display, val );
//                display_fillZone( sensors->display , x, y , 2, 2);
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

//    extract_image( sensors );

}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void log_blob_hook(void* user_struct, struct blob* b)
{
    if ( (b->center_x <0 ) || (b->center_y <0 ))
        return;
    
    const uint8_t x = b->center_x*2;
    const uint8_t y = (14-b->center_y)*2;
    
    if ( (x >30) || (y>30 ) )
        return;
    


    Sensors* sensors = ( Sensors* ) user_struct;

    serial_send(  10 );
    _delay_us( 100 );
    
    serial_send(  x );
    _delay_us( 100 );
    
    serial_send(  y );
    _delay_us( 100 );


    
    
    
    

    if (1)//b->color )
    {
        display_setFillColor( sensors->display, 255 );
        display_fillZone( sensors->display , x-b->size, y-b->size , b->size*2, b->size*2 );
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
// you need to set several variables:
// stream->w = image width
// stream->h = image hight
// return status (0 for success)

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

int close_pixel_stream_hook(void* user_struct, struct stream_state* stream)
{

    return 0;
}
// free up anything you allocated in init_pixel_stream_hook
// return status (0 for success)

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

int next_row_hook(void* user_struct, struct stream_state* stream)
{
    Sensors* sensors = ( Sensors* ) user_struct;

    if ( sensors->currentRow == SENSOR_COUNT)
    {
        
        return -1;
    }
    
    for (int i = 0 ; i<stream->w ; i++)
        stream->row[i] = sensors->values[ sensors->currentRow ][i];

    sensors->currentRow++;
    

    
    return 0;
}
// load the (grayscale) row at stream->y into the (8 bit) stream->row array
// return status (0 for success)
/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */



int next_frame_hook(void* user_struct, struct stream_state* stream)
{

    
    Sensors* sensors = ( Sensors*) user_struct;

    display_clear( sensors->display );
    sensors->currentRow = 0;
    sensors_reccordState( sensors , 2 , 10  , 2);
    
    

    return 0;
}
// basically a no-op in the library, but useful for applications
// return status (0 for success, otherwise breaks the video loop)





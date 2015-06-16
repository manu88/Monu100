//
//  Sensors.h
//  MicMonu100
//
//  Created by Manuel Deneu on 01/03/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#include "Display.h"

#ifndef MicMonu100_Sensors_h
#define MicMonu100_Sensors_h


#define SENSOR_COUNT     15
#define MIC_SENSOR_COUNT 15


#define MAX_DETECTION_POINTS 60

struct _Sensors
{
    uint8_t values     [ SENSOR_COUNT ][ MIC_SENSOR_COUNT ];
    uint8_t calibValues[ SENSOR_COUNT ][ MIC_SENSOR_COUNT ];
    
    uint8_t detected[ MAX_DETECTION_POINTS *2 ];
    uint8_t num_detected;
    
    Display* display;
    
    uint8_t currentRow; // blobdetection
    
    
};

typedef struct _Sensors Sensors;

void sensors_init( Sensors *sensors);

void sensors_setValuesTo( Sensors *sensors , uint8_t val);


void test(Sensors *sensors);
void sensors_reccordState( Sensors *sensors , uint16_t thresholdLow , uint16_t thresholdHigh ,uint8_t mode );

void sensors_resetCalibration( Sensors *sensors );
void sensors_calibration( Sensors *sensors , uint8_t count);

void computeCentroid( Sensors *sensors, uint8_t *x , uint8_t *y);

void sensors_cleanDetected( Sensors *sensors);

inline uint16_t adc_read(uint8_t channel)
{
    
    ADMUX &= 0xE0;           //Clear bits MUX0-4
    ADMUX |= channel&0x07;   //Defines the new ADC channel to be read by setting bits MUX0-2
    ADCSRB = channel&(1<<3); //Set MUX5
    ADCSRA |= (1<<ADSC);      //Starts a new conversion
    
    while( ADCSRA & (1<<ADSC) );  //Wait until the conversion is done

    return ADCW;
}




#endif

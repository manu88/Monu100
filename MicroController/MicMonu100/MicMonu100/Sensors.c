//
//  Sensors.c
//  MicMonu100
//
//  Created by Manuel Deneu on 01/03/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#include <stdio.h>
#include <avr/io.h>


#include "Sensors.h"

#include "PinsConfig.h"



// initialize adc
void adc_init(void)
{
    // MICs LDR
    
    setOutput( LDR_DATA_DDR       , LDR_DATA_PIN ); // data
    setOutput( LDR_STROBE_DDR     , LDR_STROBE_PIN ); // strobe
    setOutput( LDR_OUT_ENABLE_DDR , LDR_OUT_ENABLE_PIN ); // out enable
    setOutput( LDR_CLOCK_DDR      , LDR_CLOCK_PIN ); // clock
    
    
   
    setHigh( LDR_STROBE_PORT , LDR_STROBE_PIN ); // strobe mic LDR
    
    
    // reset mic ldr
    
    setLow( LDR_DATA_PORT , LDR_DATA_PIN ); // data to low
    
    for (int i =0;i< 16;i++)
        pulse( LDR_CLOCK_PORT , LDR_CLOCK_PIN );

    
    // AREF = AVcc
    ADMUX = (1<<REFS0);
    
    // ADC Enable and prescaler of 128
    // 16000000/128 = 125000
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
    

}

void sensors_init( Sensors *sensors)
{
    adc_init();    
    sensors_setValuesTo(sensors, 0);
    
}


void sensors_setValuesTo( Sensors *sensors , uint8_t val)
{
    for (uint8_t i =0; i<SENSOR_COUNT ; i++)
        sensors->values[i] = val;
}
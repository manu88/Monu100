//
//  Sensors.h
//  MicMonu100
//
//  Created by Manuel Deneu on 01/03/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#ifndef MicMonu100_Sensors_h
#define MicMonu100_Sensors_h


#define SENSOR_COUNT     15
#define MIC_SENCOR_COUNT 15

struct _Sensors
{
    uint8_t values[ SENSOR_COUNT ];// = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
};

typedef struct _Sensors Sensors;

void sensors_init( Sensors *sensors);

void sensors_setValuesTo( Sensors *sensors , uint8_t val);

#endif

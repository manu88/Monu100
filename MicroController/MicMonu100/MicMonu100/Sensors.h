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

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

#define SENSOR_COUNT     15
#define MIC_SENSOR_COUNT 15

#define MIN_BLOB_SIZE 2
#define MAX_BLOB_SIZE 6

#define BLOB_MARGIN 5

#define BLOB_LIGHT  255
#define BLOB_SHADOW 255

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

struct _Sensors
{
    uint16_t calibValues[ SENSOR_COUNT ][ MIC_SENSOR_COUNT ];
    
    
    uint16_t thresholdHigh;
    int16_t thresholdLow;
    
    float moyenne;
    float prevMoyenne;
    
    Display* display;
    
    uint8_t currentRow;
    
    uint8_t values[ SENSOR_COUNT ][ MIC_SENSOR_COUNT ];
    
};



typedef struct _Sensors Sensors;

void sensors_init( Sensors *sensors);
void sensors_calibration( Sensors *sensors , uint8_t count);

uint8_t readAll( Sensors *sensors);
void sensors_drawInter( Sensors *sensors);

void readFrame(Sensors *sensors);
void userCall(void);

void sensors_resetCalibration( Sensors *sensors );








#endif

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
#define MIC_SENSOR_COUNT 10

#define MIN_BLOB_SIZE 2
#define MAX_BLOB_SIZE 6

#define BLOB_MARGIN  3

#define BLOB_LIGHT   255
#define BLOB_SHADOW  200

#define SEUIL_LIGHT  200
#define SEUIL_SHADOW 50

/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

struct _Sensors
{
//    uint16_t calibValues[ SENSOR_COUNT ][ MIC_SENSOR_COUNT ];
    
    
    uint8_t lowTreshold;
    uint8_t highTreshold;
    float moyenne;
    float prevMoyenne;
    
    Display* display;
    
    uint8_t currentRow;
    
    uint8_t values    [ SENSOR_COUNT ][ MIC_SENSOR_COUNT ];

    
    uint8_t shadowX;
    uint8_t shadowY;
    uint8_t shadowSize;
    
    uint8_t lightX;
    uint8_t lightY;
    uint8_t lightSize;
};
typedef struct _Sensors Sensors;

inline uint8_t mapXFromSensors( const uint8_t i )
{
    return (i*2);
}

inline uint8_t mapYFromSensors( const uint8_t j )
{
    return (14-j)*2;
}


void readRow( Sensors *sensors , uint8_t *buffer);

inline uint8_t readOne( Sensors* sensors)
{
    uint8_t ret = 0;
    if ( sensors->currentRow == MIC_SENSOR_COUNT)
    {
        ret = 1;
        sensors->currentRow = 0;
    }
    
    readRow(sensors, NULL);
    sensors->currentRow++;
    
    return ret;
    
}

void sensors_init( Sensors *sensors);
void sensors_calibration( Sensors *sensors , uint8_t count);




void readFrame(Sensors *sensors);
void userCall(void);

void sensors_resetCalibration( Sensors *sensors );




extern void call(void);
extern void serviceCall(void);



#endif

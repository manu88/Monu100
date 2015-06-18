//
//  main.c
//  algosMonu100
//
//  Created by Manuel Deneu on 03/06/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#include <stdio.h>
#include "quickblob.h"

#define SENSOR_COUNT 15
#define MIC_SENSOR_COUNT 15

uint8_t values[15][15] =
{
    { 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, },
    { 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, },
    { 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, },
    { 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, },
    { 0 , 0 , 0, 1 , 1 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, },
    { 0 , 0 , 0, 1 , 1 , 1, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, },
    { 0 , 0 , 0, 1 , 1 , 1, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, },
    { 0 , 0 , 0, 1 , 1 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, },
    { 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, },
    { 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, },
    { 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, },
    { 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, },
    { 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, },
    { 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, },
    { 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, 0 , 0 , 0, },
};


struct _Sensors
{
    uint8_t values     [ SENSOR_COUNT ][ MIC_SENSOR_COUNT ];
    uint8_t calibValues[ SENSOR_COUNT ][ MIC_SENSOR_COUNT ];
    

    uint8_t num_detected;
    
    
    
    uint8_t currentRow; // blobdetection
    
    
};

typedef struct _Sensors Sensors;

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
    //    const Sensors* sensors = ( const Sensors*) user_struct;
    
    
    
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

    
    if ( sensors->currentRow == SENSOR_COUNT)
    {
        sensors->currentRow = 0;
        return -1;
    }
    
    return 0;
}
// basically a no-op in the library, but useful for applications
// return status (0 for success, otherwise breaks the video loop)
void log_blob_hook(void* user_struct, struct blob* b)
{
    const uint8_t x = b->center_x*2;
    const uint8_t y = (14-b->center_y)*2;
    
    Sensors* sensors = ( Sensors* ) user_struct;
    
    printf("\n blob at %i %i " , x , y);
}




int main(int argc, const char * argv[])
{
    Sensors s;
    
    for (int i = 0; i< 15;i++)
        for (int j = 0; j< 15;j++)
            s.values[i][j] = values[i][j];

    
    
    while( 1)
    {
//        s.currentRow = 0;
    extract_image(&values);
        printf("\n After");
    }
    return 0;
}


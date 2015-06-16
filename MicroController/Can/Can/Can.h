//
//  Can.h
//  Can
//
//  Created by Manuel Deneu on 26/04/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#ifndef /* French*/ Can_Can_h
#define Can_Can_h

/*
struct _canConfig
{
    uint8_t csPin;
};

typedef struct _canConfig Can;
*/


/* Public */
uint8_t can_init( uint8_t speed );
void can_reset(void);



/* Private*/

uint8_t can_setMode( uint8_t mode );
uint8_t can_setRate(const uint8_t canSpeed);




void selectMCP(void);
void unSelectMCP(void);


uint8_t can_readRegister(const uint8_t address);
void can_setRegister(const uint8_t address, const uint8_t value);

void can_initCANBuffers(void);

#endif /* Can_Can_h */



/*
// CAN Bitrate 125 kbps
#define R_CNF1  (1<<BRP2)|(1<<BRP1)|(1<<BRP0)
#define R_CNF2  (1<<BTLMODE)|(1<<PHSEG11)
#define R_CNF3  (1<<PHSEG21)

// CAN Bitrate 250 kbps
#define R_CNF1  (1<<BRP1)|(1<<BRP0)
#define R_CNF2  (1<<BTLMODE)|(1<<PHSEG11)
#define R_CNF3  (1<<PHSEG21)
*/
// CAN Bitrate 500 kbps
#define R_CNF1  (1<<BRP0)
#define R_CNF2  (1<<BTLMODE)|(1<<PHSEG11)
#define R_CNF3  (1<<PHSEG21)
/*

// CAN Bitrate 1 Mbps
#define R_CNF1  0
#define R_CNF2  (1<<BTLMODE)|(1<<PHSEG11)
#define R_CNF3  (1<<PHSEG21)
*/

#include "PinsConfig.h"

typedef struct
{
    uint16_t  id;
    uint8_t   rtr;
    uint8_t   length;
    uint8_t   data[8];
} CANMessage;



uint8_t mcp2515_init(void);

uint8_t can_send_message(CANMessage *p_message);
uint8_t can_get_message(CANMessage *p_message);
void mcp2515_write_register( uint8_t adress, uint8_t data );
uint8_t mcp2515_read_register(uint8_t adress);
void mcp2515_bit_modify(uint8_t adress, uint8_t mask, uint8_t data);
//
//  PinsConfig.h
//  MicMonu100
//
//  Created by Manuel Deneu on 25/02/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//






#ifndef MicMonu100_PinsConfig_h
#define MicMonu100_PinsConfig_h

/* **** **** **** **** **** **** **** **** **** **** */
/*
 
 These pins are defined for the ATMega2560 chip
 
 */

#ifndef __AVR_ATmega2560__
#error " pinout must be redefined"
#endif

/* **** **** **** **** **** **** **** **** **** **** */




#define setOutput(ddr, pin) ((ddr) |= (1 << (pin)))
#define setLow(port, pin) ((port) &= ~(1 << (pin)))
#define setHigh(port, pin) ((port) |= (1 << (pin)))
#define toggle(port , pin) ((port) ^= (1 << (pin)))

#define getValue(port, pin) ((port) & (1 << (pin)))
#define outputState(port, pin) getValue(port,pin)

#define pulse(port, pin) do {                       \
setHigh((port), (pin));  \
setLow((port), (pin));   \
} while (0)


#define LED_DDR  DDRH
#define LED_PORT PORTH
#define LED_PIN  PH3
/* **** **** **** **** **** **** **** **** **** **** */
// TLC Pins

#define SIN_DDR    DDRB // ok
#define SIN_PORT   PORTB
#define SIN_PIN    PB2

#define SCLK_DDR  DDRB // ok
#define SCLK_PORT PORTB
#define SCLK_PIN  PB1

#define BLANK_DDR  DDRB // DEVRAIT ETRE B0 pour SS'
#define BLANK_PORT PORTB
#define BLANK_PIN  PB0

#define XLAT_DDR  DDRB // ok
#define XLAT_PORT PORTB
#define XLAT_PIN  PB5

/* **** **** **** **** **** **** **** **** **** **** */
// MICs pins

/* LEDS */

#define DATA0_DDR  DDRL      // data row 0
#define DATA0_PORT PORTL
#define DATA0_PIN  PL3


#define DATA1_DDR  DDRL      // data row 1
#define DATA1_PORT PORTL
#define DATA1_PIN  PL4

#define MIC_CLOCK_DDR  DDRL
#define MIC_CLOCK_PORT PORTL
#define MIC_CLOCK_PIN  PL1

#define MIC_STROBE_DDR  DDRL
#define MIC_STROBE_PORT PORTL
#define MIC_STROBE_PIN PL2

#define MIC_OUT_ENABLE_DDR  DDRL // utile?
#define MIC_OUT_ENABLE_PORT PORTL
#define MIC_OUT_ENABLE_PIN  PL0


/* LDR sensors */

#define LDR_DATA_DDR  DDRD // 38
#define LDR_DATA_PORT PORTD
#define LDR_DATA_PIN  PD7

#define LDR_CLOCK_DDR  DDRD // 18
#define LDR_CLOCK_PORT PORTD
#define LDR_CLOCK_PIN  PD3

#define LDR_STROBE_DDR  DDRE //2
#define LDR_STROBE_PORT PORTE
#define LDR_STROBE_PIN  PE4

#define LDR_OUT_ENABLE_DDR  DDRE // 5
#define LDR_OUT_ENABLE_PORT PORTE
#define LDR_OUT_ENABLE_PIN  PE3


/* MCP2515*/

#define CAN_CS_DDR  DDRC
#define CAN_CS_PORT PORTC
#define CAN_CS_PIN  PC0

#define CAN_RST_DDR  DDRC
#define CAN_RST_PORT PORTC
#define CAN_RST_PIN  PC1






#endif

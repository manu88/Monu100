//
//  spi.h
//  MicMonu100
//
//  Created by Manuel Deneu on 17/07/2015.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#ifndef MicMonu100_spi_h
#define MicMonu100_spi_h

#include <avr/io.h>
#include <stdlib.h>

inline void spi_init(void)
{
    DDR_SPI  |= (1<<P_SCK)|(1<<P_MOSI);
    PORT_SPI &= ~((1<<P_SCK)|(1<<P_MOSI)|(1<<P_MISO));
    
    // Enable SPI, Master, set clock rate fck/2
    SPCR = (1 << SPE) | (1 << MSTR);
    SPSR = (1 << SPI2X);
}




inline uint8_t spi_putc( uint8_t data )
{
    // Sendet ein Byte
    SPDR = data;
    
    // Wartet bis Byte gesendet wurde
    while( !( SPSR & (1<<SPIF) ) )
        ;
    
    return SPDR;
}




#endif

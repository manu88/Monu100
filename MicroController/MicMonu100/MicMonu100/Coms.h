//
//  Coms.h
//  MicMonu100
//
//  Created by Manuel Deneu on 08/05/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#ifndef MicMonu100_Coms_h
#define MicMonu100_Coms_h

#define USART_BAUDRATE  19200
#define UBRR_VALUE ((( 8000000 / (USART_BAUDRATE * 16UL))) - 1)

inline void serial_init(void)
{
    // Set baud rate
    UBRR0H = (uint8_t)(UBRR_VALUE>>8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    // Set frame format to 8 data bits, no parity, 1 stop bit
    UCSR0C |= (1<<UCSZ01)|(1<<UCSZ00); //|(1<<UMSEL00);
    //enable transmission and reception
    UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
}

inline void serial_send(uint8_t u8Data)
{

    while( !(UCSR0A & ( 1 << UDRE0 ) ) )
    {};

    
    
    UDR0 = u8Data;

}

#endif

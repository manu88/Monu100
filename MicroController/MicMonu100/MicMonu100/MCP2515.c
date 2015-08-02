

#include <stdint.h>
#include <util/delay.h>

#include "PinsConfig.h"
#include "spi.h"
#include "MCP2515.h"
#include "mcp2515_defs.h"

void mcp2515_write_register( uint8_t adress, uint8_t data )
{
    setLow(CAN_CS_PORT, CAN_CS_PIN);
    
    spi_putc(SPI_WRITE);
    spi_putc(adress);
    spi_putc(data);
    
    setHigh(CAN_CS_PORT, CAN_CS_PIN);
}

uint8_t mcp2515_read_register(uint8_t adress)
{
    uint8_t data;
    
    setLow(CAN_CS_PORT, CAN_CS_PIN);
    
    spi_putc(SPI_READ);
    spi_putc(adress);
    
    data = spi_putc(0xff);
    
    setHigh(CAN_CS_PORT, CAN_CS_PIN);
    
    return data;
}

void mcp2515_bit_modify(uint8_t adress, uint8_t mask, uint8_t data)
{
    setLow(CAN_CS_PORT, CAN_CS_PIN);
    
    spi_putc(SPI_BIT_MODIFY);
    spi_putc(adress);
    spi_putc(mask);
    spi_putc(data);
    
    setHigh(CAN_CS_PORT, CAN_CS_PIN);
}

uint8_t mcp2515_init(void)
{
    // SPI Interface initialisieren
    spi_init();
    
    // MCP2515 per Software Reset zuruecksetzten,
    // danach ist der MCP2515 im Configuration Mode
    setLow(CAN_CS_PORT, CAN_CS_PIN);
    spi_putc( SPI_RESET );
    _delay_ms(1);
    setHigh(CAN_CS_PORT, CAN_CS_PIN);
    
    // etwas warten bis sich der MCP2515 zurueckgesetzt hat
    _delay_ms(10);
    
    /*
     *  Einstellen des Bit Timings
     *
     *  Fosc       = 16MHz
     *  BRP        = 7                (teilen durch 8)
     *  TQ = 2 * (BRP + 1) / Fosc  (=> 1 uS)
     *
     *  Sync Seg   = 1TQ
     *  Prop Seg   = (PRSEG + 1) * TQ  = 1 TQ
     *  Phase Seg1 = (PHSEG1 + 1) * TQ = 3 TQ
     *  Phase Seg2 = (PHSEG2 + 1) * TQ = 3 TQ
     *
     *  Bus speed  = 1 / (Total # of TQ) * TQ
     *             = 1 / 8 * TQ = 125 kHz
     */
    
    // BRP = 7
    mcp2515_write_register( CNF1, R_CNF1 );
    
    // Prop Seg und Phase Seg1 einstellen
    mcp2515_write_register( CNF2, R_CNF2 );
    
    // Wake-up Filter deaktivieren, Phase Seg2 einstellen
    mcp2515_write_register( CNF3, R_CNF3 );
    
    // Aktivieren der Rx Buffer Interrupts
    mcp2515_write_register( CANINTE, (1<<RX1IE)|(1<<RX0IE) );
    
    /*
     *  Einstellen der Filter
     */
    
    // Buffer 0 : Empfangen aller Nachrichten
    mcp2515_write_register( RXB0CTRL, (1<<RXM1)|(1<<RXM0) );
    
    // Buffer 1 : Empfangen aller Nachrichten
    mcp2515_write_register( RXB1CTRL, (1<<RXM1)|(1<<RXM0) );
    
    // Alle Bits der Empfangsmaske loeschen,
    // damit werden alle Nachrichten empfangen
    mcp2515_write_register( RXM0SIDH, 0 );
    mcp2515_write_register( RXM0SIDL, 0 );
    mcp2515_write_register( RXM0EID8, 0 );
    mcp2515_write_register( RXM0EID0, 0 );
    
    mcp2515_write_register( RXM1SIDH, 0 );
    mcp2515_write_register( RXM1SIDL, 0 );
    mcp2515_write_register( RXM1EID8, 0 );
    mcp2515_write_register( RXM1EID0, 0 );
    
    /*
     *  Einstellen der Pin Funktionen
     */
    
    // Deaktivieren der Pins RXnBF Pins (High Impedance State)
    mcp2515_write_register( BFPCTRL, 0 );
    
    // TXnRTS Bits als Inputs schalten
    mcp2515_write_register( TXRTSCTRL, 0 );
    
    // Device zurueck in den normalen Modus versetzten
    mcp2515_bit_modify( CANCTRL, 0xE0, 0);
    
    return mcp2515_read_register(CNF1) == (R_CNF1);
}




uint8_t can_send_message(CANMessage *p_message)
{
    uint8_t status, address;
    
    // Status des MCP2515 auslesen
    setLow(CAN_CS_PORT, CAN_CS_PIN);
    spi_putc(SPI_READ_STATUS);
    status = spi_putc(0xff);
    spi_putc(0xff);
    setHigh(CAN_CS_PORT, CAN_CS_PIN);
    
    /* Statusbyte:
     *
     * Bit  Funktion
     *  2   TXB0CNTRL.TXREQ
     *  4   TXB1CNTRL.TXREQ
     *  6   TXB2CNTRL.TXREQ
     */
    
    if (bit_is_clear(status, 2)) {
        address = 0x00;
    }
    else if (bit_is_clear(status, 4)) {
        address = 0x02;
    }
    else if (bit_is_clear(status, 6)) {
        address = 0x04;
    }
    else {
        /* Alle Puffer sind belegt,
         Nachricht kann nicht verschickt werden */
        return 0;
    }
    
    setLow(CAN_CS_PORT, CAN_CS_PIN);
    spi_putc(SPI_WRITE_TX | address);
    
    // Standard ID einstellen
    spi_putc((uint8_t) (p_message->id>>3));
    spi_putc((uint8_t) (p_message->id<<5));
    
    // Extended ID
    spi_putc(0x00);
    spi_putc(0x00);
    
    uint8_t length = p_message->length;
    
    if (length > 8) {
        length = 8;
    }
    
    // Ist die Nachricht ein "Remote Transmit Request" ?
    if (p_message->rtr)
    {
        /* Ein RTR hat zwar eine Laenge,
         aber enthaelt keine Daten */
        
        // Nachrichten Laenge + RTR einstellen
        spi_putc((1<<RTR) | length);
    }
    else
    {
        // Nachrichten Laenge einstellen
        spi_putc(length);
        
        // Daten
        for (uint8_t i=0;i<length;i++) {
            spi_putc(p_message->data[i]);
        }
    }
    setHigh(CAN_CS_PORT, CAN_CS_PIN);
    
    asm volatile ("nop");
    
    /* CAN Nachricht verschicken
     die letzten drei Bit im RTS Kommando geben an welcher
     Puffer gesendet werden soll */
    setLow(CAN_CS_PORT, CAN_CS_PIN);
    if (address == 0x00) {
        spi_putc(SPI_RTS | 0x01);
    } else {
        spi_putc(SPI_RTS | address);
    }
    setHigh(CAN_CS_PORT, CAN_CS_PIN);
    
    return 1;
}



uint8_t mcp2515_read_rx_status(void)
{
    uint8_t data;
    
    // /CS des MCP2515 auf Low ziehen
    setLow(CAN_CS_PORT, CAN_CS_PIN);
    
    spi_putc(SPI_RX_STATUS);
    data = spi_putc(0xff);
    
    // Die Daten werden noch einmal wiederholt gesendet,
    // man braucht also nur eins der beiden Bytes auswerten.
//    spi_putc(0xff);
    
    // /CS Leitung wieder freigeben
    setHigh(CAN_CS_PORT, CAN_CS_PIN);
    
    return data;
}



uint8_t can_get_message(CANMessage *p_message)
{
    // Status auslesen
    uint8_t status = mcp2515_read_rx_status();
    
    if (bit_is_set(status,6))
    {
        // Nachricht in Puffer 0
        
        setLow(CAN_CS_PORT, CAN_CS_PIN);
        spi_putc(SPI_READ_RX);
    }
    else if (bit_is_set(status,7))
    {
        // Nachricht in Puffer 1
        
        setLow(CAN_CS_PORT, CAN_CS_PIN);
        spi_putc(SPI_READ_RX | 0x04);
    }
    else {
        /* Fehler: Keine neue Nachricht vorhanden */
        return 0;
    }
    
    // Standard ID auslesen
    p_message->id =  (uint16_t) spi_putc(0xff) << 3;
    p_message->id |= (uint16_t) spi_putc(0xff) >> 5;
    
    spi_putc(0xff);
    spi_putc(0xff);
    
    // Laenge auslesen
    uint8_t length = spi_putc(0xff) & 0x0f;
    p_message->length = length;
    
    // Daten auslesen
    for (uint8_t i=0;i<length;i++) {
        p_message->data[i] = spi_putc(0xff);
    }
    
    setHigh(CAN_CS_PORT, CAN_CS_PIN);
    
    if (bit_is_set(status,3)) {
        p_message->rtr = 1;
    } else {
        p_message->rtr = 0;
    }
    
    // Interrupt Flag loeschen
    if (bit_is_set(status,6)) {
        mcp2515_bit_modify(CANINTF, (1<<RX0IF), 0);
    } else {
        mcp2515_bit_modify(CANINTF, (1<<RX1IF), 0);
    }
    
    return (status & 0x07)+1;
}
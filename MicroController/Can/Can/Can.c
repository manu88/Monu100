//
//  Can.c
//  Can
//
//  Created by Manuel Deneu on 26/04/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#include <stdio.h>

#include "PinsConfig.h"

#include "Can.h"

#include "MCP2515.h"



void can_writeId( const uint8_t mcp_addr, const uint8_t ext, const uint8_t id );


/* **** **** **** **** **** **** **** **** **** **** */

inline void sendSPI( const uint8_t val)
{
    SPDR = val;
    while (!(SPSR & (1 << SPIF)));
}

inline void sendReadSPI( const uint8_t val , uint8_t *out)
{
    SPDR = val;
    while (!(SPSR & (1 << SPIF)));
    
    *out = SPDR;
}

/* **** **** **** **** **** **** **** **** **** **** */

inline void can_modifyRegister(const uint8_t address, const uint8_t mask, const uint8_t data)
{
    selectMCP();
    
    sendSPI( MCP_BITMOD );
    sendSPI( address);
    sendSPI( mask );
    sendSPI( data );
    
    unSelectMCP();
}

/* **** **** **** **** **** **** **** **** **** **** */

uint8_t can_init( uint8_t speed /*Can *can*/ )
{

    setOutput( CAN_CS_DDR, CAN_CS_PIN);

    can_reset();

    if (!can_setMode( MODE_CONFIG ) )
        return 0;
    
    
    if (!can_setRate( speed ))
        return 0;

    can_initCANBuffers();
    
    /* interrupt mode               */
    can_setRegister(MCP_CANINTE, MCP_RX0IF | MCP_RX1IF);
    
    can_modifyRegister( MCP_RXB0CTRL,
                           MCP_RXB_RX_MASK | MCP_RXB_BUKT_MASK,
                           MCP_RXB_RX_STDEXT | MCP_RXB_BUKT_MASK
                       );
    can_modifyRegister( MCP_RXB1CTRL, MCP_RXB_RX_MASK, MCP_RXB_RX_STDEXT );
    
    
    if( !can_setMode(MODE_NORMAL))
        return 0;
    

    
    return 1;
}

/* **** **** **** **** **** **** **** **** **** **** */

uint8_t can_setMode( uint8_t mode )
{
    uint8_t ret = 0;
    can_modifyRegister( MCP_CANCTRL, MODE_MASK, mode);
    
    ret = can_readRegister(MCP_CANCTRL) &MODE_MASK;
    
    return ret == mode;
}

/* **** **** **** **** **** **** **** **** **** **** */

uint8_t can_sendMsg( uint32_t id, uint8_t ext, uint8_t len, uint8_t *pData)
{
    /* SET */
    int i = 0;
//    m_nExtFlg = ext;
//    m_nID     = id;
//    m_nDlc    = len;
    
    for(i = 0; i<MAX_CHAR_IN_MESSAGE; i++)
        m_nDta[i] = *(pData+i);
    return MCP2515_OK;
    
    
    /* SEND */
    uint8_t res;
    uint8_t res1;
    uint8_t txbuf_n;
    
    uint16_t uiTimeOut = 0;
    
    do {
        res = mcp2515_getNextFreeTXBuf(&txbuf_n);                       /* info = addr.                 */
        uiTimeOut++;
    } while (res == MCP_ALLTXBUSY && (uiTimeOut < TIMEOUTVALUE));
    
    if(uiTimeOut == TIMEOUTVALUE)
    {
        return CAN_GETTXBFTIMEOUT;                                      /* get tx buff time out         */
    }
    uiTimeOut = 0;
    mcp2515_write_canMsg( txbuf_n);
    mcp2515_start_transmit( txbuf_n );
    do
    {
        uiTimeOut++;
        res1= mcp2515_readRegister(txbuf_n);  			                /* read send buff ctrl reg 	*/
        res1 = res1 & 0x08;
    }while(res1 && (uiTimeOut < TIMEOUTVALUE));
    if(uiTimeOut == TIMEOUTVALUE)                                       /* send msg timeout             */
    {
        return CAN_SENDMSGTIMEOUT;
    }
    return CAN_OK;
}

/* **** **** **** **** **** **** **** **** **** **** */

void can_reset()
{

    selectMCP();
    sendSPI( MCP_RESET );
    unSelectMCP();
//    delay(10);
}


uint8_t can_setRate(const uint8_t canSpeed)
{
    uint8_t set;
    uint8_t cfg1;
    uint8_t cfg2;
    uint8_t cfg3;
    
    set = 1;
    switch (canSpeed)
    {
        case (CAN_5KBPS):
            cfg1 = MCP_16MHz_5kBPS_CFG1;
            cfg2 = MCP_16MHz_5kBPS_CFG2;
            cfg3 = MCP_16MHz_5kBPS_CFG3;
            break;
            
        case (CAN_10KBPS):
            cfg1 = MCP_16MHz_10kBPS_CFG1;
            cfg2 = MCP_16MHz_10kBPS_CFG2;
            cfg3 = MCP_16MHz_10kBPS_CFG3;
            break;
            
        case (CAN_20KBPS):
            cfg1 = MCP_16MHz_20kBPS_CFG1;
            cfg2 = MCP_16MHz_20kBPS_CFG2;
            cfg3 = MCP_16MHz_20kBPS_CFG3;
            break;
            
        case (CAN_31K25BPS):
            cfg1 = MCP_16MHz_31k25BPS_CFG1;
            cfg2 = MCP_16MHz_31k25BPS_CFG2;
            cfg3 = MCP_16MHz_31k25BPS_CFG3;
            break;
            
        case (CAN_40KBPS):
            cfg1 = MCP_16MHz_40kBPS_CFG1;
            cfg2 = MCP_16MHz_40kBPS_CFG2;
            cfg3 = MCP_16MHz_40kBPS_CFG3;
            break;
            
        case (CAN_50KBPS):
            cfg1 = MCP_16MHz_50kBPS_CFG1;
            cfg2 = MCP_16MHz_50kBPS_CFG2;
            cfg3 = MCP_16MHz_50kBPS_CFG3;
            break;
            
        case (CAN_80KBPS):
            cfg1 = MCP_16MHz_80kBPS_CFG1;
            cfg2 = MCP_16MHz_80kBPS_CFG2;
            cfg3 = MCP_16MHz_80kBPS_CFG3;
            break;
            
        case (CAN_100KBPS):                                             /* 100KBPS                  */
            cfg1 = MCP_16MHz_100kBPS_CFG1;
            cfg2 = MCP_16MHz_100kBPS_CFG2;
            cfg3 = MCP_16MHz_100kBPS_CFG3;
            break;
            
        case (CAN_125KBPS):
            cfg1 = MCP_16MHz_125kBPS_CFG1;
            cfg2 = MCP_16MHz_125kBPS_CFG2;
            cfg3 = MCP_16MHz_125kBPS_CFG3;
            break;
            
        case (CAN_200KBPS):
            cfg1 = MCP_16MHz_200kBPS_CFG1;
            cfg2 = MCP_16MHz_200kBPS_CFG2;
            cfg3 = MCP_16MHz_200kBPS_CFG3;
            break;
            
        case (CAN_250KBPS):
            cfg1 = MCP_16MHz_250kBPS_CFG1;
            cfg2 = MCP_16MHz_250kBPS_CFG2;
            cfg3 = MCP_16MHz_250kBPS_CFG3;
            break;
            
        case (CAN_500KBPS):
            cfg1 = MCP_16MHz_500kBPS_CFG1;
            cfg2 = MCP_16MHz_500kBPS_CFG2;
            cfg3 = MCP_16MHz_500kBPS_CFG3;
            break;
            
        case (CAN_1000KBPS):
            cfg1 = MCP_16MHz_1000kBPS_CFG1;
            cfg2 = MCP_16MHz_1000kBPS_CFG2;
            cfg3 = MCP_16MHz_1000kBPS_CFG3;
            break;  
            
        default:
            set = 0;
            break;
    }
    
    if (set)
    {
        can_setRegister( MCP_CNF1, cfg1);
        can_setRegister( MCP_CNF2, cfg2);
        can_setRegister( MCP_CNF3, cfg3);
        return 1;
    }
    else
        return 0;

}

/* **** **** **** **** **** **** **** **** **** **** */

uint8_t can_readRegister(const uint8_t address)
{
    uint8_t ret;
    
    selectMCP();
    
    sendSPI( MCP_READ );
    sendReadSPI(address , &ret);
    
    unSelectMCP();
    
    return ret;
}

/* **** **** **** **** **** **** **** **** **** **** */

void can_setRegister(const uint8_t address, const uint8_t value)
{
    selectMCP();
    sendSPI(MCP_WRITE);
    sendSPI(address);
    sendSPI(value);
    unSelectMCP();
}

void can_setRegisters(const uint8_t address, const uint8_t values[], const uint8_t n)
{
    uint8_t i;
    selectMCP();
    sendSPI(MCP_WRITE);
    sendSPI(address);
    
    for (i=0; i<n; i++)
    {
        sendSPI(values[i]);
    }
    unSelectMCP();
}

/* **** **** **** **** **** **** **** **** **** **** */

void can_initCANBuffers(void)
{
    uint8_t i;
    uint8_t a1;
    uint8_t a2;
    uint8_t a3;
    
    uint8_t std = 0;
    uint8_t ext = 1;
    uint8_t ulMask = 0x00;
    uint8_t ulFilt = 0x00;
    
    
    can_writeId(MCP_RXM0SIDH, ext, ulMask);			/*Set both masks to 0           */
    can_writeId(MCP_RXM1SIDH, ext, ulMask);			/*Mask register ignores ext bit */
    
    /* Set all filters to 0         */
    can_writeId(MCP_RXF0SIDH, ext, ulFilt);			/* RXB0: extended               */
    can_writeId(MCP_RXF1SIDH, std, ulFilt);			/* RXB1: standard               */
    can_writeId(MCP_RXF2SIDH, ext, ulFilt);			/* RXB2: extended               */
    can_writeId(MCP_RXF3SIDH, std, ulFilt);			/* RXB3: standard               */
    can_writeId(MCP_RXF4SIDH, ext, ulFilt);
    can_writeId(MCP_RXF5SIDH, std, ulFilt);
    
    /* Clear, deactivate the three  */
    /* transmit buffers             */
    /* TXBnCTRL -> TXBnD7           */
    a1 = MCP_TXB0CTRL;
    a2 = MCP_TXB1CTRL;
    a3 = MCP_TXB2CTRL;
    
    for (i = 0; i < 14; i++)
    {
        can_setRegister(a1, 0);
        can_setRegister(a2, 0);
        can_setRegister(a3, 0);
        a1++;
        a2++;
        a3++;
    }
    can_setRegister(MCP_RXB0CTRL, 0);
    can_setRegister(MCP_RXB1CTRL, 0);
}

void can_writeId( const uint8_t mcp_addr, const uint8_t ext, const uint8_t id )
{
    uint16_t canid;
    uint8_t tbufdata[4];
    
    canid = (uint16_t)(id & 0x0FFFF);
    
    if ( ext == 1)
    {
        tbufdata[MCP_EID0] = (uint8_t) (canid & 0xFF);
        tbufdata[MCP_EID8] = (uint8_t) (canid >> 8);
        canid = (uint16_t)(id >> 16);
        tbufdata[MCP_SIDL] = (uint8_t) (canid & 0x03);
        tbufdata[MCP_SIDL] += (uint8_t) ((canid & 0x1C) << 3);
        tbufdata[MCP_SIDL] |= MCP_TXB_EXIDE_M;
        tbufdata[MCP_SIDH] = (uint8_t) (canid >> 5 );
    }
    else
    {
        tbufdata[MCP_SIDH] = (uint8_t) (canid >> 3 );
        tbufdata[MCP_SIDL] = (uint8_t) ((canid & 0x07 ) << 5);
        tbufdata[MCP_EID0] = 0;
        tbufdata[MCP_EID8] = 0;
    }
    can_setRegisters( mcp_addr, tbufdata, 4 );
}




void selectMCP()
{
    setLow( CAN_CS_PORT, CAN_CS_PIN);
}

void unSelectMCP()
{
    setHigh( CAN_CS_PORT, CAN_CS_PIN);
}
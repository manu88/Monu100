//
//  CanMessages.h
//  MicMonu100
//
//  Created by Manuel Deneu on 31/07/2015.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#ifndef MicMonu100_CanMessages_h
#define MicMonu100_CanMessages_h

/* Message state par plaque */
#define CAN_STATE_ID 1
/*
 Data 0 -> id de la plaque
 Data 1 -> MCU Status Register (MCUSR)
 ...
 */


/* Message blob detect*/
#define CAN_BLOB_ID 2
/*
 Data 0 -> id de la plaque
 Data 1 -> X
 Data 2 -> Y
 Data 3 -> size
 */

#define CAN_ERROR 3

#endif

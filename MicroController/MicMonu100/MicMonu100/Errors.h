//
//  Errors.h
//  MicMonu100
//
//  Created by Manuel Deneu on 03/08/2015.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#ifndef MicMonu100_Errors_h
#define MicMonu100_Errors_h


#define ERROR_NONE         0
#define ERROR_LENGTH_MSG   1

#define ERROR_LENGTH_STR    2 // string length differs in can msgs.
#define ERROR_BOUNDS_LENGTH 3 // textLength > TEXT_LENGTH_MAX
#define ERROR_CHAR_POS      4 // char pos ouside text bounds <0 || >=textLength
#endif

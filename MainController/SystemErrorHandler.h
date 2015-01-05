//
//  ErrorHandler.h
//  Broadway_core
//
//  Created by Manuel Deneu on 04/01/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#ifndef __Broadway_core__ErrorHandler__
#define __Broadway_core__ErrorHandler__

#include "Config.h"

/* **** **** **** **** **** **** */

typedef enum
{
    // hopefully
    ERRORS_FREE                  = 0,
    
    // XML
    ERRORS_XMLFILE_NOTFOUND      = ( 1 << 0 ),
    ERRORS_XMLFILE_PARSING       = ( 1 << 1 ),

    // Network
    ERRORS_NETWORK_DOWN          = ( 1 << 4 ),
    ERRORS_WEB_UNREACHABLE       = ( 1 << 5 ),
    
    // Can
    ERRORS_CAN_DOWN              = ( 1 << 5 ),
    
    ERRORS_ALL                   = -1
} Error_flags;

/* **** **** **** **** **** **** */

class SystemErrorHandler
{
    
public:
    SystemErrorHandler();
    ~SystemErrorHandler();
    
    void addError( const Error_flags error);
    void removeError( const Error_flags error);
    bool isErrorSet( const Error_flags error) const ;
    
    bool isEverythingOk() const
    {
        return _errorsList == ERRORS_FREE;
    }
    
    bool hasErrors() const
    {
        return !isEverythingOk();
    }
    
    void dumpSystemReportOnLog() const;
    
private:
    Error_flags _errorsList;
    
};


#endif /* defined(__Broadway_core__ErrorHandler__) */

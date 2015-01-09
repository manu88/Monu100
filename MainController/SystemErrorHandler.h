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
    ERRORS_FREE                  = 0, // yeeeha
    
    // DATA
    ERRORS_DATAFILE_NOTFOUND     = ( 1 << 0 ), // data file not found
    ERRORS_DATAFILE_PARSING      = ( 1 << 1 ), // error in parse -> check NameParser's errors

    // Netork fetch
    ERROR_INVALID_URL            = ( 1 << 2), // curl fetch failed
    ERROR_UPDATE_INSPECT         = ( 1 << 3), // set if before copy temp. to def., inspect JSON fails
    ERROR_UPDATE_COPY            = ( 1 << 4), // copy from temp. file to def file failed
    
    // Network
    ERRORS_NETWORK_DOWN          = ( 1 << 10 ), // unable to communicate whith network
    ERRORS_WEB_UNREACHABLE       = ( 1 << 11 ), // unable to access web server and/or NTP wall time.
    
    // Can
    ERRORS_CAN_DOWN              = ( 1 << 16 ), // error on can bus
    
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

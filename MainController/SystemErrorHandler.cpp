//
//  ErrorHandler.cpp
//  Broadway_core
//
//  Created by Manuel Deneu on 04/01/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#include "SystemErrorHandler.h"

SystemErrorHandler::SystemErrorHandler() :

_errorsList ( ERRORS_FREE )
{
    
}

SystemErrorHandler::~SystemErrorHandler()
{
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void SystemErrorHandler::addError( const Error_flags error)
{
    _errorsList = ( Error_flags ) ( _errorsList | error );
}

void SystemErrorHandler::removeError( const Error_flags error)
{
    _errorsList = ( Error_flags ) ( _errorsList & ~error );
}

bool SystemErrorHandler::isErrorSet( const Error_flags error) const
{
    return _errorsList & error;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void SystemErrorHandler::dumpSystemReportOnLog() const
{
    Log::log("##########################");
    Log::log("Begin system report :");
    
    const bool dataFound   = !isErrorSet( ERRORS_DATAFILE_NOTFOUND );
    const bool dataParseOk = !isErrorSet( ERRORS_DATAFILE_PARSING );
    
    const bool urlValid = !isErrorSet( ERROR_INVALID_URL );
    const bool updateInspect = !isErrorSet( ERROR_UPDATE_INSPECT );
    const bool updateCopy = !isErrorSet( ERROR_UPDATE_COPY );
    
    Log::log("Data File    -> %s", dataFound? "Present" : "not found");
    Log::log("Data Parsing -> %s", dataParseOk? "Ok" : "has errors");
    Log::log("-------");
    Log::log("URL valid      -> %s", urlValid? "Ok" : "invalid");
    Log::log("Update inspect -> %s", updateInspect? "Ok" : "invalid");
    Log::log("Update copy    -> %s", updateCopy? "Ok" : "failed");
    
    Log::log("##########################");
}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

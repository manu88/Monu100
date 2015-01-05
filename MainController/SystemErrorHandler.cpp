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
    
    const bool xmlFound   = !isErrorSet( ERRORS_XMLFILE_NOTFOUND );
    const bool xmlParseOk = !isErrorSet( ERRORS_XMLFILE_PARSING );
    
    Log::log("XML File    -> %s", xmlFound? "Present" : "not found");
    Log::log("XML Parsing -> %s", xmlParseOk? "Ok" : "has errors");

    
    Log::log("##########################");
}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

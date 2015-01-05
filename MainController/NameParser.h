//
//  NameParser.h
//  Broadway_core
//
//  Created by Manuel Deneu on 04/01/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#ifndef __Broadway_core__NameParser__
#define __Broadway_core__NameParser__

#include <iostream>
#include "Config.h"
#include "SystemErrorHandler.h"

class NameParser
{
public :
    NameParser( SystemErrorHandler &errorHandler );
    ~NameParser();
    
    bool parseXml( const std::string &file );
    
private:
    SystemErrorHandler *_errorHandler;
};

#endif /* defined(__Broadway_core__NameParser__) */

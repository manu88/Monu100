//
//  NameParser.cpp
//  Broadway_core
//
//  Created by Manuel Deneu on 04/01/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#include "NameParser.h"


NameParser::NameParser( SystemErrorHandler &errorHandler ) :
_errorHandler( &errorHandler)
{
    
}

NameParser::~NameParser()
{
    
}

bool NameParser::parseXml( const std::string &file )
{
    if (!FileSystem::fileExists( file ) )
    {
        Log::log("Unable to find '%s' file" , file.c_str() );
        
        _errorHandler->addError( ERRORS_XMLFILE_NOTFOUND );
        return false;
    }
    
    XMLParser x;

    
    if ( !x.parseFile( file ))
    {
        Log::log("\n unable to parse XML file '%s'" , file.c_str() );

        _errorHandler->addError(ERRORS_XMLFILE_PARSING);
        return false;
    }
    
/*
    g.setPlateform(  MakeGenerator::getPlateformForName(x.getAttributeForTag("targetPlateform", "value")) );
    
    g.setVersion( x.getAttributeForTag("version", "value") );
    
    g.setDebugConfig( x.getAttributeForTag("targetConfig", "value") == NAME_DEBUG ? true : false );
    
    g.setTargetName( x.getAttributeForTag("targetName", "value") );
*/
    
    return true;
    
}
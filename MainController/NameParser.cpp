//
//  NameParser.cpp
//  Broadway_core
//
//  Created by Manuel Deneu on 04/01/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#include <assert.h>
#include "NameParser.h"

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

NameParser::NameParser( SystemErrorHandler &errorHandler ) :
_errorHandler( &errorHandler)
{
    
}

NameParser::~NameParser()
{
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

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
  
    auto list = x.getNodeListForName("item");
    
    _nameList.clear();
    
    const long elementSize = list.size();
    
    for ( auto item : list)
    {
        const std::string nom     = XMLParser::getAttributeForName( item, "nom");
        const std::string prenom  = XMLParser::getAttributeForName( item, "prenom");
        const std::string mention = XMLParser::getAttributeForName( item, "mention");
        const std::string jour    = XMLParser::getAttributeForName( item, "jour");
        const std::string date    = XMLParser::getAttributeForName( item, "date");
        
        Day _day = dayFromExplicitFrench( jour );
        
        Date _date = dateFromStringWithDelimiter( date, '/');
        _date.day = _day;
        
    
        _nameList.push_back( NameItem(nom , prenom , mention , _date ) );
        


        
    }
    
    assert( elementSize == _nameList.size() );
    
    return true;
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void NameParser::inspectCurrentList()
{
    sortByDate();
    
    for (auto name : _nameList )
    {
        Log::log("-------------");
        Log::log("Got one '%s' '%s' mention='%s' " , name.prenom.c_str() , name.nom.c_str() , name.mention.c_str() );
        Log::log("Date : %s " , name.date.toString().c_str() );
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void NameParser::sortByDay()
{
    auto predicateByDay = [] ( const NameItem &first,const NameItem &second ) -> bool
    {
        // true if first goes before second
        return first.date.day < second.date.day;
    };
    
    _nameList.sort( predicateByDay );
    
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void NameParser::sortByDate()
{
    auto predicateByDate = [] ( const NameItem &first,const NameItem &second ) -> bool
    {
        // true if first goes before second
        return first.date < second.date;
    };
    
    _nameList.sort( predicateByDate );
}





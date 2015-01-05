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
#include <list>

#include "Config.h"
#include "SystemErrorHandler.h"

/* **** **** **** **** **** **** **** **** **** */

struct NameItem
{
    NameItem( const std::string &nom , const std::string &prenom , const std::string &mention , Date date):
    nom     ( nom),
    prenom  ( prenom),
    mention ( mention ),
    date    ( date )
    {}
    
    /* *** */
    
    std::string nom;
    std::string prenom;
    std::string mention;
    
    Date date;
};

/* **** **** **** **** **** **** **** **** **** */

class NameParser
{
public :
    NameParser( SystemErrorHandler &errorHandler );
    ~NameParser();
    
    bool parseXml( const std::string &file );
    
    void inspectCurrentList();
    
    void sortByDay();
    void sortByDate();
    
private:
    
    std::list<NameItem> _nameList;
    
    SystemErrorHandler *_errorHandler;
};

#endif /* defined(__Broadway_core__NameParser__) */

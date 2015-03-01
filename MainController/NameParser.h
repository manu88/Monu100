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
    NameItem( const std::string &nom_ , const std::string &prenom_ , const std::string &mention_ , Date date_):
    nom     ( nom_     ),
    prenom  ( prenom_  ),
    mention ( mention_ ),
    date    ( date_    )
    {}
    
    NameItem( NameItem const& r):
    nom     ( r.nom),
    prenom  ( r.prenom),
    mention ( r.mention ),
    date    ( r.date )
    {
        
    }
    
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
    typedef std::list<NameItem>::iterator NameIterator;
    
    NameParser( SystemErrorHandler &errorHandler );
    ~NameParser();
    
    bool inspectJSON( const std::string &file );
    
    bool parseJSON( const std::string &file );
    
    void inspectCurrentList();
    
    void sortByDay();
    void sortByDate();
    
    const NameItem &getNextName()
    {
        if ( _nameIter == _nameList.end() )
            _nameIter = _nameList.begin();

        return *_nameIter++;
    }
    
    const NameItem &getNextItemForDay(const Day day)
    {
        while (_nameIter->date.day != day)
        {
            _nameIter++;
            
            if (_nameIter == _nameList.end() )
                _nameIter = _nameList.begin();
        }
        
        return *_nameIter++;
    }
    
private:
    
    std::list<NameItem> _nameList;
    NameIterator        _nameIter;
    
    SystemErrorHandler *_errorHandler;
};

#endif /* defined(__Broadway_core__NameParser__) */

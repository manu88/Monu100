//
//  main.cpp
//  Monument
//
//  Created by Manuel Deneu on 04/01/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#include <iostream>

#include "Config.h"
#include "MainController.h"



int main(int argc, const char * argv[])
{

    ScopedPlateformConfig p;
/*
    Log::addLocalLogger();
    
    JSONParser parser;
    
    if (parser.parseFile("test.json"))
    {
        Log::log("parse OK");
        
        auto node = parser.getNode("GlossList");
        
        
        if ( node )
        {   
            Log::log(" Node found");
            
            if (node->isArray() )
            {
                const int size = node->getArraySize();
                
                Log::log("is ARRAY %i" , size );
                
                for ( int i = 0; i< size ; i++)
                {
                    auto item = node->getArrayItem( i );
                    
                    if ( item->isString() )
                        Log::log("value %i is '%s'" , i , item->getString().c_str() );
                }
            }
        }
        
        else
            Log::log(" Node _NOT_ found");
        
        
        
    }
    else
    {
        Log::log("Error while parsing file ");        
    }

*/
    
    std::string configFile = "config_monument.txt";
    
    if ( argc > 1)
        configFile = argv[1];
    
    
    { // nested scope for Broadway to be deleted before cleanupLogger
        MainController b( configFile );
        
        if ( !b.run() )
        {
            
        }
    }

    
    Log::cleanupLogger();


    return 0;
}


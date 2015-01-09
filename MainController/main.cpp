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
    
    Log::addLocalLogger();
    
    JSONParser parser;
    
    if (parser.parseFile("test.json"))
    {
        Log::log("parse OK");
        
        JSONParser::Node node;
        
        if ( (node = parser.getNode("testNum2")) )
        {
            Log::log(" Node found");
            
            if (node.isNumber() )
                Log::log("is number %f" , node.getDouble());
        }
        
        else
            Log::log(" Node _NOT_ found");
        
        if ( (node = parser.getNode("testNum4")) )
        {
            Log::log(" Node found");
            
            if (node.isNumber() )
                Log::log("is number %f" , node.getDouble());
        }
        else
            Log::log(" Node _NOT_ found");
        
        
        if ( (node = parser.getNode("title")) )
        {
            Log::log(" Node found");
            
            if ( node.isString() )
                Log::log("is string '%s'" , node.getString().c_str() );
            
            
        }
        else
            Log::log(" Node _NOT_ found");
        
        
    }
    else
    {
        Log::log("Error while parsing file ");        
    }
    

    
/*
    
    std::string configFile = "config_monument.txt";
    
    if ( argc > 1)
        configFile = argv[1];
    
    
    { // nested scope for Broadway to be deleted before cleanupLogger
        MainController b( configFile );
        
        if ( !b.run() )
        {
            
        }
    }
 */
    
    Log::cleanupLogger();


    return 0;
}


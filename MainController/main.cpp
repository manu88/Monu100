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


//
//  MainController.cpp
//  Broadway_core
//
//  Created by Manuel Deneu on 04/01/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#include "MainController.h"


MainController::MainController( const std::string &configFile ):
_nameParser( _errorHandler ),

_shouldQuit ( false ),
_configFile ( configFile ),

_globalCheckTimerID ( -1 )
{
    
    _scheduler.setDelegate( this );
    
    
}

/* **** **** **** **** **** **** **** **** */

MainController::~MainController()
{
    
}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

bool MainController::parseConfigFile()
{
    Database<std::string> config;
    

    
    if ( config.parseFile(_configFile, '=') )
    {
        if ( config.itemExists("XmlFile") )
        {
            _xmlFile = config.getValueForItemName<std::string>("XmlFile");
            Log::log("Will use XML file : '%s'", _xmlFile.c_str() );
            
        }
        
        if (config.itemExists("ErrorCheckTimerInMin"))
            _delayBeforeNextGlobalCheck = Timecode( 0,
                                                    atoi( config.getValueForItemName<std::string>("ErrorCheckTimerInMin").c_str() ),
                                                    0,
                                                    0
                                                   );
        
        if (config.itemExists("WatchdogPingIntervalInSec"))
        {

            _pingInterval = Timecode( 0,
                                      0,
                                      atoi( config.getValueForItemName<std::string>("WatchdogPingIntervalInSec").c_str() ),
                                      0
                                     );
        }

    }
    
    

    return true;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

bool MainController::run()
{
    if ( parseConfigFile() )
    {
        _scheduler.start();
        
        
        Controllers::waitForAllControllersToBeReady();
        
        _globalCheckTimerID = _scheduler.registerTimedEvent(_delayBeforeNextGlobalCheck, _delayBeforeNextGlobalCheck, false);

        _pingTimerID = _scheduler.registerTimedEvent(_pingInterval, _pingInterval, false);
        
        printf("\n timer = %i",_pingTimerID);
        
        inspectAndLoadNamesIfNeeded();
        

        while ( _shouldQuit == false )
        {
            // idle for now
        }

        _scheduler.stop();
        
        
        
        Controllers::waitForAllControllersToFinish();
    }
        
    
    return false;
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

bool MainController::inspectAndLoadNamesIfNeeded()
{
    
    if ( _nameParser.parseXml( _xmlFile ) )
    {
        
        return true;
    }
    
    // something happened, better chek _errorHandler
    
    if (_errorHandler.hasErrors() )
    {
        _errorHandler.dumpSystemReportOnLog();
    }
    
    return false ;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainController::dayHasChanged()
{
    Date newDate;
    if ( newDate != _currentDate )
    {
        _currentDate.update();
        Log::log("Day has changed");
    }
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainController::scheduledEventReceived( TimedEvent &event)
{
    dayHasChanged();
 
    printf("\n timer id = %i" , event.timerId );
    
    if ( event.timerId == _globalCheckTimerID)
    {
        _errorHandler.dumpSystemReportOnLog();
    }
    
    else if ( event.timerId == _pingTimerID )
    {
        printf("\n ping to watchdog");
    }
    
    else
        printf("\n ?");
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */


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

_shouldQuit    ( false ),
_shouldRestart ( false ),
_configFile ( configFile ),

_server     ( "127.0.0.1"),
_oscOutPort ( 9000 ),
_oscInPort  ( 7000 ),

_globalCheckTimerID ( -1 ),
_pingTimerID        ( -1 ),

_offsetBeforeUpdating ()
{
    
    _scheduler.setDelegate( this );
    _network.setDelegate( this );
    
    
}

/* **** **** **** **** **** **** **** **** */

MainController::~MainController()
{
    
}


/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

bool MainController::parseConfigFile()
{
    Database<std::string> config;
    
    Log::cleanupLogger();
    
    if ( config.parseFile(_configFile, '=') )
    {
        if ( config.itemExists("LogOnStdOut") && atoi( config.getValueForItemName<std::string>("LogOnStdOut").c_str() ) )
        {
            Log::addLocalLogger();
            Log::log("Use stdout logger ");
        }
        
        if ( config.itemExists("LogOnFile") && atoi( config.getValueForItemName<std::string>("LogOnFile").c_str() ) )
        {
            Log::addFileLogger( "monument_log.txt" );
            Log::log("Use file logging " );
        }
        
        
        if ( config.itemExists("XmlFile") )
        {
            _xmlFile = config.getValueForItemName<std::string>("XmlFile");
            Log::log("Will use XML file : '%s'", _xmlFile.c_str() );
            
        }
        
        if (config.itemExists("ErrorCheckTimerInMin"))
        {
            _delayBeforeNextGlobalCheck = Timecode( 0,
                                                    atoi( config.getValueForItemName<std::string>("ErrorCheckTimerInMin").c_str() ),
                                                    0,
                                                    0
                                                   );
            
        }
        if (config.itemExists("WatchdogPingIntervalInSec"))
        {

            _pingInterval = Timecode( 0,
                                      0,
                                      atoi( config.getValueForItemName<std::string>("WatchdogPingIntervalInSec").c_str() ),
                                      0
                                     );
        }
        
        // osc
        if ( config.itemExists( "OSCServer" ) )
            _server = config.getValueForItemName<std::string>("OSCServer");
        
        if ( config.itemExists( "OSCOutPort" ) )
            _oscOutPort = atoi( config.getValueForItemName<std::string>("OSCOutPort").c_str() );
        
        if ( config.itemExists( "OSCInPort" ) )
            _oscInPort = atoi( config.getValueForItemName<std::string>("OSCInPort").c_str() );
        
        if ( config.itemExists( "UpdateList_TimeOffset" ) )
        {
            _offsetBeforeUpdating = Timecode( 0,
                                              0,
                                              atoi( config.getValueForItemName<std::string>("UpdateList_TimeOffset").c_str() ),
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
        _network.start();
        
        
        Controllers::waitForAllControllersToBeReady();
        
        _globalCheckTimerID = _scheduler.registerTimedEvent(_delayBeforeNextGlobalCheck, _delayBeforeNextGlobalCheck, false);
        _pingTimerID        = _scheduler.registerTimedEvent(_pingInterval,               _pingInterval, false);
                
        _network.addPort( 7000 );
        
        inspectAndLoadNamesIfNeeded();
        

        _shouldQuit    = false;
        _shouldRestart = false;
        while ( _shouldQuit == false )
        {
            // idle for now
        }

        _scheduler.stop();
        _scheduler.removeAllEvents();
        
        _network.stop();
        _network.removeAllSockets();
        
        Controllers::waitForAllControllersToFinish();
    }
    
    if (_shouldRestart)
        run();
        
    
    return false;
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

bool MainController::inspectAndLoadNamesIfNeeded()
{
    
    if ( _nameParser.parseXml( _xmlFile ) )
    {
//        _nameParser.inspectCurrentList();
//        _nameParser.sortByDate();
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
 
 //   printf("\n timer id = %i" , event.timerId );
    
    if ( event.timerId == _globalCheckTimerID )
    {
//        printf("\n ping to check state");
    }
    
    else if ( event.timerId == _pingTimerID )
    {
        _network.sendOSC("127.0.0.1", 9000, "/ping", ArgumentsArray() );   
    }
    
    else
        printf("\n ?");
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainController::oscReceived( const std::string &ipAddress ,
                                  const int port,
                                  const std::string & addressPattern,
                                  const ArgumentsArray &arguments)
{
    if (addressPattern == "/quit")
    {
        _shouldQuit = true;
        _shouldRestart = false;
    }
    
    else if (addressPattern == "/restart")
    {
        _shouldQuit    = true;
        _shouldRestart = true;
    }
    
    else if ( addressPattern == "/dumpReport")
    {
        _errorHandler.dumpSystemReportOnLog();
    }
    
    else if ( addressPattern == "/next")
    {
        const std::string jour = arguments.getValueAtIndex<std::string>(0);
        
        const NameItem name = _nameParser.getNextItemForDay( dayFromExplicitFrench(jour)); //getNextName();
        
        Log::log("-------------");
        Log::log("'%s' '%s' mention='%s' " , name.prenom.c_str() , name.nom.c_str() , name.mention.c_str() );
        Log::log("Date : %s " , name.date.toString().c_str() );
    }
    
    
    
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

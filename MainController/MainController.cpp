    //
//  MainController.cpp
//  Broadway_core
//
//  Created by Manuel Deneu on 04/01/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//



#include "MainController.h"


MainController::MainController( const std::string &configFile ) :

_nameParser( _errorHandler ),

_shouldQuit    ( false ),
_shouldRestart ( false ),
_isActive      ( false ),

_configFile ( configFile ),

_server     ( "127.0.0.1"),
_oscOutPort ( 9000 ),
_oscInPort  ( 7000 ),

_globalCheckTimerID ( -1 ),
_pingTimerID        ( -1 ),

_can( nullptr ),

_activeLineInput ( -1 ),
_pingOutput      ( -1 ),
_shutdownInput   ( -1 ),

_gpShutdown ( nullptr ),
_gpActive   ( nullptr ),

_offsetBeforeUpdating ()
{
    WebFetcher::init();
    _currentDate.day =  Friday;
    
    _scheduler.setDelegate( this );
    _network.setDelegate( this );
    _interface.setDelegate( this );
    
}

/* **** **** **** **** **** **** **** **** */

MainController::~MainController()
{
    WebFetcher::deInit();
    
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
            std::string fileLog = "monument_log.txt";
            
            if ( config.itemExists("FileLog") )
                fileLog = config.getValueForItemName<std::string>("FileLog");
            
            Log::addFileLogger( fileLog );
            
            Log::log("Use file logging '%s' " , fileLog.c_str() );
        }
        
        /* **** **** **** **** DATA stuff **** **** **** **** **** */
        
        if ( config.itemExists("DataFile") )
        {
            _dataFile = config.getValueForItemName<std::string>("DataFile");
            Log::log("Will use JSON file : '%s'", _dataFile.c_str() );
            
        }
        
        if ( config.itemExists("TempDataFile") )
        {
            _tempDataFile = config.getValueForItemName<std::string>("TempDataFile");
            Log::log("Will use temp JSON file : '%s'", _tempDataFile.c_str() );
            
        }
        
        if ( config.itemExists("JsonURL") )
        {
            _dataUrl = config.getValueForItemName<std::string>("JsonURL");
            Log::log("Will use XML url : '%s'", _dataUrl.c_str() );
            
        }
        
        /* **** **** **** **** **** TIMER stuff **** **** **** **** */
        
        
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
        
        if ( config.itemExists( "UpdateList_TimeOffset" ) )
        {
            
            _offsetBeforeUpdating = Timecode( config.getValueForItemName<std::string>("UpdateList_TimeOffset") );
            Log::log("will update data at %s" , _offsetBeforeUpdating.getString().c_str() );
        }
        
        /* **** **** **** **** in/out pins **** **** **** **** **** */
        
        if ( config.itemExists("ActiveLinePin") )
            _activeLineInput = config.getValueForItemNameAsInt("ActiveLinePin");
        
        if ( config.itemExists("PingPin") )
            _pingOutput = config.getValueForItemNameAsInt("PingPin");
        
        if ( config.itemExists("ShutdownPin") )
            _shutdownInput = config.getValueForItemNameAsInt("ShutdownPin");
        
        /* **** **** **** **** OSC **** **** **** **** **** */
        
        // osc
        if ( config.itemExists( "OSCServer" ) )
            _server = config.getValueForItemName<std::string>("OSCServer");
        
        if ( config.itemExists( "OSCOutPort" ) )
            _oscOutPort = config.getValueForItemNameAsInt("OSCOutPort");
        
        if ( config.itemExists( "OSCInPort" ) )
            _oscInPort = config.getValueForItemNameAsInt("OSCInPort");
        

        

    }
    
    

    return true;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

bool MainController::prepareCan()
{
    DEBUG_ASSERT( _interface.isRunning() );
    
    _can = _interface.addCanConnexion("can0");
    
    if ( !_can->connect() )
        _errorHandler.addError( ERRORS_CAN_INTERFACE );
    
    return _errorHandler.hasCanErrors() == false;
}

bool MainController::prepareNetwork()
{
    DEBUG_ASSERT( _network.isRunning() );
    
    if ( !_network.addPort( 7000 ) )
        _errorHandler.addError( ERRORS_NETWORK_INTERFACE );
    
    return _errorHandler.hasNetworkErrors() == false;
}

bool MainController::prepareGpio()
{
    DEBUG_ASSERT( _interface.isRunning() );
    
    _gpActive   =  _interface.addGpioInput( _activeLineInput , InputDirect );
    _gpShutdown =  _interface.addGpioInput( _shutdownInput   , InputDirect );
    
    return _errorHandler.hasGpioErrors() == false;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainController::wakeMainLoop()
{
    _wakeUp.notify_all();
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

bool MainController::run()
{
    if ( parseConfigFile() )
    {
        _scheduler.start();
        _network.start();
        _interface.start();
        
        Controllers::waitForAllControllersToBeReady();
        
        _globalCheckTimerID = _scheduler.registerTimedEvent(_delayBeforeNextGlobalCheck, _delayBeforeNextGlobalCheck, false);
        _pingTimerID        = _scheduler.registerTimedEvent(_pingInterval,               _pingInterval, false);
                

        prepareCan();
        prepareNetwork();
        prepareGpio();

        
        
        fetchJSONFile();
        
        inspectAndLoadNamesIfNeeded();
        

        _shouldQuit    = false;
        _shouldRestart = false;
        
        ScopedLock lock( _sync );
        while ( _shouldQuit == false )
        {

            _wakeUp.wait( lock );
            
            Log::log("main loop unlocked");
            
        }

        _scheduler.stop();
        _scheduler.removeAllEvents();
        
        _network.stop();
        _network.removeAllSockets();
        
        _interface.stop();

        
        Controllers::waitForAllControllersToFinish();
    }
    
    if (_shouldRestart)
        run();
        
    
    return false;
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

bool MainController::fetchJSONFile()
{
    WebFetcher f( _dataUrl , _tempDataFile , _dataFile , _errorHandler );

    if ( !f.fetch() )
    {
        Log::log("Error while fetching url '%s' to '%s'" , _dataUrl.c_str() , _tempDataFile.c_str() );

        _errorHandler.addError( ERROR_INVALID_URL );
        _errorHandler.addError( ERROR_UPDATE_INSPECT );
        _errorHandler.addError( ERROR_UPDATE_COPY );
        return false;
    }
    
    if ( !_nameParser.inspectJSON( _tempDataFile ) )
    {
        Log::log("Temp. JSON seems wrong. Can't copy");
        _errorHandler.addError( ERROR_UPDATE_INSPECT );
        _errorHandler.addError( ERROR_UPDATE_COPY );
        return false;
    }
    

    if ( !f.checkAndCopyFile() )
    {
        Log::log("Unable to copy temp JSON to def");
        _errorHandler.addError( ERROR_UPDATE_COPY );
        return false;
    }

    
    return true;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

bool MainController::inspectAndLoadNamesIfNeeded()
{
    
    if ( _nameParser.parseJSON( _dataFile ) )
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

void MainController::deepShutdown()
{
    wakeMainLoop();
    
    Log::log("system will now shutdown");
    
    _shouldQuit    = true;
    _shouldRestart = false;
    
    system("sudo shutdown -h now");
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainController::deepReboot()
{
    wakeMainLoop();
    
    _shouldQuit    = true;
    _shouldRestart = false;
    
    system( "sudo reboot ");
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainController::restart()
{
    wakeMainLoop();
    
    _shouldQuit    = true;
    _shouldRestart = true;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainController::quit()
{
    wakeMainLoop();
    
    _shouldQuit    = true;
    _shouldRestart = false;
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainController::activityChangedTo( bool isActive)
{
    if ( isActive != _isActive )
    {
        
        // go to inactive state
        if ( _isActive )
        {
            Log::log("system goes to inactive state ...");
        }
        
        // go to active state
        else if ( !_isActive )
        {
            Log::log("system goes to active state ...");
        }
        
        _isActive = isActive;
        
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainController::dayHasChanged()
{
    Date newDate;
    
    if (    _offsetBeforeUpdating <= Timecode::getCurrent()
         && newDate.day != _currentDate.day
       )
    {
        _currentDate.update();
        Log::log("Day has changed");
    }
    else
        printf(" Time not reached yet ");
    
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
                                  const VariantList &arguments)
{
    if (addressPattern == "/quit")
    {
        quit();
    }
    
    else if ( addressPattern == "/test")
    {
        _wakeUp.notify_all();
    }
    
    else if (addressPattern == "/restart")
    {
        restart();
    }
    
    else if (addressPattern == "/reboot")
    {
        deepReboot();
    }
    
    else if ( addressPattern == "/fetch")
    {
        fetchJSONFile();
    }
    
    else if ( addressPattern == "/dumpReport")
    {
        _errorHandler.dumpSystemReportOnLog();
    }
    
    else if ( addressPattern == "/next")
    {
        const std::string jour = arguments.getValueAtIndex<std::string>(0);

        _currentDate.day = dayFromExplicitFrench( jour );
        /*
        const NameItem name = _nameParser.getNextItemForDay( dayFromExplicitFrench(jour)); //getNextName();
        
        Log::log("-------------");
        Log::log("'%s' '%s' mention='%s' " , name.prenom.c_str() , name.nom.c_str() , name.mention.c_str() );
        Log::log("Date : %s " , name.date.toString().c_str() );
         */
    }

    /* **** ***** */
    
    // watchdog emulation
    
    else if ( addressPattern == "/activeline")
    {
        const bool isActive = arguments.getIntValueAtIndex(0);
        
        activityChangedTo( isActive );
    }
    
    
    
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

void MainController::inputChanged( const InterfaceEvent *event )
{
    // shutdown request
    if ( event == _gpShutdown )
    {
        deepShutdown();
    }
    
    // Active line : HIGH is active, LOW if not
    else if (event == _gpActive )
    {
        activityChangedTo( _gpActive->read() == high );
    }
}

/* **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** **** */

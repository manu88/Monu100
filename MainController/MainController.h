//
//  MainController.h
//  Broadway_core
//
//  Created by Manuel Deneu on 04/01/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#ifndef __Broadway_core__MainController__
#define __Broadway_core__MainController__

#include <iostream>
#include <condition_variable>

#include "Config.h"
#include "NameParser.h"
#include "SystemErrorHandler.h"
#include "WebFetcher.h"

class MainController : public SchedulerDelegate,
                       public NetworkControllerDelegate,
                       public InterfaceControllerDelegate
{
public:
    MainController( const std::string &configFile );
    ~MainController();
    
    bool run();
    
    
    void scheduledEventReceived( TimedEvent &event);
    void oscReceived(const std::string &ipAddress , const int port,const std::string & addressPattern, const ArgumentsArray &arguments);
    void inputChanged( const InterfaceEvent *event );
    
private:
    
    void wakeMainLoop();
    
    bool parseConfigFile();
    
    bool fetchJSONFile();
    bool inspectAndLoadNamesIfNeeded();
    
    bool prepareCan();
    bool prepareNetwork();
    bool prepareGpio();
    
    void dayHasChanged();
    
    void deepShutdown();
    void deepReboot();
    void restart();
    void quit();
    
    void activityChangedTo( bool isActive);
    
    // flags
    bool        _shouldQuit;
    bool        _shouldRestart;
    Date        _currentDate;
    bool        _isActive;

    // config stuffs
    std::string _configFile;
    
    std::string _dataFile;
    std::string _tempDataFile;
    std::string _dataUrl;
    
    // osc
    std::string _server;
    unsigned int _oscOutPort;
    unsigned int _oscInPort;
    
    Timecode    _delayBeforeNextGlobalCheck;
    int         _globalCheckTimerID;
    
    Timecode    _pingInterval;
    int         _pingTimerID;
    
    Timecode    _offsetBeforeUpdating;
    
    // controllers
    Scheduler           _scheduler;
    NetworkController   _network;
    InterfaceController _interface;
    
    CanEvent *_can;
    
    // watchdog
    GPioPin _activeLineInput;
    GPioPin _pingOutput;
    GPioPin _shutdownInput;
    
    GpioEvent * _gpShutdown;
    GpioEvent * _gpActive;
    
    // utility
    SystemErrorHandler _errorHandler;
    NameParser         _nameParser;
    
    // threading
    
    std::condition_variable _wakeUp;
    std::mutex              _sync;
    
};
#endif /* defined(__Broadway_core__MainController__) */

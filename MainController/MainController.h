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

#include "Config.h"
#include "NameParser.h"
#include "SystemErrorHandler.h"

class MainController : public SchedulerDelegate,
                       public NetworkControllerDelegate
{
public:
    MainController( const std::string &configFile );
    ~MainController();
    
    bool run();
    
    
    void scheduledEventReceived( TimedEvent &event);
    void oscReceived(const std::string &ipAddress , const int port,const std::string & addressPattern, const ArgumentsArray &arguments);
    
private:
    
    bool parseConfigFile();
    
    bool inspectAndLoadNamesIfNeeded();
    
    void dayHasChanged();
    
    // flags
    bool        _shouldQuit;
    bool        _shouldRestart;
    Date        _currentDate;

    // config stuffs
    std::string _configFile;
    std::string _xmlFile;
    
    Timecode    _delayBeforeNextGlobalCheck;
    int         _globalCheckTimerID;
    
    Timecode    _pingInterval;
    int         _pingTimerID;
    
    // controllers
    Scheduler         _scheduler;
    NetworkController _network;
    
    // utility
    SystemErrorHandler _errorHandler;
    NameParser         _nameParser;
    
};
#endif /* defined(__Broadway_core__MainController__) */

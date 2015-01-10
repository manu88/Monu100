//
//  WebFetcher.h
//  Broadway_core
//
//  Created by Manuel Deneu on 09/01/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#ifndef __Broadway_core__WebFetcher__
#define __Broadway_core__WebFetcher__

#include <iostream>

#include "Config.h"

#include "SystemErrorHandler.h"

class WebFetcher // : public Thread
{
public:
    WebFetcher( const std::string &url , const std::string &tempFile, const std::string &fileDef, SystemErrorHandler &errorHandler );
    ~WebFetcher();

    bool fetch();
    
    bool checkAndCopyFile();
    
    static void init();
    
    static void deInit();
    
private:

    
    std::string _tempDataFile;
    std::string _destDataFile;
    std::string _url;

    
    SystemErrorHandler *_errorHandler;
    
    static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
    
    static bool s_curlInitialized;
    
};

#endif /* defined(__Broadway_core__WebFetcher__) */

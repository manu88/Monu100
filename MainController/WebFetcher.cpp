//
//  WebFetcher.cpp
//  Broadway_core
//
//  Created by Manuel Deneu on 09/01/15.
//  Copyright (c) 2015 Manuel Deneu. All rights reserved.
//

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define CURL_STATICLIB
#include <stdio.h>
#include <curl/curl.h>

#include <curl/easy.h>
#include <string>


#include "WebFetcher.h"

WebFetcher::WebFetcher(  const std::string &url , const std::string &tempFile, const std::string &fileDef, SystemErrorHandler &errorHandler ):

_tempDataFile ( tempFile ),
_destDataFile ( fileDef ),
_url          ( url ),

_errorHandler ( &errorHandler )
{

}

WebFetcher::~WebFetcher()
{
    
}

/*static*/ size_t WebFetcher::write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

bool WebFetcher::fetch()
{
    
    CURL *curl;
    FILE *fp;
    CURLcode res;
    
//    const std::string url = "https://github.com/manu88/Monu100/blob/master/MainController/list.json";
    
//    char outfilename[FILENAME_MAX] = "/Users/manueldeneu/Documents/projets/dev/Monu100/MainController/list_temp.json";
    
    curl = curl_easy_init();
    
    if ( curl )
    {
        fp = fopen(_tempDataFile.c_str() , "wb" );
        
        curl_easy_setopt(curl, CURLOPT_URL, _url.c_str() );
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        
        res = curl_easy_perform(curl);
        
        curl_easy_cleanup(curl);
        fclose(fp);
        
        return true;
    }
    
    return false;
}


bool WebFetcher::checkAndCopyFile()
{
    int fd_to, fd_from;
    char buf[4096];
    ssize_t nread;
    int saved_errno;
    
    fd_from = open( _tempDataFile.c_str() , O_RDONLY);
    if (fd_from < 0)
        return false;
    
    fd_to = open(_destDataFile.c_str() , O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd_to < 0)
        goto out_error;
    
    while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;
        
        do {
            nwritten = write(fd_to, out_ptr, nread);
            
            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                goto out_error;
            }
        } while (nread > 0);
    }
    
    if (nread == 0)
    {
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
        close(fd_from);
        
        /* Success! */
        return true;
    }
    
out_error:
    saved_errno = errno;
    
    close(fd_from);
    if (fd_to >= 0)
        close(fd_to);
    
    errno = saved_errno;

    
    
    return false;
}

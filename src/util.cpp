#include <curl/curl.h>
#include <util.hpp>
#include <iostream>
#include <future>
#include <vector>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int downloadFiles(std::vector<std::string> urls, std::vector<std::string> filepaths)
{
    if(filepaths.size() < urls.size())
    {
        std::cerr << "downloadFiles: not enough filepaths given for urls" << std::endl;
        return -1;
    }
    CURLM *multi_handle = curl_multi_init();
    int still_running = 0;
    std::vector<FILE *> pagefiles(urls.size());
    std::vector<CURL *> curl_handles(urls.size());

    for(int i = 0; i < urls.size(); i++)
    {

        curl_global_init(CURL_GLOBAL_ALL);
        
        /* init the curl session */ 
        curl_handles[i] = curl_easy_init();
        
        /* set URL to get here */ 
        curl_easy_setopt(curl_handles[i], CURLOPT_URL, urls[i].c_str());
        
        /* Switch on full protocol/debug output while testing */ 
        curl_easy_setopt(curl_handles[i], CURLOPT_VERBOSE, 1L);
        
        /* disable progress meter, set to 0L to enable and disable debug output */ 
        curl_easy_setopt(curl_handles[i], CURLOPT_NOPROGRESS, 1L);
        
        /* send all data to this function  */ 
        curl_easy_setopt(curl_handles[i], CURLOPT_WRITEFUNCTION, write_data);
        
        /* open the file */ 
        pagefiles[i] = fopen(filepaths[i].c_str(), "wb");
        
        if(pagefiles[i]) {
            /* write the page body to this file handle */ 
            curl_easy_setopt(curl_handles[i], CURLOPT_WRITEDATA, pagefiles[i]);
        }

        /* add to multi operation */
        curl_multi_add_handle(multi_handle, curl_handles[i]);
    }
        /* get it! */ 
        curl_multi_perform(multi_handle, &still_running);

        while(still_running)
        {
            struct timeval timeout;
            int rc; /* select() return code */
            CURLMcode mc; /* curl_multi_fdset() return code */

            fd_set fdread;
            fd_set fdwrite;
            fd_set fdexcep;
            int maxfd = -1;

            long curl_timeo = -1;

            FD_ZERO(&fdread);
            FD_ZERO(&fdwrite);
            FD_ZERO(&fdexcep);

            /* set a suitable timeout to play around with */
            timeout.tv_sec = 1;
            timeout.tv_usec = 0;

            curl_multi_timeout(multi_handle, &curl_timeo);
            if(curl_timeo >= 0) {
              timeout.tv_sec = curl_timeo / 1000;
              if(timeout.tv_sec > 1)
                timeout.tv_sec = 1;
              else
                timeout.tv_usec = (curl_timeo % 1000) * 1000;
            }

            /* get file descriptors from the transfers */
            mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

            if(mc != CURLM_OK) {
              fprintf(stderr, "curl_multi_fdset() failed, code %d.\n", mc);
              break;
            }

            /* On success the value of maxfd is guaranteed to be >= -1. We call
               select(maxfd + 1, ...); specially in case of (maxfd == -1) there are
               no fds ready yet so we call select(0, ...) --or Sleep() on Windows--
               to sleep 100ms, which is the minimum suggested value in the
               curl_multi_fdset() doc. */

            if(maxfd == -1) {
              /* Portable sleep for platforms other than Windows. */
              struct timeval wait = { 0, 100 * 1000 }; /* 100ms */
              rc = select(0, NULL, NULL, NULL, &wait);
            }
            else {
              /* Note that on some platforms 'timeout' may be modified by select().
                 If you need access to the original value save a copy beforehand. */
              rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
            }

            switch(rc) {
            case -1:
              /* select error */
              break;
            case 0:
            default:
              /* timeout or readable/writable sockets */
              curl_multi_perform(multi_handle, &still_running);
              break;
            }
        }
        
        /* cleanup curl stuff */ 
        curl_multi_cleanup(multi_handle);

        /* close the header file */ 
        for(int i = 0; i != pagefiles.size(); i++)
        {
            fclose(pagefiles[i]);
            curl_easy_cleanup(curl_handles[i]);
        }

        curl_global_cleanup();
    
    return 0;
}


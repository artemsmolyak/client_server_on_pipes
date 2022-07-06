#ifndef PipeServer_h
#define PipeServer_h

#include <iostream>
#include <vector>
#include <poll.h>
#include <unistd.h> //pipe
#include <fcntl.h> //F_GETFL
 

// can't in blocking mode
class PipeServer{
    
private:
    const unsigned int timeout_ = 1000;
    std::atomic<bool> quitFlag_ = false; 
    
    int pipeHandleOut_ = 0;
    int pipeHandleIn_ = 0;
    
    const size_t bufferSize = 1024;
    
private:
    void waitRequest();
    void writeResponse(const std::vector<unsigned char>& data);
    
public:
    PipeServer( int pipeIn, int pipeOut);
    
    void start();
    void stop();
};


#endif /* PipeServer_h */


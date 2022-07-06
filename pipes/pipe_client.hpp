#ifndef pipe_client_h
#define pipe_client_h

#include <iostream>
#include <vector>
#include <poll.h>
#include <unistd.h> //pipe
#include <fcntl.h> //F_GETFL

// can't in blocking mode
class PipeClient{
    
private:
    const unsigned int timeout_ = 1000;
    
    int pipeHandleOut_ = 0;
    int pipeHandleIn_ = 0;
    
    const size_t bufferSize = 1024;
    
private:
    void getResponse();
    
public:
    PipeClient( int pipeIn, int pipeOut);
    
    void sendRequest( const std::string& message );
};


#endif /* pipe_client_h */


 

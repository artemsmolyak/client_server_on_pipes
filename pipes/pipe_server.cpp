#include "pipe_server.hpp" 


PipeServer::PipeServer( int pipeIn, int pipeOut)
:pipeHandleIn_(pipeIn),
pipeHandleOut_(pipeOut)
{}

void PipeServer::start(){
    waitRequest();
}

void PipeServer::waitRequest(){
    
    struct pollfd fds[1];
    fds[0].fd = pipeHandleIn_;
    fds[0].events = POLLIN;
    
    while( !quitFlag_ ){
        
        int rc = poll( fds, 1, timeout_ );
        if ( rc < 0 ){
            std::cout << "poll error" << std::endl;
        }
        
        if ( rc == 0 ){
            std::cout << "server timeout" << std::endl;
            continue;
        }
        
        if ( fds[0].revents & POLLIN ){
            fds[0].revents = 0;
            
            std::vector<unsigned char> buffer(bufferSize);
            ssize_t res = read(pipeHandleIn_, buffer.data(), buffer.size());
            if ( res <= 0 ){
                throw std::runtime_error("server read error: " + std::to_string(errno) );
            }
            std::cout << "server get request: " << buffer.data() << std::endl;
             
            std::transform(buffer.data(),
                           buffer.data() + res,
                           buffer.data(),
                           [](unsigned char c){ return std::toupper(c); } );
            
            writeResponse(buffer);
        }
    }
}


void PipeServer::writeResponse(const std::vector<unsigned char>& data){
    ssize_t res = write(pipeHandleOut_, data.data(), data.size());
    if ( res <= 0 ){
        throw std::runtime_error("send error: " + std::to_string(errno) );
    }
    std::cout << "server sent response: " << data.data() << std::endl;
}


#include "pipe_client.hpp" 

PipeClient::PipeClient( int pipeIn, int pipeOut)
:pipeHandleIn_(pipeIn),
pipeHandleOut_(pipeOut)
{}

void PipeClient::sendRequest( const std::string& message ){
    
    while(true){
        
        ssize_t res = write(pipeHandleOut_, message.data(), message.size());
        if ( res <= 0 ){
            throw std::runtime_error("send errer: " + std::to_string(errno) );
        }
        std::cout << "client sent: "<< message.data() << std::endl;
            
        getResponse();
        sleep(2);
    }
}


void PipeClient::getResponse(){
    
    struct pollfd fds[1];
    fds[0].fd = pipeHandleIn_;
    fds[0].events = POLLIN;
    
    int rc = poll( fds, 1, timeout_ );
    if ( rc < 0 ){
        std::cout << "poll error" << std::endl;
    }
    
    if ( rc == 0 ){
        std::cout << "client timeout" << std::endl;
        return;
    }
    
    if ( fds[0].revents & POLLIN ){
        
        fds[0].revents = 0;
        
        std::vector<unsigned char> buffer(1024);
        ssize_t res = read(pipeHandleIn_, buffer.data(), buffer.size());
        if ( res <= 0 ){
            throw std::runtime_error("client read error: " + std::to_string(errno) );
        }
        std::cout << "client get response: " << buffer.data() << std::endl;
    }
}

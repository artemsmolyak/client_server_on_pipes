#include "pipe_client.hpp"
#include "pipe_server.hpp"

#include <iostream>
#include <future>
#include <unistd.h> //pipe

 
// unnamed pipes
enum { READ = 0, WRITE = 1 };

void preparePipes( int descriptorsToServer[2], int descriptorsToClient[2] ){
    
    int res = pipe( descriptorsToServer );
    if ( res == -1 ){
        throw std::runtime_error( "error" );
    }
    int flags = fcntl( descriptorsToServer[READ], F_GETFL, 0 );
    res = fcntl( descriptorsToServer[READ], F_SETFL, flags|O_NONBLOCK );
    if ( res == -1 ){
        throw std::runtime_error( "error" );
    }
    flags = fcntl( descriptorsToServer[WRITE], F_GETFL, 0 );
    res = fcntl( descriptorsToServer[WRITE], F_SETFL, flags|O_NONBLOCK );
    if ( res == -1 ){
        throw std::runtime_error( "error" );
    }
    
    res = pipe( descriptorsToClient );
    if ( res == -1 ){
        throw std::runtime_error( "error" );
    }
    flags = fcntl( descriptorsToClient[READ], F_GETFL, 0 );
    res = fcntl( descriptorsToClient[READ], F_SETFL, flags|O_NONBLOCK );
    if ( res == -1 ){
        throw std::runtime_error( "error" );
    }
    flags = fcntl( descriptorsToClient[WRITE], F_GETFL, 0 );
    res = fcntl( descriptorsToClient[WRITE], F_SETFL, flags|O_NONBLOCK );
    if ( res == -1 ){
        throw std::runtime_error( "error" );
    }
}


int main(int argc, const char * argv[]) {
    
    int descriptorsToServer[2];
    int descriptorsToClient[2];
    
    preparePipes(descriptorsToServer, descriptorsToClient);
    
    PipeServer pipeServer( descriptorsToServer[READ], descriptorsToClient[WRITE] );
    auto pipeFuture = std::async(std::launch::async, &PipeServer::start, &pipeServer);
    
    PipeClient pipeClient( descriptorsToClient[READ], descriptorsToServer[WRITE] );
    pipeClient.sendRequest("hello server");
    
    try{
        pipeFuture.get();
    }
    catch(const std::exception& ex){
        std::cout << ex.what() << std::endl;
    }
    
    return 0;
}

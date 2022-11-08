#define TVM_EXPORTS
#include <cstdint>
#include <cmath>

#include "backend/System.h"
int32_t load_conv_kernel(System *sys) {  
    int kernel_size=8;
    int loaders=3;                               
    //int sender=2;                                                                                
    for (int sender = 0; sender < 3; ++sender) {  
        int recv = loaders;                                    
      //for (int recv = loaders; recv < 120; recv++) { 
                                      
        for (int row = 0; row < (kernel_size); ++row) {                                       
          {                                                                                   
            Request request(Request::Type::TileReceive);                                      
            request.addOperand(sys->getAddress(sender, 0, row), 256*4,                          
                               PrecisionT::Precision{0, 8, 0});                               
            request.addOperand(sys->getAddress(recv, 0, row), 256*4,                            
                               PrecisionT::Precision{0, 8, 0});                               
            sys->sendRequest(request);                                                        
          }                                                                                   
          {                                                                                   
            Request request(Request::Type::TileSend);                                         
            request.addOperand(sys->getAddress(sender, 0, row), 256*4,                          
                               PrecisionT::Precision{0, 8, 0});                               
            request.addOperand(sys->getAddress(recv, 0, row), 256*4,                            
                               PrecisionT::Precision{0, 8, 0});                               
            sys->sendRequest(request);                                                        
          }                                                                                   
        }  
      //}                                                                                   
                                                                                     
    }                                                                                         
    return 0;                                                                                 
  }  

int32_t test_multi_sender(System* sys){
    {                                                                                   
        Request request(Request::Type::TileSend);                                      
        request.addOperand(sys->getAddress(0, 0, 0), 256,                          
                            PrecisionT::Precision{0, 8, 0});                               
        request.addOperand(sys->getAddress(3, 0, 0), 0,                            
                            PrecisionT::Precision{0, 8, 0});                               
        sys->sendRequest(request);                                                        
    }
    {                                                                                   
        Request request(Request::Type::TileReceive);                                      
        request.addOperand(sys->getAddress(0, 0, 0), 256,                          
                            PrecisionT::Precision{0, 8, 0});                               
        request.addOperand(sys->getAddress(3, 0, 0), 0,                            
                            PrecisionT::Precision{0, 8, 0});                               
        sys->sendRequest(request);                                                        
    }
    {                                                                                   
        Request request(Request::Type::TileSend);                                      
        request.addOperand(sys->getAddress(1, 0, 0), 256,                          
                            PrecisionT::Precision{0, 8, 0});                               
        request.addOperand(sys->getAddress(3, 0, 0), 0,                            
                            PrecisionT::Precision{0, 8, 0});                               
        sys->sendRequest(request);                                                        
    }
    {                                                                                   
        Request request(Request::Type::TileReceive);                                      
        request.addOperand(sys->getAddress(1, 0, 0), 256,                          
                            PrecisionT::Precision{0, 8, 0});                               
        request.addOperand(sys->getAddress(3, 0, 0), 256,                            
                            PrecisionT::Precision{0, 8, 0});                               
        sys->sendRequest(request);                                                        
    }
    {                                                                                   
        Request request(Request::Type::TileSend);                                      
        request.addOperand(sys->getAddress(2, 0, 0), 256,                          
                            PrecisionT::Precision{0, 8, 0});                               
        request.addOperand(sys->getAddress(3, 0, 0), 256,                            
                            PrecisionT::Precision{0, 8, 0});                               
        sys->sendRequest(request);                                                        
    }
    {                                                                                   
        Request request(Request::Type::TileReceive);                                      
        request.addOperand(sys->getAddress(2, 0, 0), 256,                          
                            PrecisionT::Precision{0, 8, 0});                               
        request.addOperand(sys->getAddress(3, 0, 0), 256,                            
                            PrecisionT::Precision{0, 8, 0});                               
        sys->sendRequest(request);                                                        
    }
}

int32_t broadcast(System* sys){
    Config* cfg = sys->_config;
    PrecisionT::Precision precision_input = PrecisionT::Precision{0, 8, 0};
    std::vector<Request> requests;
    Request *request;
    for(int col=1; col<cfg->_meshWidth; col++){
        request = new Request(Request::Type::TileSend);
        request->addOperand(sys->getAddress(0,0,0), 256*32, precision_input); //src
        request->addOperand(sys->getAddress(col,0,0), 256*32, precision_input); //dst
        requests.push_back(*request);

        request = new Request(Request::Type::TileReceive);
        request->addOperand(sys->getAddress(0,0,0), 256*32, precision_input); //src
        request->addOperand(sys->getAddress(col,0,0), 256*32, precision_input); //dst
        requests.push_back(*request);
    }

    for(int col=0; col<cfg->_meshWidth; col++){
        for(int row=1; row<cfg->_meshHeight; row++){
            request = new Request(Request::Type::TileSend);
            request->addOperand(sys->getAddress(col,0,0), 256*32, precision_input); //src
            request->addOperand(sys->getAddress(row * cfg->_meshWidth + col,0,0), 256*32, precision_input); //dst
            requests.push_back(*request);

            request = new Request(Request::Type::TileReceive);
            request->addOperand(sys->getAddress(col,0,0), 256*32, precision_input); //src
            request->addOperand(sys->getAddress(row * cfg->_meshWidth + col,0,0), 256*32, precision_input); //dst
            requests.push_back(*request);        
        }
    }
    for (unsigned int i = 0; i < requests.size(); i++)
        sys->sendRequest(requests[i]);
    return 0;
}                                                                                         
static __attribute__((unused)) Registry::Entry &__debug__ = pimsim::registerFunc("debug", load_conv_kernel);
static __attribute__((unused)) Registry::Entry &__debug_broadcast__ = pimsim::registerFunc("debug_broadcast", broadcast);
static __attribute__((unused)) Registry::Entry &__debug_multi_sender__ = pimsim::registerFunc("debug_multi_sender", test_multi_sender);
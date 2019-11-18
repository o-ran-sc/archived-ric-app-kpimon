/*
==================================================================================

        Copyright (c) 2018-2019 SAMSUNG Intellectual Property.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
==================================================================================
*/

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <error.h>
#include <thread>
#include <map>
#include <mutex>
#include <functional>
#include <sstream>
#include <rmr/rmr.h>
#include <rmr/RIC_message_types.h>
#include <mdclog/mdclog.h>
#include <hiredis/hiredis.h>


#ifndef XAPP_UTIL
# define XAPP_UTIL

#define DEBUG 0

#define XAPP_NAME_LENGTH 128
#define PROTO_PORT_LENGTH 16
#define MAX_RETRIES 16
#define MAX_WAIT_TIME 10
#define RMR_TIMEOUT 50// in mill-seconds
#define RMR_BUFFER_SIZE 16384
#define MAX_THREADS 8

#ifdef __GNUC__
#define likely(x)  __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

void init_logger(const char  *AppName, mdclog_severity_t log_level);


class XaPP {
  
 public:

  XaPP(char *, char *, int, int);
  XaPP(char *, char *, int, int, int);
  ~XaPP(void);
  XaPP(XaPP &&) = default;  // destructor freaks out with non-copyable thread otherwise ..
  std::string getName(void);
  int  getStatus(void);

  // ideally can reduce tempate definitions to just two
  // but for now leaving it open ...
  
  // template definition to allow a user defined
  // processor to be started in multiple threads
  template <typename messageProcessor>
  void Start(messageProcessor &&);

  // template definition to allow a user defined
  // processor and  error handler if a send fails 
  // to be started in multiple threads
  template <typename messageProcessor, typename errorHandler>
  void Start(messageProcessor &&, errorHandler &&);

  // Template to allow a user defined processor to start
  // on a single thread each time it is invoked
  template <typename messageProcessor > 
  unsigned int  StartThread(messageProcessor &&);

  // Template to allow a user defined processor and 
  // error handle to start // on a single thread each time it
  // is invoked
  template <typename messageProcessor , typename errorHandler> 
  unsigned int StartThread(messageProcessor &&, errorHandler &&);

  void Stop(void);
  bool Send(int type,  int payload_len, void *payload);
  bool Send(int type, int payload_len, void *payload, unsigned char const  *meid);
  bool Send(rmr_mbuf_t * rmr_tx_message);
  void * get_rmr_context(void);
  void set_num_retries(int );
  int  get_num_retries(void );
  unsigned long get_Send_attempts(void);
  unsigned long get_Send_fails(void);
  
private:

  void init(int);
  void get_routes();
  void redisInit();
  void _error_handler(rmr_mbuf_t *); // pass through placeholder
  
  template<typename messageProcessor, typename errorHandler>
  void _workThread(messageProcessor &&, errorHandler &&, XaPP *);

  char _xapp_name[XAPP_NAME_LENGTH];
  char _proto_port[PROTO_PORT_LENGTH];
  int _redis_port;

  int _is_ready;
  bool _listen;
  int _num_retries;
  int _msg_size;
  unsigned int _num_threads;
  unsigned long _num_attempts;
  unsigned long _num_fails;

  void* _rmr_ctx;
  redisContext *c;
  std::mutex *_transmit;
  std::map <unsigned int, std::thread> thread_group;
  rmr_mbuf_t * _rmr_tx_message;
 
  bool _isRunning(void);
  
};


template <typename messageProcessor, typename errorHandler>
void XaPP::_workThread(messageProcessor && msg_fn, errorHandler && error_handler, XaPP *parent){


  // Get the thread id 
  std::thread::id my_id = std::this_thread::get_id();
  std::stringstream thread_id;
  thread_id << my_id;

  // Stats counters 
  unsigned long recvs = 0;
  unsigned long attempts = 0;
  unsigned long fails = 0;
 
  // Get the rmr context 
  void *rmr_context = parent->get_rmr_context(); 
  if (!rmr_context){
    std::string identifier = __FILE__ +  std::string(", Line: ") + std::to_string(__LINE__) ; 
    std::string error_string = identifier +  " Thread : " + thread_id.str() + " Listener cannot run : no context available";
    mdclog_write(MDCLOG_ERR, error_string.c_str(), "");
    throw error_string;
  }

  // Get buffer specific to this thread
  rmr_mbuf_t *rmr_message;
  if ( (rmr_message = rmr_alloc_msg(rmr_context, RMR_BUFFER_SIZE)) == NULL){
    // throw exception here ..
    std::string identifier = __FILE__ +  std::string(", Line: ") + std::to_string(__LINE__) ; 
    std::string reason = strerror(errno);
    std::string error_string = identifier + " Thread: " + thread_id.str() + " Error getting a buffer : " + reason;
    mdclog_write(MDCLOG_ERR, error_string.c_str(), "");
    throw error_string;
  }
  
  int num_retries = this->get_num_retries();
  
  mdclog_write(MDCLOG_INFO, "Starting thread %s",  thread_id.str().c_str());
  
  while(parent->_isRunning()){
    
      rmr_message = rmr_torcv_msg(rmr_context, rmr_message, RMR_TIMEOUT);
      //rmr_message = rmr_rcv_msg(rmr_context, rmr_message);
  
    if (rmr_message && rmr_message->state == RMR_OK){
      recvs++;
      bool res = msg_fn(c,rmr_message);
      }
      
    }

  // Clean up 
  try{
    rmr_free_msg(rmr_message);
  }
  catch(std::runtime_error &e){
    std::string identifier = __FILE__ +  std::string(", Line: ") + std::to_string(__LINE__) ; 
    std::string error_string = identifier = " Error freeing RMR message ";
    mdclog_write(MDCLOG_ERR, error_string.c_str(), "");
  }
  
  mdclog_write(MDCLOG_INFO, "Finished  thread %s : Recv = %lu, Tx Attempts = %lu, Tx Fail = %lu", thread_id.str().c_str(), recvs, attempts, fails);
}


template <typename messageProcessor>
void XaPP::Start(messageProcessor && msg_fn){

  std::lock_guard<std::mutex> guard(*_transmit);
  _listen = true;

  // Spin up the the workThreads ..... 
  for(unsigned int i = 0; i < _num_threads; i++){
    thread_group.insert(std::make_pair(i, std::thread( ([&](){_workThread(msg_fn, std::bind(&XaPP::_error_handler, this, std::placeholders::_1), this);}))));
  }
  
};

// template if calling function provides an error handler also
template <typename messageProcessor, typename errorHandler>
void XaPP::Start(messageProcessor && msg_fn, errorHandler && error_handler){

  std::lock_guard<std::mutex> guard(*_transmit);
  _listen = true;

  // Spin up the the workThreads ..... 
  for(unsigned int i = 0; i < _num_threads; i++){
    //std::cout << "Starting thread number " << i << std::endl;
    thread_group.insert(std::make_pair(i, std::thread( ([&](){_workThread(msg_fn, error_handler, this);}))));

  }

  
};

// Template to allow a user defined processor to start
// on a specific thread 
template <typename messageProcessor>
unsigned int XaPP::StartThread(messageProcessor && msg_fn){

  std::lock_guard<std::mutex> guard(*_transmit);
  _listen = true;

  _num_threads++;
  thread_group.insert(std::make_pair(_num_threads, std::thread( ([&](){_workThread(msg_fn, std::bind(&XaPP::_error_handler, this, std::placeholders::_1), this);}))));
  return _num_threads;

};


// Template to allow a user defined processor and error handler to start
// on a specific thread 
template <typename messageProcessor, typename  errorHandler>
unsigned int XaPP::StartThread(messageProcessor && msg_fn, errorHandler && error_handler){

  std::lock_guard<std::mutex> guard(*_transmit);
  _listen = true;

  _num_threads++;
  thread_group.insert(std::make_pair(_num_threads, std::thread( ([&](){_workThread(msg_fn, error_handler, this);}))));
  return _num_threads;

};


#endif

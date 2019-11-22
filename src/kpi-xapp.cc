/*
==================================================================================

        Copyright (c) 2018-2019 SAMSUNG and AT&T Intellectual Property.

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

#include "kpi-xapp.hpp"

static int RunProg = 1;  // keep loop running

bool add_subscription(SubscriptionHandler & sub_handler, XaPP * xapp_ref, subscription_helper & he, subscription_response_helper he_resp, std::string & gNodeB){
  unsigned char node_buffer[32];
  std::copy(gNodeB.begin(), gNodeB.end(), node_buffer);
  node_buffer[gNodeB.length()] = '\0';
  bool res = sub_handler.RequestSubscription(he, he_resp,  RIC_SUB_REQ, std::bind(static_cast<bool (XaPP::*)(int, int, void *, unsigned char const*)>( &XaPP::Send), xapp_ref, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, node_buffer));
  return res;
};

void EndProgram(int signum){
  std::cout <<"Signal received. Stopping program ....." << std::endl;
  RunProg = 0;
}

void msg_error(rmr_mbuf_t *message){
  mdclog_write(MDCLOG_ERR, "Error: %s, %d  Could not send RMR message of length %d and type %d, Reason %s",  __FILE__, __LINE__, message->len,  message->mtype, strerror(errno) );
};


int main(int argc, char *argv[]){

  // initially set log level to INFO
  init_logger("XaPP", MDCLOG_INFO);
  
  configuration my_config;

  // set config variables from environment
  // used when deploying via start-up script
  get_environment_config(my_config);

  // over-ride with any command line variables if
  // provided
  get_command_line_config(argc, argv, my_config);

  std::unique_ptr<XaPP> my_xapp;


  // Reset log level based on configuration
  init_logger(my_config.name, static_cast<mdclog_severity_t>(my_config.log_level));
  
  if (my_config.gNodeB_list.size() == 0){
    mdclog_write(MDCLOG_WARN, "WARNING  : gNodeB not set for subscription. Subscription MAY FAIL");
  }
  
  // Finished parsing command line/environment arguments 
  //=============================================================

   // instantiate xapp object
   if(my_config.num_threads >= 1){
    mdclog_write(MDCLOG_INFO, "XaPP listener threads specified = %d", my_config.num_threads);
    // Create XaPP that starts with specified number of threads 
    my_xapp = std::make_unique<XaPP>(my_config.name, my_config.port, my_config.redis_port, 1024, my_config.num_threads);
  }
  else{
    mdclog_write(MDCLOG_INFO,"XaPP listener threads specified = auto");
    //Let XaPP pick threads based on hardware 
    my_xapp = std::make_unique<XaPP>(my_config.name, my_config.port, my_config.redis_port, 1024);
  }
  

  mdclog_write(MDCLOG_INFO, "XaPP name specified = %s", my_config.name);
  mdclog_write(MDCLOG_INFO, "XaPP port specified = %s", my_config.port);
  mdclog_write(MDCLOG_INFO, "XaPP redis port specified = %d", my_config.redis_port);

   // Instantiate subscription handler
   SubscriptionHandler sub_handler;

   // Instantiate message handlers for RMR

   std::vector<std::unique_ptr<message_processor> > message_procs;
   for(int i = 0; i < my_config.num_threads; i++){
     std::unique_ptr<message_processor> mp_handler = std::make_unique<message_processor> ();
     mp_handler.get()->register_subscription_handler(& sub_handler);
     message_procs.push_back(std::move(mp_handler));
   }
  
  
   // Start the listening loops
   std::vector<int> thread_ids(my_config.num_threads);
   unsigned int i = 0;
   for(auto  &e: message_procs){
     thread_ids[i] = (*my_xapp).StartThread(*(e.get()), msg_error);
     i++;
   };

   mdclog_write(MDCLOG_INFO, "xAPP is UP and Listening on RMR. ...\n");
   mdclog_write(MDCLOG_INFO, "Number of message processors = %lu", message_procs.size());

   //======================================================
   // sgnb Subscription spec

   int request_id = 2; // will be over-written by subscription handler
   int req_seq = 1;
   int function_id = 0;
   int action_id = 4;
   int action_type = 0;

   int message_type = TypeOfMessage_initiating_message;
   int procedure_code = ProcedureCode_id_kPIMonitor;
   std::string egnb_id = "Testgnb";
   std::string plmn_id = "Testplmn";

   unsigned char event_buf[128];
   size_t event_buf_len = 128;
   bool res;


   e2sm_event_trigger_helper trigger_data;
   e2sm_event_trigger event_trigger;
  
   trigger_data.egNB_id = egnb_id;
   trigger_data.plmn_id = plmn_id;
   trigger_data.egNB_id_type = Interface_ID_PR_global_gNB_ID;
   trigger_data.interface_direction = InterfaceDirection_outgoing;
   trigger_data.procedure_code = procedure_code;
   trigger_data.message_type = message_type;
   res = event_trigger.encode_event_trigger(&event_buf[0], &event_buf_len, trigger_data);
   if (!res){
     mdclog_write(MDCLOG_ERR, "Error : %s, %d: Could not encode subscription Request. Reason = %s\n", __FILE__, __LINE__, event_trigger.get_error().c_str());
     exit(0);
   }

   subscription_helper sgnb_add_subscr_req;
   subscription_response_helper subscr_response;
  
   sgnb_add_subscr_req.clear();
   sgnb_add_subscr_req.set_request(request_id, req_seq);
   sgnb_add_subscr_req.set_function_id(function_id);
   sgnb_add_subscr_req.add_action(action_id, action_type);
  
  
   sgnb_add_subscr_req.set_event_def(&event_buf[0], event_buf_len);
   mdclog_write(MDCLOG_INFO, "Encoded event trigger definition into PDU of size %lu bytes\n", event_buf_len);
  
   
   // keep sending subscription request till successfull for all gnodebs ?
   auto it = my_config.gNodeB_list.begin();
   while(my_config.gNodeB_list.size() > 0 && RunProg){
     int attempt = 0;
     res = false;
      
     while(!res){
       mdclog_write(MDCLOG_INFO, "Sending subscription request for %s ... Attempt number = %d\n", (*it).c_str(), attempt);
       res = add_subscription(sub_handler, my_xapp.get(),  sgnb_add_subscr_req, subscr_response, *it);
       if (!res){
	 sleep(5);
       };
       attempt ++;
       if (attempt > MAX_SUBSCRIPTION_ATTEMPTS){
	 break;
       }
     }
     
     if(res){
       mdclog_write(MDCLOG_INFO, "Successfully subscribed for gNodeB %s", (*it).c_str());
       // remove node from list,
       // move to next gnobde
       it = my_config.gNodeB_list.erase(it);
     }

     if (it == my_config.gNodeB_list.end()){
       it = my_config.gNodeB_list.begin();
     }
     
   }
   
   
   std::cout <<"SUBSCRIPTION REQUEST :: Successfully subscribed to events for all gNodeBs " << std::endl;

   //Register signal handler to stop 
   signal(SIGINT, EndProgram);
   signal(SIGTERM, EndProgram);
   

   //Wait for stop
   while(RunProg){
     sleep(10);
   }
  
   i = 0;
   for(auto  &e: message_procs){
     mdclog_write(MDCLOG_INFO, "Thread %d : Number of packets handled = %lu", thread_ids[i], e.get()->get_messages());
     std::cout << "Thread " << thread_ids[i] << "  Number of packets handled = " <<  e.get()->get_messages() << std::endl;
     
     i ++ ;
   }
   
   std::cout <<"Stopping all running threads ..." << std::endl;
   (*my_xapp).Stop();
   std::cout <<"Stopped RMR processing threads ...." << std::endl;

   std::cout <<"Finished ... " << std::endl;
 
   return 0;
};

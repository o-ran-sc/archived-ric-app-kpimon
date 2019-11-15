/*
==================================================================================

        Copyright (c) 2018-2019 AT&T Intellectual Property.

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

#pragma once
#ifndef KPI_XAPP_
#define KPI_XAPP_

#include <limits>
#include <map>
#include <getopt.h>
#include <csignal>
#include <cstdlib>
#include <time.h>
#include <errno.h>
#include <xapp_utils.hpp>
#include <rmr/RIC_message_types.h>
#include <e2sm.hpp>
#include "message_processor_class.hpp"

#define MAX_SLEEP 86400
#define MAX_SUBSCRIPTION_ATTEMPTS 10
//================================================


// configuration parameters 
struct configuration {
  
  char name[128] = "kpi-xapp";
  char port[16] = "tcp:4560";

  int redis_port = 6379;
  
  int num_threads = 1;
  std::unique_ptr<XaPP> my_xapp;
  int log_level = MDCLOG_WARN;

  void fill_gnodeb_list(char * gNodeB_string){
    gNodeB_list.clear();
    char * gnb = strtok(gNodeB_string, ",");
    while(gnb != NULL){
      gNodeB_list.push_back(gnb);
      gnb = strtok(NULL, ",");
    }

  };

  std::vector<string> gNodeB_list;

};

void usage(char *command);
void get_environment_config(configuration & config_instance);
void get_command_line_config(int argc, char **argv, configuration &config_instance);
#endif

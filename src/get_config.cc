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

void get_environment_config(configuration & config_instance){

  // Order of priority for setting variables
  // So we start in reverse order
  //  -- command line 
  // -- environment variable
  // -- default path 
   
  if (char *env_gnodeb = std::getenv("GNODEB")){
    config_instance.fill_gnodeb_list(env_gnodeb);
    mdclog_write(MDCLOG_INFO, "gNodeB List set to %s from environment variable", env_gnodeb);
  }
   
  if (const char *threads = std::getenv("THREADS")){
    config_instance.num_threads = atoi(threads);
    if (    config_instance.num_threads <= 0 or     config_instance.num_threads  > MAX_THREADS){
      mdclog_write(MDCLOG_ERR, "Error :: %s, %d :: Must specify numnber of threads between [1 and %d]. Specified = %d\n", __FILE__, __LINE__, MAX_THREADS,     config_instance.num_threads);
      exit(-1);
    }
    else{
      mdclog_write(MDCLOG_INFO, "Number of threads set to %d from environment variable\n",     config_instance.num_threads);
    }
  }

  if (const char *log_env = std::getenv("LOG_LEVEL")){
    if (!strcmp(log_env, "MDCLOG_INFO")){
      config_instance.log_level = MDCLOG_INFO;
    }
    else if (!strcmp(log_env, "MDCLOG_WARN")){
      config_instance.log_level = MDCLOG_WARN;
    }
    else if (!strcmp(log_env, "MDCLOG_ERR")){
      config_instance.log_level = MDCLOG_ERR;
    }
    else if (!strcmp(log_env, "MDCLOG_DEBUG")){
      config_instance.log_level = MDCLOG_DEBUG;
    }
    else{
      config_instance.log_level = MDCLOG_WARN;
      std::cerr <<"Error ! Illegal environment option for log level  ignored. Setting log level to " << config_instance.log_level << std::endl;
    }
  }
  
}

void get_command_line_config(int argc, char **argv, configuration &config_instance){

    // Parse command line options to over ride
  static struct option long_options[] = 
    {
	/* Thse options require arguments */
	{"name", required_argument, 0, 'n'},
    {"port", required_argument, 0, 'p'},
    {"redisport", required_argument,0, 'r'},
 	{"threads", required_argument,    0, 't'},
	{"gNodeB", required_argument, 0, 'g'},
	{"verbose", no_argument, &config_instance.log_level, MDCLOG_INFO},
    };


   while(1) {

	int option_index = 0;
	char c = getopt_long(argc, argv, "n:p:r:t:g:", long_options, &option_index);

        if(c == -1){
	    break;
         }

	switch(c)
	  {

	  case 0:
	    /* An option flag was set. 
	       Do nothing for now */
	    break;
	  
	  case 'n':
	    strcpy(config_instance.name, optarg);
	    break;
	  
	  case 'p':
	    strcpy(config_instance.port, optarg);
	    break;
	  
	  case 't':
	    config_instance.num_threads = atoi(optarg);
	    mdclog_write(MDCLOG_INFO, "Number of threads set to %d from command line e\n", config_instance.num_threads);
	    break;
	    
	  case 'g':
	    config_instance.fill_gnodeb_list(optarg);
	    mdclog_write(MDCLOG_INFO, "gNodeB List set to %s from command line ", optarg);
	    break;

      case 'r':
        config_instance.redis_port = atoi(optarg);
        mdclog_write(MDCLOG_INFO, "Redis Port set to %d from command line e\n", config_instance.redis_port);
	    break;
	  
	  case 'h':
	    usage(argv[0]);
	    exit(0);
	  
	  default:
	    usage(argv[0]);
	    exit(1);
	  }
   };

}


void usage(char *command){
  std::cout <<"Usage : " << command << " " << std::endl;
  std::cout <<" --name[-n] xapp_instance_name "<< std::endl;
    std::cout <<" --port[-p] port to listen on e.g tcp:4561  "<< std::endl;
    std::cout << "--threads[-t] number of listener threads "<< std::endl ;
    std::cout <<"[--gNodeB[][-g] gNodeB" << std::endl;
    std::cout <<"--redisport[-r] port to connect to redis DB eg., tcp:6379 "<<std::endl;
}

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


#ifndef KPI_DB_RESP_
#define KPI_DB_RESP_

  
#include <iostream>
#include <string.h>
#include <errno.h>
#include <mdclog/mdclog.h>
#include <sstream>
#include <hiredis/hiredis.h>
#include <cu_cp_resource_status_report_helper.hpp>

class kpi_db_resp {
public:

  ~kpi_db_resp();

  void UpdateDB(redisContext* c,cucp_resource_helper *kpi_data);

  std::string get_error(void) const {return error_string;};

private:

  // Wrapper over Redis APIs
  void Rpush(redisContext *, std::string key,std::string value);
  void Get(redisContext *, std::string key);
  void Del(redisContext *, std::string key);
  void Exists(redisContext *, std::string key);

  redisReply *redis_reply;
  std::string error_string;
  char errbuf[128];
  size_t errbuf_len = 128;
};

#endif

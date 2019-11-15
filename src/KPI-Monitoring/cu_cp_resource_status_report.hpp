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

#ifndef CU_CP_RESOURCE_STATUS_REPORT_
#define CU_CP_RESOURCE_STATUS_REPORT_

  
#include <iostream>
#include <errno.h>
#include <mdclog/mdclog.h>
#include <sstream>
#include <cu_cp_resource_status_report_helper.hpp>

#include <CuCpResourceStatusReport.h>
#include <CuCpResourceStatus.h>

#define NUM_CU_CP_RESOURCE_STATUS_REPORT_IES 8
#define INITIAL_SIZE 4

class cucp_resource_status_report {
public:
  cucp_resource_status_report(void);
  ~cucp_resource_status_report(void);

  bool get_fields(CuCpResourceStatusReport_t *, cucp_resource_helper &);
  std::string get_error(void) const {return error_string; };
  
private:

    CuCpResourceStatusReport_t *cucpresourcestatusreport_obj;
    CuCpResourceStatus_t *cucpresourcestatus;

  std::string error_string;
  char errbuf[128];
  size_t errbuf_len = 128;
};


#endif

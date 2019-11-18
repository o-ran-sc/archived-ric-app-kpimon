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


#ifndef KPI_MESSAGE_HANDLER_H
#define KPI_MESSAGE_HANDLER_H

#include <mutex>
#include <memory>
#include <iostream>
#include <ctime>    // For time()
#include <cu_cp_resource_status_report_helper.hpp>
#include <cu_cp_resource_status_report.hpp>
#include <kpi_db.hpp>


#ifdef __GNUC__
#define likely(x)  __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

class kpi_msg_handler
{
public: 
  kpi_msg_handler();
  bool operator()(redisContext *,unsigned char *, size_t);
  void clear();
  std::string get_error(void) { return error_string;};

private:

  std::unique_ptr<std::mutex> m_access;
  cucp_resource_helper kpi_data;
  cucp_resource_status_report kpi_report;
  kpi_db_resp kpi_db_res;
  std::string error_string;
};

#endif

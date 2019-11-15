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


#include "KPIMessageHandler.h"

kpi_msg_handler::kpi_msg_handler()
{	
  m_access = std::make_unique<std::mutex>();
}

// Decodes the CuCpResourceStatusReport present in the RIC Indication Message and Calls the method to Update the content in the DB.
bool kpi_msg_handler::operator()(redisContext *redis_context, unsigned char *msg_ref, size_t msg_size)
{
  
  bool res = true;
  
  std::lock_guard<std::mutex> lck(*(m_access.get()));

  CuCpResourceStatusReport_t *cucp_resource_status_rcv = 0;

  /* Decode */
  asn_dec_rval_t dec_res = asn_decode(0,ATS_ALIGNED_BASIC_PER, &asn_DEF_CuCpResourceStatusReport, (void **)&cucp_resource_status_rcv, msg_ref, msg_size);

  if (dec_res.code == RC_OK){
	  mdclog_write(MDCLOG_INFO, "Processing CuCpResourceStatusReport message\n");
	  res = true;
  }
  else{
    mdclog_write(MDCLOG_ERR, "Error :: %s, %d :: Could not decode CuCpResourceStatusReport of size %lu bytes \n", __FILE__, __LINE__, msg_size);
    res = false;
  }

  if (res){
    
    //std::cout <<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;
    //xer_fprint(stdout,  &asn_DEF_CuCpResourceStatusReport, cucp_resource_status_rcv);
    //std::cout <<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << std::endl;

    // Add our get field function for Custom message
    res = kpi_report.get_fields(cucp_resource_status_rcv, kpi_data);
    if (!res){
      mdclog_write(MDCLOG_ERR, "Error :: %s, %d :: could not get  fields from CuCpResourceStatusReport. Reason = %s\n", __FILE__, __LINE__, kpi_report.get_error().c_str());
    }
  }

  kpi_db_res.UpdateDB(redis_context,&kpi_data);

  ASN_STRUCT_FREE(asn_DEF_CuCpResourceStatusReport, cucp_resource_status_rcv);
  return res;
  
}

void kpi_msg_handler::clear()
{
  std::lock_guard<std::mutex> lck(*(m_access.get()));
}
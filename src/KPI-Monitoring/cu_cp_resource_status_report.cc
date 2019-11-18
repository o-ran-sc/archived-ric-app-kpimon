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


#include <cu_cp_resource_status_report.hpp>

cucp_resource_status_report::cucp_resource_status_report(void){
  cucpresourcestatusreport_obj = 0;
  cucpresourcestatusreport_obj = (CuCpResourceStatusReport_t *)calloc(1, sizeof(CuCpResourceStatusReport_t));
  assert(cucpresourcestatusreport_obj != 0);

  cucpresourcestatus = 0;
  cucpresourcestatus = (CuCpResourceStatus_t * )calloc(1, sizeof(CuCpResourceStatus_t));
  assert(cucpresourcestatus != 0);

  cucpresourcestatus->numberOfUEs = (long *)malloc(sizeof(long));

  cucpresourcestatusreport_obj->cuCpResourceStatus = *cucpresourcestatus;
};


cucp_resource_status_report::~cucp_resource_status_report(void){

  mdclog_write(MDCLOG_INFO, "Freeing object memory");

  cucpresourcestatusreport_obj->cuCpResourceStatus.numberOfUEs = 0;
  ASN_STRUCT_FREE(asn_DEF_CuCpResourceStatus, cucpresourcestatus);
  ASN_STRUCT_FREE(asn_DEF_CuCpResourceStatusReport, cucpresourcestatusreport_obj);

  mdclog_write(MDCLOG_INFO, "Freed object mempory");
}

bool cucp_resource_status_report::get_fields(CuCpResourceStatusReport_t * cucpresourcestatusreport, cucp_resource_helper & dout){

  if (cucpresourcestatusreport == 0){
    error_string = "Error ! Invalid reference for Cu Cp Resource Status Report get_fields";
    return false;
  }

  int res = 0;

  if(cucpresourcestatusreport->cuCpName != 0) {
      dout.cuCpName = cucpresourcestatusreport->cuCpName->buf;
      dout.cuCpName_size = cucpresourcestatusreport->cuCpName->size;
  }

  dout.gnbId = cucpresourcestatusreport->gnbId.buf;
  dout.gnbId_size = cucpresourcestatusreport->gnbId.size;

  dout.cuCpResourceStatus.bhca = cucpresourcestatusreport->cuCpResourceStatus.bhca;

  if(cucpresourcestatusreport->cuCpResourceStatus.numberOfUEs !=0) {
      dout.cuCpResourceStatus.numberOfUEs = *(cucpresourcestatusreport->cuCpResourceStatus.numberOfUEs);
  }

  return true;
}

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

#include <e2ap_indication.hpp>

// Set up the initiating message and also allocate protocolIEs in container
// Note : this bypasses requirement to use ASN_SEQUENCE_ADD. We can directly
// assign pointers to the array in ProtocolIE. However, this also leaves us on the
// hook to manually clear the memory

ric_indication::ric_indication(void){

  e2ap_pdu_obj = 0;
  e2ap_pdu_obj = (E2AP_PDU_t * )calloc(1, sizeof(E2AP_PDU_t));
  assert(e2ap_pdu_obj != 0);

  initMsg = 0;
  initMsg = (InitiatingMessage_t * )calloc(1, sizeof(InitiatingMessage_t));
  assert(initMsg != 0);

  IE_array = 0;
  IE_array = (RICindication_IEs_t *)calloc(NUM_INDICATION_IES, sizeof(RICindication_IEs_t));
  assert(IE_array != 0);

  e2ap_pdu_obj->present = E2AP_PDU_PR_initiatingMessage;
  e2ap_pdu_obj->choice.initiatingMessage = initMsg;

  RICindication_t * ric_indication = &(initMsg->value.choice.RICindication);
  for(int i = 0; i < NUM_INDICATION_IES; i++){
    ASN_SEQUENCE_ADD(&(ric_indication->protocolIEs), &(IE_array[i]));
  }
		       
  
    
};



// Clear assigned protocolIE list from RIC indication IE container
ric_indication::~ric_indication(void){

  mdclog_write(MDCLOG_INFO, "Freeing E2AP Indication object memory");
  RICindication_t *ricIndication  = &(initMsg->value.choice.RICindication);
  for(int i = 0; i < ricIndication->protocolIEs.list.size; i++){
    ricIndication->protocolIEs.list.array[i] = 0;
  }
  if (ricIndication->protocolIEs.list.size > 0){
    free(ricIndication->protocolIEs.list.array);
    ricIndication->protocolIEs.list.array = 0;
    ricIndication->protocolIEs.list.count = 0;
    ricIndication->protocolIEs.list.size = 0;
  }
  
  free(IE_array);
  ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, e2ap_pdu_obj);
  mdclog_write(MDCLOG_INFO, "Freed E2AP Indication object mempory");
}

bool ric_indication:: get_fields(InitiatingMessage_t * init_msg,  E2APRicIndication &dout)
{
  if (init_msg == 0){
    error_string = "Invalid reference for E2AP Indication message in get_fields";
    return false;
  }
  
 
  for(int edx = 0; edx < init_msg->value.choice.RICindication.protocolIEs.list.count; edx++) {
    RICindication_IEs_t *memb_ptr = init_msg->value.choice.RICindication.protocolIEs.list.array[edx];
    
    switch(memb_ptr->id)
      {
      case (ProtocolIE_ID_id_RICindicationHeader):
  	dout.indication_header = memb_ptr->value.choice.RICindicationHeader.buf;
  	dout.indication_header_size = memb_ptr->value.choice.RICindicationHeader.size;
  	break;
	
      case (ProtocolIE_ID_id_RICindicationMessage):
  	dout.indication_msg =  memb_ptr->value.choice.RICindicationMessage.buf;
  	dout.indication_msg_size = memb_ptr->value.choice.RICindicationMessage.size;
  	break;
	    
      case (ProtocolIE_ID_id_RICrequestID):
  	dout.req_id = memb_ptr->value.choice.RICrequestID.ricRequestorID;
  	dout.req_seq_no = memb_ptr->value.choice.RICrequestID.ricRequestSequenceNumber;
  	break;
	
      case (ProtocolIE_ID_id_RANfunctionID):
  	dout.func_id = memb_ptr->value.choice.RANfunctionID;
  	break;
	
      case (ProtocolIE_ID_id_RICindicationSN):
  	dout.indication_sn = memb_ptr->value.choice.RICindicationSN;
  	break;
	
      case (ProtocolIE_ID_id_RICindicationType):
  	dout.indication_type = memb_ptr->value.choice.RICindicationType;
  	break;
	
      case (ProtocolIE_ID_id_RICactionID):
  	dout.action_id = memb_ptr->value.choice.RICactionID;
  	break;
	
      default:
  	break;
      }
    
  }
  
  return true;

}

InitiatingMessage_t * ric_indication::get_message(void)  {
    return initMsg;
}

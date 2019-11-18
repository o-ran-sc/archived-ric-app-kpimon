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


#include <message_processor_class.hpp>

void message_processor::register_subscription_handler(SubscriptionHandler * sub){
  _ref_sub_handler = sub;
}

// main message processing 
bool message_processor::operator()(redisContext *redis_context, rmr_mbuf_t *message){

  bool res;
  int message_type, procedure_code;
  asn_dec_rval_t rval;
  size_t buf_size = BUFFER_LENGTH;
  size_t mlen;
  _num_messages ++;
  std::string response;

  // main message processing code
  switch(message->mtype){
    
  case RIC_INDICATION:

    e2ap_recv_pdu = 0;
    e2sm_header = 0;
   
    rval = asn_decode(0,ATS_ALIGNED_BASIC_PER, &asn_DEF_E2AP_PDU, (void**)&(e2ap_recv_pdu), message->payload, message->len);

    if(likely(rval.code == RC_OK)){
      num_indications ++;
      res = indication_processor.get_fields(e2ap_recv_pdu->choice.initiatingMessage, indication_data);
      if (unlikely(!res)){
	mdclog_write(MDCLOG_ERR, "Error :: %s, %d :: Could not get fields from RICindication message\n", __FILE__, __LINE__);
	return false;
      }
      //std::cout <<"+++++++++++++++++++++++ E2AP Indication ++++++++++++++++++++++++" << std::endl;
      //xer_fprint(stdout, &asn_DEF_E2AP_PDU, e2ap_recv_pdu);
      //std::cout <<"+++++++++++++++++++++++ E2AP Indication ++++++++++++++++++++++++" << std::endl;
    }
    else{
      num_err_indications ++;
      mdclog_write(MDCLOG_ERR, "Error :: %s, %d: Error decoding E2AP PDU\n", __FILE__, __LINE__);
      return false;
    }

    mdclog_write(MDCLOG_INFO, "E2AP INDICATION :: Successfully received E2AP Indication message with id = %d, sequence no = %d, Number of indication messages = %lu, Number of erroneous indications = %lu\n", indication_data.req_id, indication_data.req_seq_no, num_indications, num_err_indications);
      
    //Decode the SM header
    rval = asn_decode(0, ATS_ALIGNED_BASIC_PER, &asn_DEF_E2SM_gNB_X2_indicationHeader, (void**)&(e2sm_header), indication_data.indication_header, indication_data.indication_header_size);
    if (likely(rval.code == RC_OK)){
      res = e2sm_indication_processor.get_header_fields(e2sm_header, header_helper);
      if (unlikely(!res)){
	mdclog_write(MDCLOG_ERR, "Error :: %s, %d :: Could not get fields from E2SM HEADER\n", __FILE__, __LINE__);
	return false;
      }
      
      // std::cout <<"+++++++++++++++++++++++ E2SM Indication Header ++++++++++++++++++++++++" << std::endl;
      // xer_fprint(stdout, &asn_DEF_E2SM_gNB_X2_indicationHeader, e2sm_header);
      // std::cout <<"+++++++++++++++++++++++ E2SM Indication Header ++++++++++++++++++++++++" << std::endl;	
    }
    else{
      mdclog_write(MDCLOG_ERR, "Error :: %s, %d: Error decoding E2SM Header.", __FILE__, __LINE__);
      return false;
    }
    mdclog_write(MDCLOG_DEBUG, "E2SM INDICATION HEADER :: Successfully decoded E2SM Indication Header of size %lu\n", indication_data.indication_header_size);

    // Do the further decoding here for KPI message

    res = (*_ref_kpi_msg_handler)(redis_context, indication_data.indication_msg, indication_data.indication_msg_size);

    if(unlikely(!res)){
      return false;
    }
    // NOTE : We assume RICindicationMessage contains payload (not E2SM message)
    ASN_STRUCT_FREE(asn_DEF_E2SM_gNB_X2_indicationHeader, e2sm_header);
    ASN_STRUCT_FREE(asn_DEF_E2AP_PDU, e2ap_recv_pdu);

    break;

  case (RIC_SUB_RESP):
  case (RIC_SUB_FAILURE):
    if (_ref_sub_handler != NULL){
      mdclog_write(MDCLOG_INFO, "Received subscription message of type = %d", message->mtype);
      _ref_sub_handler->Response(message->mtype, message->payload, message->len);
    }
    else{
      mdclog_write(MDCLOG_ERR, " Error :: %s, %d : Subscription handler not assigned in message processor !", __FILE__, __LINE__);
      return false;
    }
    
    break;

  default:
    mdclog_write(MDCLOG_ERR, "Error :: Unknown message type %d received from RMR", message->mtype);
  };
  
  return false;
};


unsigned long const message_processor::get_messages (void){
    return _num_messages;
};


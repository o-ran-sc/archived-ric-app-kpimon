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

#include <kpi_db.hpp>

// Destructor
kpi_db_resp::~kpi_db_resp(void){
    freeReplyObject(redis_reply);
};

// Push the data to the DB
void kpi_db_resp::Rpush(redisContext *c, std::string key,std::string value){
  redis_reply = (redisReply*)redisCommand(c, "RPUSH %s %s",key,value);
}

// Getting val corresponding to key
void kpi_db_resp::Get(redisContext *c, std::string key){
  redis_reply= (redisReply*)redisCommand(c, "GET %s",key);
}

// Delete the key and value pair from the DB
void kpi_db_resp::Del(redisContext *c, std::string key){
  redis_reply= (redisReply*)redisCommand(c, "DEL %s",key);
}

// Check whether the Key exists in the DB
void kpi_db_resp::Exists(redisContext *c, std::string key){
  redis_reply= (redisReply*)redisCommand(c, "EXISTS %s",key);
}

// Called whenever the KPI information is received from the RIC Indication message
void kpi_db_resp::UpdateDB(redisContext* c,cucp_resource_helper *kpi_data){

    if(kpi_data == NULL || c==NULL) {
        mdclog_write(MDCLOG_INFO, "Reddis Context or KPI_DATA pointer NULL\n");
        return;
    }
    std::string gnb_id(reinterpret_cast<char*>(kpi_data->gnbId));
    Exists(c,gnb_id);
    if(redis_reply->type==REDIS_REPLY_STATUS){
      if(redis_reply->integer==1){
        //Key already exists ---> delete the data and insert again
        Del(c,gnb_id);
        Rpush(c,gnb_id,std::to_string(kpi_data->cuCpResourceStatus.bhca));
        Rpush(c,gnb_id,std::to_string(kpi_data->cuCpResourceStatus.numberOfUEs));
      }
      else{
        //Key does not exists --> Simply insert
        Rpush(c,gnb_id,std::to_string(kpi_data->cuCpResourceStatus.bhca));
        Rpush(c,gnb_id,std::to_string(kpi_data->cuCpResourceStatus.numberOfUEs));
      }
    }
    mdclog_write(MDCLOG_INFO, "GNB_ID : %s\n\tBHCA : %d\n\tUE_Count : %d",gnb_id, kpi_data->cuCpResourceStatus.bhca, kpi_data->cuCpResourceStatus.numberOfUEs);
}
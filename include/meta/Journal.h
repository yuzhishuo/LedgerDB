/*
 * @Author: Leo
 * @Date: 2022-07-17 06:35:03
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-17 07:28:06
 */

#pragma once

#include <string>


struct Payload {

    char * payload;
    uint64_t length;
};

struct JournalRequest {

    std::string ledger_name;
    std::string journal_id;
    Payload payload;
    uint64_t nonce;
    uint64_t timestamp;
    std::string sender;
};


struct JournalProxying  {

    std::string request_hash;

    uint64_t stream_id;
    uint64_t offset;
    uint64_t journal_size;

    uint64_t server_id;
    uint64_t server_timestamp;
    uint64_t execute_status;
    uint64_t execute_result;
    
    uint64_t root_hash_status;
};

struct JournalReceipt {
    
    std::string request_hash;
    std::string proxying_hash;
    std::string block_hash;

    uint64_t server_id;
    uint64_t server_timestamp;
    uint64_t execute_status;
    uint64_t execute_result;
};

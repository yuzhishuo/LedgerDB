/*
 * @Author: Leo
 * @Date: 2022-08-01 02:18:01
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-07 01:14:54
 */

#pragma once
#ifndef LEDGER_YUZHI_UTILITY_SNOW_H
#define LEDGER_YUZHI_UTILITY_SNOW_H

#include <inttypes.h>
#include <mutex>
#include <pthread.h>
namespace yuzhi::utility
{
  // 机器id所占的位数
constexpr uint64_t workerIdBits = 5uL;

struct SnowflakeIdWorker
{
  // 数据中心ID(0~31)
  uint64_t datacenterId;
  // 工作机器ID(0~31)
  uint64_t workerId;
  // 毫秒内序列(0~4095)
  uint64_t sequence;
  // 上次生成ID的时间截
  uint64_t lastTimestamp;
};

void snowflakeIdWorkerInit(SnowflakeIdWorker *snowflakeIdWorker, uint64_t datacenterId, uint64_t workerId);

/**
 * 获得下一个ID (该方法是线程安全的)
 * @return SnowflakeId
 */
uint64_t nextId(SnowflakeIdWorker *snowflakeIdWorker, std::mutex &mutex);
} // namespace yuzhi::utility
#endif // LEDGER_YUZHI_UTILITY_SNOW_H

/*
 * @Author: Leo
 * @Date: 2022-09-17 11:45:29
 * @LastEditors: Leo
 * @LastEditTime: 2022-09-25 11:42:19
 */
#pragma once

#ifndef LEDGERDB_YUZHI_UTILITY_BUFFER_H
#define LEDGERDB_YUZHI_UTILITY_BUFFER_H

#include <stddef.h>
#include <stdint.h>
#include <vector>

namespace yuzhi::utility
{

struct Buffer
{
  uint32_t *data;
  size_t length;
};

struct IOBuffer
{
public:
    
private:
  std::vector<Buffer> buffers_;
};

} // namespace yuzhi::utility

#endif // LEDGERDB_YUZHI_UTILITY_BUFFER_H
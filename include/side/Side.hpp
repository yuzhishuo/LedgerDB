/*
 * @Author: Leo
 * @Date: 2022-09-11 16:40:48
 * @LastEditors: Leo
 * @LastEditTime: 2022-09-25 14:29:33
 */
#pragma once

#ifndef YUZHI_SIDE_SIDE_H
#define YUZHI_SIDE_SIDE_H

#include <common/Error.h>
#include <stdlib.h>
#include <uv.h>

namespace yuzhi::side
{
class Side
{

public:
  Side();
  ~Side();

private:
  friend void on_new_connection(uv_stream_t *server, int status);

public:
  std::optional<common::Error> run();
  void OnNewConnection(uv_stream_t *server, int status);

private:
  uv_loop_t *loop_;
  uv_tcp_t channel_;
};

} // namespace yuzhi::side

#endif // YUZHI_SIDE_SIDE_H
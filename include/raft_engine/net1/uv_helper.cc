/*
 * @Author: Leo
 * @Date: 2022-08-07 00:28:59
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-15 14:59:29
 */
#include <assert.h>
#include <spdlog/spdlog.h>
#include <stdlib.h>

#include "uv_helpers.h"

void uv_bind_listen_socket(uv_tcp_t *listen, const char *host, const int port, uv_loop_t *loop, void * data)
{
  SPDLOG_DEBUG("bind listen socket: {}:{}", host, port);
  if (auto e = uv_tcp_init(loop, listen); e != 0)
  {
    spdlog::error("uv_tcp_init error: {}", uv_strerror(e));
    uv_fatal(e);
  }
  
  listen->data = data;
  
  struct sockaddr_in addr;
  switch (auto e = uv_ip4_addr(host, port, &addr); e)
  {
  case 0:
    break;
  case EINVAL:
    SPDLOG_ERROR("Invalid IP address/port given: {}:{}", host, port);
    abort();
    break;
  default:
    uv_fatal(e);
  }

  if (auto e = uv_tcp_bind(listen, (struct sockaddr *)&addr, 0); e != 0)
  {
    spdlog::error("uv_tcp_bind error: {}", uv_strerror(e));
    uv_fatal(e);
  }
}
/*
 * @Author: Leo
 * @Date: 2022-09-25 13:33:33
 * @LastEditors: Leo
 * @LastEditTime: 2022-09-25 14:34:20
 */
#include <side/Side.hpp>

namespace yuzhi::side
{

void on_new_connection(uv_stream_t *server, int status)
{
  auto side = (Side *)uv_handle_get_data(reinterpret_cast<uv_handle_t *>(&server));
  side->Side::OnNewConnection(server, status);
}

std::optional<common::Error> Side::run()
{
  struct sockaddr_in addr;
  uv_ip4_addr("0.0.0.0", 1323, &addr);

  uv_tcp_bind(&channel_, (const struct sockaddr *)&addr, 0);
  uv_handle_set_data(reinterpret_cast<uv_handle_t *>(&channel_), this);
  uv_listen((uv_stream_t *)&channel_, 10, on_new_connection);

  uv_run(loop_, UV_RUN_DEFAULT);

  return std::nullopt;
}

Side::~Side() { uv_loop_close(loop_); }

Side::Side() : loop_{reinterpret_cast<uv_loop_t *>(malloc(sizeof(uv_loop_t)))}
{
  uv_loop_init(loop_);
  uv_tcp_init(loop_, &channel_);
}
} // namespace yuzhi::side

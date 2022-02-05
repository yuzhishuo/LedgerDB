/*
 * @Author: Leo
 * @Date: 2022-02-01 20:04:04
 * @LastEditTime: 2022-02-06 01:16:38
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/raft_engine/net/RaftService.hpp
 */
#pragma once

#include <config/Config.h>
#include <memory>
#include <muduo/net/EventLoop.h>
#include <spdlog/spdlog.h>
#include <unordered_map>

extern "C" {
#include <raft.h>
#include <raft_log.h>
#include <raft_private.h>
}

namespace muduo::net {
class TcpServer;
class TcpClient;
} // namespace muduo::net

struct peer_connection_t;

#include <condition_variable>
#include <mutex>
namespace yuzhi::raft_engine::net {

class RaftService : public IConfigurable {

public:
  RaftService();
  virtual ~RaftService() {}
  virtual const char *Field() const override { return "raft"; }

public:
  void onConnection(const muduo::net::TcpConnectionPtr &conn);
  peer_connection_t *__new_connection();
  void __connect_to_peer(peer_connection_t *peer);
  void __connect_to_peer_at_host(peer_connection_t *conn, char *host, int port);
  void
  __on_connection_accepted_by_peer(peer_connection_t *data,
                                   const muduo::net::TcpConnectionPtr &conn);

  int __append_cfg_change(RaftService *sv, raft_logtype_e change_type,
                          char *host, int raft_port, int http_port,
                          int node_id);
  int __deserialize_and_handle_msg(void *img, size_t sz, void *data);
  peer_connection_t *__find_connection(const muduo::net::InetAddress &addr);
  void __delete_connection(peer_connection_t *conn);

public:
  int32_t getNodeId() const { return node_id; }
  int16_t getRaftPort() const { return raft_port; }
  int16_t getHttpPort() const { return http_port; }

private:
  std::mutex mutex;
  std::condition_variable cond;
  int32_t node_id;
  void *raft;
  peer_connection_t *conns = nullptr;
  muduo::net::EventLoop loop_;
  std::unique_ptr<muduo::net::TcpServer> server_;
  std::vector<std::unique_ptr<muduo::net::TcpClient>> cls_;

  int16_t http_port;
  int16_t raft_port;
};

} // namespace yuzhi::raft_engine::net

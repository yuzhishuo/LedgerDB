/*
 * @Author: Leo
 * @Date: 2022-02-01 20:04:04
 * @LastEditTime: 2022-02-11 13:39:29
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /Ledger/include/raft_engine/net/RaftService.hpp
 */
#pragma once

#include <config/Config.h>
#include <memory>
#include <muduo/net/EventLoop.h>
#include <spdlog/spdlog.h>
#include <store/PersistenceStore.h>
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

class RaftService : public yuzhi::IConfigurable {

  friend int __deserialize_and_handle_msg(void *img, size_t sz, void *data);
  friend peer_connection_t *__new_connection();
  friend void __connect_to_peer(peer_connection_t *peer);
  friend void __connect_to_peer_at_host(peer_connection_t *conn, char *host,
                                        int port);

  friend void __send_handshake(peer_connection_t *conn);
  friend void
  __on_connection_accepted_by_peer(peer_connection_t *data,
                                   const muduo::net::TcpConnectionPtr &conn);

  friend int __append_cfg_change(RaftService *sv, raft_logtype_e change_type,
                                 char *host, int raft_port, int http_port,
                                 int node_id);
  friend int __raft_applylog(raft_server_t *raft, void *udata,
                             raft_entry_t *ety, raft_index_t entry_idx);
  friend peer_connection_t *
  __find_connection(const muduo::net::InetAddress &addr);
  friend void __delete_connection(peer_connection_t *conn);
  friend int __offer_cfg_change(raft_server_t *raft, const unsigned char *data,
                                raft_logtype_e change_type);

  friend int __raft_persist_vote(raft_server_t *raft, void *udata,
                                 const int voted_for);
  friend int __raft_persist_term(raft_server_t *raft, void *udata,
                                 raft_term_t current_term, raft_node_id_t vote);
  friend int __raft_persist_term(raft_server_t *raft, void *udata,
                                 raft_term_t current_term, raft_node_id_t vote);
  friend int __raft_persist_vote(raft_server_t *raft, void *udata,
                                 const int voted_for);

public:
  std::optional<Error> Save(const std::string &key, const std::string &value);

public:
  static RaftService &Instance() {
    static RaftService instance;
    return instance;
  }

public:
  RaftService();
  virtual ~RaftService() {}
  virtual const char *Field() const override { return "raft"; }

public:
public:
  void onConnection(const muduo::net::TcpConnectionPtr &conn);
  void onMessage(const muduo::net::TcpConnectionPtr &conn,
                 muduo::net::Buffer *buf, muduo::Timestamp receiveTime);

public:
  int32_t getNodeId() const { return node_id; }
  int16_t getRaftPort() const { return raft_port; }
  int16_t getHttpPort() const { return http_port; }

public:
private:
  std::mutex mutex_;
  std::condition_variable cond;
  int32_t node_id;
  void *raft;
  peer_connection_t *conns = nullptr;
  muduo::net::EventLoop loop_;
  std::unique_ptr<muduo::net::TcpServer> server_;
  std::vector<std::unique_ptr<muduo::net::TcpClient>> cls_;
  PersistenceStore persistenceStore;
  int16_t http_port;
  int16_t raft_port;
};

} // namespace yuzhi::raft_engine::net
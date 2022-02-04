/*
 * @Author: Leo
 * @Date: 2022-02-01 20:04:04
 * @LastEditTime: 2022-02-05 00:12:15
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/raft_engine/net/RaftService.hpp
 */

#include <config/Config.h>
#include <memory>
#include <muduo/net/EventLoop.h>
#include <spdlog/spdlog.h>

namespace muduo::net {
class TcpServer;
class EventLoop;
} // namespace muduo::net

struct peer_connection_t;

#include <condition_variable>
#include <mutex>
namespace yuzhi::raft_engine::net {

class RaftService : public IConfigurable {

public:
  RaftService();
  virtual ~RaftService() = default;
  virtual const char *Field() const override { return "raft"; }

public:
  void onConnection(const muduo::net::TcpConnectionPtr &conn);
  peer_connection_t *__new_connection();
  void __connect_to_peer(peer_connection_t *peer);
  void __connect_to_peer_at_host(peer_connection_t *conn, char *host, int port);
  void
  __on_connection_accepted_by_peer(peer_connection_t *data,
                                   const muduo::net::TcpConnectionPtr &conn);

private:
  std::shared_ptr<muduo::net::EventLoop> loop_;
  std::unique_ptr<muduo::net::TcpServer> server_;
  void *raft;
  peer_connection_t *conns = nullptr;
  int32_t node_id;
  std::mutex mutex;
  std::condition_variable cond;
};

} // namespace yuzhi::raft_engine::net

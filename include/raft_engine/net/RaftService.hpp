/*
 * @Author: your name
 * @Date: 2022-02-01 20:04:04
 * @LastEditTime: 2022-02-01 20:25:45
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/raft_engine/net/RaftService.hpp
 */

#include <muduo/net/EventLoop.h>

namespace yuzhi::raft_engine::net {

class RaftService {

public:
private:
  std::shared_ptr<muduo::net::EventLoop> loop_;
  std::unique_ptr<grpc::Server> server_;
};

} // namespace yuzhi::raft_engine::net

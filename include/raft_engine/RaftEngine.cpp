/*
 * @Author: Leo
 * @Date: 2022-08-06 16:36:56
 * @LastEditors: Leo
 * @LastEditTime: 2022-08-09 12:30:06
 */
#include <config/Config.h>
#include <raft_engine/RaftEngine.h>
namespace yuzhi::raft_engine
{

RaftEngine::RaftEngine()
{
  auto &config = Config::Instance();

  auto raft_port = config.get<int>(this, "raft_port");
  auto engine_port = config.get<int>(this, "engine_port");
  auto join = config.get<bool>(this, "joinable");
  raft_server_ = std::make_unique<RaftServer>(raft_port, engine_port, join);
}

// RaftEngine::~RaftEngine() {}

} // namespace yuzhi::raft_engine
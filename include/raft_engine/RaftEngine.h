/*
 * @Author: Leo
 * @Date: 2022-01-30 13:11:29
 * @LastEditTime: 2022-08-09 12:30:02
 * @LastEditors: Leo
 */
#pragma once
#include <memory>

#include <functional>
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <unistd.h>

#include <raft_engine/net1/RaftServer.h>

#include <config/Config.h>

namespace yuzhi::raft_engine
{

class RaftEngine : public IConfigurable
{

public:
  RaftEngine();

  const char *Field() const override { return "raft"; }
  ~RaftEngine() override = default;

private:
  std::unique_ptr<RaftServer> raft_server_;
};
} // namespace yuzhi::raft_engine

/*
 * @Author: Leo
 * @Date: 2022-01-30 13:11:29
 * @LastEditTime: 2022-08-06 17:27:44
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 *
 * @FilePath: /example-authority-cpp/include/raft_engine/RaftEngine.h
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
  virtual const char *Field() const override { return "raft"; }
  virtual ~RaftEngine();

private:
public:
private:
};
} // namespace yuzhi::raft_engine

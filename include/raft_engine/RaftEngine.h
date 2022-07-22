/*
 * @Author: Leo
 * @Date: 2022-01-30 13:11:29
 * @LastEditTime: 2022-07-22 09:09:38
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 *
 * @FilePath: /example-authority-cpp/include/raft_engine/RaftEngine.h
 */
#pragma once
#include <memory>

#include <spdlog/spdlog.h>

#include "query.pb.h"
#include <raft_engine/net/codec.h>
#include <raft_engine/net/dispatcher.h>

#include <muduo/base/Logging.h>
#include <muduo/base/Mutex.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpServer.h>

#include <functional>
#include <stdio.h>
#include <unistd.h>

extern "C"
{
#include <raft.h>
}

#include <raft.grpc.pb.h>

#include <config/Config.h>
using namespace muduo;
using namespace muduo::net;

typedef std::shared_ptr<muduo::Query> QueryPtr;
typedef std::shared_ptr<muduo::Answer> AnswerPtr;
namespace yuzhi::raft_engine
{

class RaftEngine : private raft::Raft::Service, public IConfigurable
{

public:
  RaftEngine(EventLoop *loop, const InetAddress &listenAddr)
      : server_(loop, listenAddr, "RaftEngine"),
        dispatcher_(std::bind(&RaftEngine::onUnknownMessage, this, _1, _2, _3)),
        codec_(std::bind(&ProtobufDispatcher::onProtobufMessage, &dispatcher_,
                         _1, _2, _3))
  {
    dispatcher_.registerMessageCallback<muduo::Query>(
        std::bind(&RaftEngine::onQuery, this, _1, _2, _3));
    dispatcher_.registerMessageCallback<muduo::Answer>(
        std::bind(&RaftEngine::onAnswer, this, _1, _2, _3));
    server_.setConnectionCallback(
        std::bind(&RaftEngine::onConnection, this, _1));
    server_.setMessageCallback(
        std::bind(&ProtobufCodec::onMessage, &codec_, _1, _2, _3));
  }

  virtual const char *Field() const override { return "raft"; }

  void Listen();

  void start() { server_.start(); }

  virtual ~RaftEngine() = default;

private:
  void onConnection(const TcpConnectionPtr &conn)
  {
    LOG_INFO << conn->localAddress().toIpPort() << " -> "
             << conn->peerAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
  }

  void onUnknownMessage(const TcpConnectionPtr &conn, const MessagePtr &message,
                        Timestamp)
  {
    LOG_INFO << "onUnknownMessage: " << message->GetTypeName();
    conn->shutdown();
  }

  void onQuery(const muduo::net::TcpConnectionPtr &conn,
               const QueryPtr &message, muduo::Timestamp)
  {
    LOG_INFO << "onQuery:\n"
             << message->GetTypeName() << message->DebugString();
    Answer answer;
    answer.set_id(1);
    answer.set_questioner("Chen Shuo");
    answer.set_answerer("blog.csdn.net/Solstice");
    answer.add_solution("Jump!");
    answer.add_solution("Win!");
    codec_.send(conn, answer);

    conn->shutdown();
  }

  void onAnswer(const muduo::net::TcpConnectionPtr &conn,
                const AnswerPtr &message, muduo::Timestamp)
  {
    LOG_INFO << "onAnswer: " << message->GetTypeName();
    conn->shutdown();
  }

public:
private:
  TcpServer server_;
  ProtobufDispatcher dispatcher_;
  ProtobufCodec codec_;
};
} // namespace yuzhi::raft_engine

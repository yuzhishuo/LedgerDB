/*
 * @Author: Leo
 * @Date: 2022-02-01 20:36:02
 * @LastEditTime: 2022-07-22 09:50:43
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/raft_engine/net/dispatcher_lite.h
 */
// Copyright 2011, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_EXAMPLES_PROTOBUF_CODEC_DISPATCHER_LITE_H
#define MUDUO_EXAMPLES_PROTOBUF_CODEC_DISPATCHER_LITE_H

#include <muduo/base/noncopyable.h>
#include <muduo/net/Callbacks.h>

#include <google/protobuf/message.h>

#include <map>

typedef std::shared_ptr<google::protobuf::Message> MessagePtr;

class ProtobufDispatcherLite : muduo::noncopyable
{
public:
  typedef std::function<void(const muduo::net::TcpConnectionPtr &, const MessagePtr &, muduo::Timestamp)>
      ProtobufMessageCallback;

  // ProtobufDispatcher()
  //   : defaultCallback_(discardProtobufMessage)
  // {
  // }

  explicit ProtobufDispatcherLite(const ProtobufMessageCallback &defaultCb) : defaultCallback_(defaultCb) {}

  void onProtobufMessage(const muduo::net::TcpConnectionPtr &conn, const MessagePtr &message,
                         muduo::Timestamp receiveTime) const
  {
    CallbackMap::const_iterator it = callbacks_.find(message->GetDescriptor());
    if (it != callbacks_.end())
    {
      it->second(conn, message, receiveTime);
    }
    else
    {
      defaultCallback_(conn, message, receiveTime);
    }
  }

  void registerMessageCallback(const google::protobuf::Descriptor *desc, const ProtobufMessageCallback &callback)
  {
    callbacks_[desc] = callback;
  }

private:
  // static void discardProtobufMessage(const muduo::net::TcpConnectionPtr&,
  //                                    const MessagePtr&,
  //                                    muduo::Timestamp);

  typedef std::map<const google::protobuf::Descriptor *, ProtobufMessageCallback> CallbackMap;
  CallbackMap callbacks_;
  ProtobufMessageCallback defaultCallback_;
};

#endif // MUDUO_EXAMPLES_PROTOBUF_CODEC_DISPATCHER_LITE_H

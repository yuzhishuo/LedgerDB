/*
 * @Author: Leo
 * @Date: 2022-02-08 15:39:01
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 09:47:14
 */
#include <client/ledgerClient.hpp>
#include <cmdline.hpp>
#include <spdlog/spdlog.h>

#include <functional>
#include <iostream>
#include <string>

using namespace std;

void append_op(int argc, char *argv[])
{

  cmdline::parser append_parser;

  append_parser.add<string>("name", 'n', "name of ledger", true);

  append_parser.add<string>("type", 't', "protocol type", true, "str", cmdline::oneof<string>("file", "str"));

  if (!append_parser.parse(argc, argv))
  {

    std::cout << "--Append" << std::endl;
    std::cout << append_parser.usage() << std::endl;
    exit(0);
  }
  else
  {
    std::cout << "--Append" << std::endl;
    std::cout << "Ok" << std::endl;
  }
}

void create_op(int argc, char *argv[])
{

  cmdline::parser create_parser;

  create_parser.add<string>("name", 'n', "name of ledger", true);

  if (!create_parser.parse(argc, argv))
  {

    std::cout << "--Create" << std::endl;
    std::cout << create_parser.usage() << std::endl;
    exit(0);
  }
  else
  {
    std::cout << "--Create" << std::endl;
    auto channel = grpc::CreateChannel("localhost:9901", grpc::InsecureChannelCredentials());
    auto client = yuzhi::cli::ledgerClient(channel);
    client.CreateLedger(create_parser.get<string>("name"));
  }
}

int main(int argc, char *argv[])
{
  // create a parser
  cmdline::parser op_parser;

  std::vector<std::pair<std::string, std::function<void(int argc, char *argv[])>>> ops{
      std::pair("Create", std::bind(create_op, std::placeholders::_1, std::placeholders::_2)),
      std::pair("Append", std::bind(append_op, std::placeholders::_1, std::placeholders::_2))};

  for (auto &op : ops)
  {
    op_parser.add<bool>(op.first, 0, "op :" + op.first + " ledger", false);
  }

  if (argc < 2)
  {
    std::cout << op_parser.usage();
    exit(0);
  }

  std::vector<char *> args_{argv[0], argv[1]};

  if (op_parser.parse(2, args_.data()) && argc > 1)
  {
    args_.pop_back();
    for (size_t i = 2; i < argc; i++)
    {
      args_.push_back(argv[i]);
    }
    for (auto &op : ops)
    {
      if (op_parser.exist(op.first))
      {
        op.second((int)(args_.size()), args_.data());
      }
    }
  }
  else
  {
    std::cout << op_parser.usage();
  }

  return 0;
}
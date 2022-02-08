/*
 * @Author: Leo
 * @Date: 2022-02-08 15:39:01
 * @LastEditors: Leo
 * @LastEditTime: 2022-02-08 22:50:38
 */
#define STRIP_FLAG_HELP 1
#include <cmdline.hpp>
#include <functional>
#include <gflags/gflags.h>
#include <iostream>
#include <spdlog/spdlog.h>
#include <string>

using namespace std;

void create_op(int argc, char *argv[]) {

  cmdline::parser create_parser;

  create_parser.add<string>("name", 'n', "name of leadger", true);

  create_parser.add<string>("type", 't', "protocol type", true, "http",
                            cmdline::oneof<string>("file", "str"));

  if (!create_parser.parse(argc, argv)) {

    std::cout << "--Create" << std::endl;
    std::cout << create_parser.usage() << std::endl;
    exit(0);
  } else {
    std::cout << "--Create" << std::endl;
    std::cout << "Ok" << std::endl;
  }
}

int main(int argc, char *argv[]) {
  // create a parser
  cmdline::parser op_parser;

  std::vector<std::pair<std::string, std::function<void()>>> ops{
      std::pair("Create", std::bind(create_op, argc, argv)),

      std::pair("Append", []() { spdlog::info("Append"); })};

  for (auto &op : ops) {
    op_parser.add<bool>(op.first, 0, "op :" + op.first + " Leadger", false);
  }

  if (op_parser.parse(argc, argv)) {
    for (auto &op : ops) {
      if (op_parser.exist(op.first)) {
        op.second();
      }
    }
  } else {
    std::cout << op_parser.usage();
  }

  return 0;
}
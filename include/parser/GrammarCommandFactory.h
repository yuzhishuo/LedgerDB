//
// Created by yuzhi on 5/29/22.
//

#ifndef LEDGERDB_GRAMMARCOMMADFACTORY_H
#define LEDGERDB_GRAMMARCOMMADFACTORY_H

#include <common/SelfDictionary.h>
#include <memory>
#include <parser/IGrammarCommand.h>
#include <spdlog/spdlog.h>
#include <utility/Enum.h>

namespace yuzhi::grammar {

  class GrammarCommandFactory {

public:
    static GrammarCommandFactory &Instance() {
      static GrammarCommandFactory factory{};
      return factory;
    }

    void add(std::shared_ptr<IGrammarCommand> command) {
      if (auto err = selfDictionary_.Add(command); err == Error::RepeatKey()) {
        SPDLOG_ERROR("exist repeat key, program will exit");
        exit(-1);
      }
    }

    IGrammarCommand &get(GrammarType key) {
      if (auto command = selfDictionary_.Get(key); command) {
        return *command;
      }
      SPDLOG_ERROR("capture not exist key, program will exit");
      exit(-1);
    }

    IGrammarCommand &get(const std::string &key) {
      if (auto opt = magic_enum::enum_cast<GrammarType>(key); opt.has_value()) {
        return get(opt.value());
      }
      SPDLOG_ERROR("capture not exist key, program will exit");
      exit(-1);
    }

private:
    GrammarCommandFactory() = default;
    ~GrammarCommandFactory() = default;

private:
    SelfDictionary<std::shared_ptr<IGrammarCommand>, IGrammarCommand::Key> selfDictionary_;
  };
}// namespace yuzhi::grammar

#endif//LEDGERDB_GRAMMARCOMMADFACTORY_H

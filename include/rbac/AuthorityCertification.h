/*
 * @Author: Leo
 * @Date: 2022-02-14 02:36:28
 * @LastEditors: Leo
 * @LastEditTime: 2022-07-22 09:20:26
 */
#pragma once

#include <mutex>
#include <string>

#include "LedgerStrategy.h"
#include "Strategy.h"
#include "UserStrategy.h"
#include "meta/Ledger.h"
#include "meta/User.h"

class AuthorityCertification
{
public:
  AuthorityCertification() {}

  bool UserPass(const std::string &command_name,
                const std::string &user_name) const;

  bool LedgerPass(const std::string &command_name, const std::string &user_name,
                  const std::string &ledger_name) const;

  bool RegisterStrategy(std::unique_ptr<IStrategy> &&strategy);

public:
  static void Start()
  {
    UserAuthorityCertificationHelper;
    LedgerAuthorityCertificationHelper;
  }

  static AuthorityCertification &Instance()
  {
    static AuthorityCertification instance;
    return instance;
  }

private:
  std::map<std::string, std::unique_ptr<IStrategy>> strategys_;
};

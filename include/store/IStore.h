/*
 * @Author: Leo
 * @Date: 2022-02-01 21:47:19
 * @LastEditTime: 2022-02-01 22:52:23
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/store/IStore.h
 */
#pragma once

#include <string>
#include <optional>
#include "common/Error.h"

class IStorage
{

public:
    virtual ~IStorage() = default;
    virtual std::optional<Error> save(const std::string &key, const std::string &value) = 0;
    virtual std::pair<std::string, std::optional<Error>> load(const std::string &key) = 0;
    virtual std::optional<Error> delete_key(const std::string &key) = 0;
    virtual std::optional<Error> update_key(const std::string &key, const std::string &value) = 0;
};
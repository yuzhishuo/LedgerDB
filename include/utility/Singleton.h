/*
 * @Author: Yimin Liu
 * @Date: 2022-01-25 16:36:55
 * @LastEditTime: 2022-02-01 21:56:29
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/utility/Singleton.h
 */
#pragma once

namespace yuzhi::tool {


template <typename T> auto &Instance() { return T::Instance(); }
} // namespace yuzhi::tool

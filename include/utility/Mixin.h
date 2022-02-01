/*
 * @Author: Leo
 * @Date: 2022-02-01 23:21:40
 * @LastEditTime: 2022-02-02 00:24:55
 * @LastEditors: Leo
 * @Description: 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /example-authority-cpp/include/utility/Mixin.h
 */
#pragma once

#ifndef YUZHI_TOOL_MIXIN_H_
#define YUZHI_TOOL_MIXIN_H_

#include <type_traits>

namespace yuzhi::tool {

// note:
// https://www.zhihu.com/question/279734963#:~:text=%E9%80%9A%E5%B8%B8%20this%20%E6%8C%87%E9%92%88%E5%9C%A8%E5%AF%B9%E8%B1%A1%E6%9E%84%E9%80%A0%E5%AE%8C%E6%AF%95%E5%90%8E%E6%89%8D%E5%AE%8C%E5%85%A8%E7%94%9F%E6%88%90%EF%BC%8C%E8%80%8C%E5%9C%A8%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0%E6%89%A7%E8%A1%8C%E8%BF%87%E7%A8%8B%E4%B8%AD%EF%BC%8C%E5%AF%B9%E8%B1%A1%E8%BF%98%E6%B2%A1%E6%9C%89%E5%AE%8C%E5%85%A8%E7%94%9F%E6%88%90%EF%BC%8C%E6%89%80%E4%BB%A5,this%20%E6%8C%87%E9%92%88%E4%B9%9F%E6%98%AF%E6%B2%A1%E6%9C%89%E5%AE%8C%E5%85%A8%E7%94%9F%E6%88%90%E7%9A%84%EF%BC%8C%E5%9C%A8%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0%E4%B8%AD%E4%BD%BF%E7%94%A8%20%E2%80%A6
template <typename T, typename... Mixins>
class Mixin : public T, public Mixins... {
  // static_assert(!std::has_virtual_destructor_v<Mixins>...,
  //               "Mixin can not have virtual destructor");
  // static_cast(std::is_base_of<T, Mixin<T, Mixins...>>::value,
  //             "Mixin can not have virtual destructor");

public:
  template <typename... Args>
  Mixin(Args &&... args) : T{args...}, Mixins{this}... {}
};

}; // namespace yuzhi::tool

#endif // YUZHI_TOOL_MIXIN_H_
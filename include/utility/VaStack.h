
#pragma once

#include<stdarg.h> // for va_list, va_start, va_end, va_arg
namespace yuzhi::utility {

    class VaStack {

    public:
        explicit VaStack(va_list& list)
        {
            va_copy(list_, list);
        }

        ~VaStack()
        {
            va_end(list_);
        }

        VaStack(VaStack&) = delete;
        VaStack& operator = (VaStack&) = delete;

        template<typename T>
        auto pop () -> T
        {
            return va_arg(list_,T);
        }
    private:
        va_list list_;
    };

} // yuzhi::utility

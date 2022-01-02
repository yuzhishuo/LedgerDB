#pragma once

namespace inner
{
    namespace operators_detail
    {

        template <typename T>
        struct empty_base
        {
        };

    } // namespace operators_detail
    template <class T, class U, class B = operators_detail::empty_base<T>>
    struct less_than_comparable2 : B
    {
        friend bool operator<=(const T &x, const U &y) { return !static_cast<bool>(x > y); }
        friend bool operator>=(const T &x, const U &y) { return !static_cast<bool>(x < y); }
        friend bool operator>(const U &x, const T &y) { return y < x; }
        friend bool operator<(const U &x, const T &y) { return y > x; }
        friend bool operator<=(const U &x, const T &y) { return !static_cast<bool>(y < x); }
        friend bool operator>=(const U &x, const T &y) { return !static_cast<bool>(y > x); }
    };

    template <class T, class B = operators_detail::empty_base<T>>
    struct less_than_comparable1 : B
    {
        friend bool operator>(const T &x, const T &y) { return y < x; }
        friend bool operator<=(const T &x, const T &y) { return !static_cast<bool>(y < x); }
        friend bool operator>=(const T &x, const T &y) { return !static_cast<bool>(x < y); }
    };

} // namespace

template <typename T>
using less_than_comparable = inner::less_than_comparable1<T>;
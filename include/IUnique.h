#include <utility>
#include <string>

#include "utility/opeartors.h"
template <typename T, typename = std::void_t<>>
struct is_comparable : std::false_type
{
};

template <typename T>
struct is_comparable<T, std::void_t<
                            // TODO: Do not rely on the default constructor
                            decltype(T{} > T{} && T{} <= T{})()>> : std::true_type
{
};

template <typename T>
constexpr bool is_comparable_v = is_comparable<T>::value;

template <typename T>
class IUnique : public less_than_comparable<IUnique<T>>
{
public:
    static_assert(is_comparable_v<T>); // true

    IUnique(const T &value) : unique_(value) {}
    IUnique() = default;

    template <typename U>
    friend bool operator<(const IUnique<U> &lhs, const IUnique<U> &rhs);

public:
    virtual ~IUnique() = default;
    virtual const T &GetUnique() const
    {
        return unique_;
    }

private:
    const T unique_;
};

template <typename U>
bool operator<(const IUnique<U> &lhs, const IUnique<U> &rhs)
{
    return lhs.GetUnique() > rhs.GetUnique();
};

class UniqueString : public IUnique<std::string>
{
} gg;
static_assert(is_comparable_v<UniqueString>); // true
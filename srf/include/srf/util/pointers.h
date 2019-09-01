#pragma once

#include <string>
#include <memory>

namespace srf
{
namespace util
{



template <typename _ResType>
class res_ptr
{
public:

    _ResType operator * () { return resource_; }

    bool loaded();

    //res_ptr<_ResType> clone() {}
private:
    int count_;
    _ResType* resource_;
};

// TODO : dynamic cast function





//template <typename _Type>
//class owned_ptr
//{
//
//};
//
//
//template <typename _Type>
//class leased_ptr
//{
//
//};

template <typename _Type>
using sptr = std::shared_ptr<_Type>;
template <typename _Type>
using wptr = std::weak_ptr<_Type>;


}
}
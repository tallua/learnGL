#pragma once

#include <vector>

namespace srf
{
namespace util
{

template<typename _ObjectType, int Size>
class ObjectPool
{
public:
    ObjectPool()
        :_objects(Size)
    {

    }

private:
    std::vector<_ObjectType*> _objects;
};

}
}
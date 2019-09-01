#pragma once

#include <string>
#include "../util/pointers.h"

namespace srf
{
namespace system
{



/// GC based resource manager
/// 
class ResoruceSystem
{




public:
    template <typename _ResType>
    void save(std::string name);
    template <typename _ResType>
    void save_async(std::string name);

    template <typename _ResType>
    void load(std::string path);
    template <typename _ResType>
    void load_async(std::string path);

    template <typename _ResType>
    void set_timeout(int ms);

    template <typename _ResType>
    void get(std::string name);

};


}
}




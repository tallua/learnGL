#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/common.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace srf
{
namespace math
{


#ifndef SRF_DISABLE_NS_GLM
using namespace glm;
#else
using ivec2 = glm::ivec2;
#endif


}
}
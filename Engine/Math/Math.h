#pragma once

#include "glm/glm.hpp"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace zn::math
{
    //-----------------------------------------------------------------------------
    // Vectors
    //-----------------------------------------------------------------------------
    using v3 = glm::vec3;
    using v2 = glm::vec2;
    using v4 = glm::vec4;

    // Int vectors
    using iv2 = glm::ivec2; 
    using iv3 = glm::ivec3; 
    using iv4 = glm::ivec4; 

    // Unsigned vectors
    using uv2 = glm::uvec2;
    using uv3 = glm::uvec3;
    using uv4 = glm::uvec4;

    //-----------------------------------------------------------------------------
    // Matrices
    //-----------------------------------------------------------------------------
    using m4 = glm::mat4;
    using m3 = glm::mat3;
    using m2 = glm::mat2;

    //-----------------------------------------------------------------------------
    // Quaternion Types (Requires <glm/gtc/quaternion.hpp>)
    //-----------------------------------------------------------------------------
    using quat  = glm::quat; 
}




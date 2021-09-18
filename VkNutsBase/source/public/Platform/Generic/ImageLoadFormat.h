#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

namespace nuts {
    enum class ImageLoadFormat : int {
        Default   = 0, // STBI_default
        Grey      = 1, // STBI_grey
        GreyAlpha = 2, // STBI_grey_alpha
        RGB       = 3, // STBI_rgb
        RGBA      = 4, // STBI_rgb_alpha
    };
}

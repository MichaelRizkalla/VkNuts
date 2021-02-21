#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Attachment/Attachment.h>

namespace nuts {
    class Module : Attachment< Module > {
        DEFAULT_INTERFACE(Module)
    };
} // namespace nuts
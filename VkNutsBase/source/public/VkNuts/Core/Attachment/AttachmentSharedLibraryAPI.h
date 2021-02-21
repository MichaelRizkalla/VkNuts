#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Attachment/AttachmentAPI.h>

namespace nuts {

    struct AttachmentSharedLibraryAPI : public AttachmentAPI {
        AttachmentSharedLibraryAPI() : mHandle { nullptr } {}
        virtual ~AttachmentSharedLibraryAPI() {
            if (mHandle) { OnUnload(); }
        }

        void OnLoad() noexcept override {
#if defined(NUTS_OS_WINDOWS)
            mHandle = ::LoadLibraryA(GetAttachmentName());
#elif defined(NUTS_OS_LINUX)
            mHandle = dlopen(mAttachmentName, RTLD_NOW | RTLD_LOCAL);
#else
    #error "OS not yet supported!"
#endif
        }
        void OnUnload() noexcept override {
            if (mHandle) {
#if defined(NUTS_OS_WINDOWS)
                ::FreeLibrary(static_cast< HMODULE >(mHandle));
#elif defined(NUTS_OS_LINUX)
                dlclose(mHandle);
#else
    #error "OS not yet supported!"
#endif
            }
        }

      protected:
        void* mHandle;
    };

} // namespace nuts
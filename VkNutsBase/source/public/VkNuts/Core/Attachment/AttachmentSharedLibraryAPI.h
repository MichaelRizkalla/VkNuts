#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Attachment/AttachmentAPI.h>

namespace nuts {

    struct AttachmentSharedLibraryAPI : public AttachmentAPI {
        AttachmentSharedLibraryAPI() : mHandle { nullptr } {}
        virtual ~AttachmentSharedLibraryAPI() {
            if (mHandle) { onUnload(); }
        }

        void onLoad() noexcept override {
#if defined(NUTS_OS_WINDOWS)
            mHandle = ::LoadLibraryA(getAttachmentName());
#elif defined(NUTS_OS_LINUX)
            mHandle = dlopen(mAttachmentName, RTLD_NOW | RTLD_LOCAL);
#else
    #error "OS not yet supported!"
#endif
        }
        void onUnload() noexcept override {
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
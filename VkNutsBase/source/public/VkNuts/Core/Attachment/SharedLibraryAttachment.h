#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Attachment/Attachment.h>

namespace nuts {

    /// <summary>
    /// 
    /// </summary>
    struct SharedLibraryAttachment : public Attachment {
        [[nodiscard]] explicit SharedLibraryAttachment(const char* name) : Attachment { name }, mHandle { nullptr } {
        }
        virtual ~SharedLibraryAttachment() {
            if (mHandle) {
                onUnload();
            }
        }

        [[nodiscard]] bool onLoad() noexcept override {
            try {
#if defined(NUTS_OS_WINDOWS)
                mHandle = ::LoadLibraryA(getAttachmentName());
#elif defined(NUTS_OS_LINUX)
                mHandle = dlopen(mAttachmentName, RTLD_NOW | RTLD_LOCAL);
#else
    #error "OS not yet supported!"
#endif
                return true;
            } catch (...) {
                return false;
            }
        }
        bool onUnload() noexcept override {
            try {
                if (mHandle) {
#if defined(NUTS_OS_WINDOWS)
                    ::FreeLibrary(static_cast< HMODULE >(mHandle));
#elif defined(NUTS_OS_LINUX)
                    dlclose(mHandle);
#else
    #error "OS not yet supported!"
#endif
                }
                return true;
            } catch (...) {
                return false;
            }
        }

      protected:
        void* mHandle;
    };

} // namespace nuts
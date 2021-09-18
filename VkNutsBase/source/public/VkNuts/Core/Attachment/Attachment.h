#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

namespace nuts {

    /// <summary>
    /// Attachment is a base object that represents any loaded entity in the Engine
    /// </summary>
    struct Attachment {
        [[nodiscard]] explicit Attachment(const char* name) : mAttachmentName(name) {
        }

        [[nodiscard]] const char* getAttachmentName() const noexcept {
            return mAttachmentName;
        }

        [[nodiscard]] virtual bool onLoad() noexcept   = 0;
        virtual bool               onUnload() noexcept = 0;

        template < class TAttachmentType, class Allocator = NutsAllocator< TAttachmentType >, class... Types >
        static UniqueRef< Attachment > createAttachment(const char* name, Types&&... types) {
            return allocate_unique< Attachment, TAttachmentType >(Allocator {}, name, types);
        }

      protected:
        const char* mAttachmentName;
    };

} // namespace nuts

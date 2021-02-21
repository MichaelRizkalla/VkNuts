#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

namespace nuts {

    /// <summary>
    /// This structs presents the interface that an Attachment should include
    /// It can be used to create children with different APIs, SharedLibraries, binaries, etc...
    /// </summary>
    struct AttachmentAPI {
        DEFAULT_CTOR(AttachmentAPI)
        VIRTUAL_DTOR(AttachmentAPI)

        DELETE_COPY_CLASS(AttachmentAPI)
        DELETE_MOVE_CLASS(AttachmentAPI)

        virtual void        OnLoad() noexcept                  = 0;
        virtual void        OnUnload() noexcept                = 0;
        virtual const char* GetAttachmentName() const noexcept = 0;
    };

} // namespace nuts
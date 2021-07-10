#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Attachment/AttachmentSharedLibraryAPI.h>

namespace nuts {

    /// <summary>
    /// Attachment is a shared library that carries information that can be used in the Engine
    /// The shared library interface should be defined in a struct/class _AttachmentData
    ///   And then define GetAttachmentData function
    /// </summary>
    /// <typeparam name="_AttachmentData">The data that is returned from the attachment</typeparam>
    /// <typeparam name="_AttachmentAPI">The API which the Attachment will act upon</typeparam>
    template < typename TAttachmentData, typename TAttachmentAPI = AttachmentSharedLibraryAPI >
    requires(std::is_base_of_v< AttachmentAPI, TAttachmentAPI >) struct Attachment : public TAttachmentAPI {
        using attachment_data = TAttachmentData;
        using attachment_api  = TAttachmentAPI;

        Attachment(const char* name) : mAttachmentName(name) {}
        virtual ~Attachment() = default;

        const char* getAttachmentName() const noexcept override { return mAttachmentName; }

        virtual attachment_data getAttachmentData() const = 0;

      protected:
        const char* mAttachmentName;
    };

} // namespace nuts
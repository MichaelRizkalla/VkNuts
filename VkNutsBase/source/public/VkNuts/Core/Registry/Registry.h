#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Attachment/Attachment.h>

namespace nuts {
    template < class TAttachmentType >
    requires(
        std::is_base_of_v< Attachment< typename TAttachmentType::attachment_data, typename TAttachmentType::attachment_api >, TAttachmentType >) struct RegistryQuery {
      public:
        virtual typename TAttachmentType::attachment_data QueryAttachment(const char* alias) const = 0;
    };

    struct RegistryInitializer {};

    template < class TAttachmentType, class TRegistryQueryAPI = RegistryQuery< TAttachmentType > >
    class Registry : public TRegistryQueryAPI {
      protected:
        DEFAULT_INTERFACE(Registry)

      public:
        using attachment_type      = TAttachmentType;
        using attachment_data_type = typename TAttachmentType::attachment_data;
        using attachment_api_type  = typename TAttachmentType::attachment_api;

        virtual void Init(RegistryInitializer*) noexcept = 0;
        // TODO: Rename the function maybe?
        template < class... Types >
        bool AttachAttachment(const char* alias, const char* nameOnDesk, Types&&... _InTypes) noexcept {
            try {
                std::lock_guard guard { mMutex };
                auto            result = std::find_if(mRegistry.begin(), mRegistry.end(), [&](auto& itr) {
                    if (itr.first == alias) return true;
                    if (std::strcmp(itr.second->GetAttachmentName(), nameOnDesk) == 0) return true;
                    return false;
                });
                if (result != mRegistry.end()) {
                    if (result->first == alias) {
                        NUTS_ENGINE_WARN("An attachment with similar alias as {} is already loaded!", alias);
                    } else {
                        NUTS_ENGINE_WARN("The attachment with another alias as {} is already loaded!", result->first);
                    }
                    return false;
                }
                auto mAttachment = std::make_unique< attachment_type >(nameOnDesk, std::forward< Types&& >(_InTypes)...);
                mAttachment->OnLoad();

                mRegistry.insert(std::make_pair(alias, std::move(mAttachment)));
                return true;
                // TODO: Better manage exception
            } catch (std::exception& e) {
                NUTS_ENGINE_ERROR("AttachAttachment threw an exception: {}", e.what());
                return false;
            }
        }
        bool DetachAttachment(const char* alias) noexcept {
            std::lock_guard guard { mMutex };
            if (!HasAttachment(alias)) {
                NUTS_ENGINE_WARN("An attachment with name {} is not loaded!", alias);
                return false;
            }
            mRegistry.at(alias)->OnUnload();
            mRegistry.erase(alias);
            return true;
        }
        void DetachAllAttachments() noexcept {
            std::lock_guard guard { mMutex };
            mRegistry.clear();
        }
        bool HasAttachment(const char* alias) const noexcept {
            try {
                return mRegistry.contains(alias);
            } catch (std::exception& e) {
                NUTS_ENGINE_ERROR("HasAttachment failed with exception: {}", e.what());
                return false;
            }
        }

        // Better to return a const ref, to avoid copies ??
        const std::unordered_map< const char*, UniqueRef< attachment_type > >& GetAttachments() const noexcept { return mRegistry; }

      protected:
        // Alias -- Attachment
        std::unordered_map< std::string, UniqueRef< attachment_type > > mRegistry;
        std::mutex                                                     mMutex;
    };
} // namespace nuts
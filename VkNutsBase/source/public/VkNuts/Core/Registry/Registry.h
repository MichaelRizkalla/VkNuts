#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Attachment/Attachment.h>

namespace nuts {
    struct RegistryInitializer {};

    /// <summary>
    ///
    /// </summary>
    /// <typeparam name="TAttachmentType"></typeparam>
    /// <typeparam name="Allocator"></typeparam>
    template < class TAttachmentType, class Allocator = std::pmr::polymorphic_allocator< TAttachmentType > >
    class Registry {
      protected:
        DEFAULT_CTOR(Registry)
        DEFAULT_DTOR(Registry)
        DELETE_MOVE_CLASS(Registry)
        DELETE_COPY_CLASS(Registry)

      public:
        using allocator_type  = Allocator;
        using attachment_type = TAttachmentType;

        virtual void init(const RegistryInitializer* const) noexcept = 0;
        virtual void finalize() noexcept {
            detachAllAttachments();
        }
        // TODO: Rename the function maybe?
        template < class... Types >
        bool attachAttachment(const char* alias, const char* nameOnDesk, Types&&... _InTypes) noexcept {
            try {
                std::lock_guard guard { mMutex };
                auto            result = std::find_if(mRegistry.begin(), mRegistry.end(), [&](auto& itr) {
                    if (itr.first == alias)
                        return true;
                    if (std::strcmp(itr.second->getAttachmentName(), nameOnDesk) == 0)
                        return true;
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
                auto mAttachment = allocate_unique< attachment_type >(allocator_type {}, nameOnDesk, std::forward< Types&& >(_InTypes)...);
                if (!mAttachment->onLoad()) {
                    return false;
                }

                mRegistry.insert(std::make_pair(alias, std::move(mAttachment)));
                return true;
                // TODO: Better manage exception
            } catch (std::exception& e) {
                NUTS_ENGINE_ERROR("AttachAttachment threw an exception: {}", e.what());
                return false;
            }
        }
        bool detachAttachment(const char* alias) noexcept {
            std::lock_guard guard { mMutex };
            if (!hasAttachment(alias)) {
                NUTS_ENGINE_WARN("An attachment with name {} is not loaded!", alias);
                return false;
            }
            mRegistry.at(alias)->onUnload();
            mRegistry.erase(alias);
            return true;
        }
        void detachAllAttachments() noexcept {
            std::lock_guard guard { mMutex };
            mRegistry.clear();
        }
        bool hasAttachment(const char* alias) const noexcept {
            try {
                return mRegistry.contains(alias);
            } catch (std::exception& e) {
                NUTS_ENGINE_ERROR("HasAttachment failed with exception: {}", e.what());
                return false;
            }
        }
        const attachment_type* const getAttachment(const char* alias) const noexcept {
            try {
                return mRegistry.at(alias).get();
            } catch (std::out_of_range& e) {
                NUTS_ENGINE_ERROR("getAttachment failed with exception: {}", e.what());
                return nullptr;
            }
        }

        // Better to return a const ref, to avoid copies ??
        const HashMap< const char*, UniqueRef< attachment_type > >& getAttachments() const noexcept {
            return mRegistry;
        }

      protected:
        // Alias -- Attachment
        HashMap< String, UniqueRef< attachment_type > > mRegistry;
        std::mutex                                      mMutex;
    };
} // namespace nuts
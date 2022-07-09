#pragma once

// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/Core/Attachment/Attachment.h>
#include <VkNuts/Core/Log/Log.h>

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

        struct attach_from_desk {};
        struct attach_created_attachment {};

      public:
        using allocator_type  = Allocator;
        using attachment_type = TAttachmentType;

        virtual void init(const RegistryInitializer* const) noexcept = 0;
        virtual void finalize() noexcept {
            detachAllAttachments();
        }
        // TODO: Rename the function maybe?
        template < class TAttchment = attachment_type, class... Types >
        [[nodiscard]] bool createAttachmentFromDesk(const char* alias, const char* nameOnDesk, Types&&... _InTypes) noexcept {
            try {
                std::scoped_lock guard { mMutex };

                if (isDuplicateAlias< attach_from_desk >(alias, nameOnDesk)) {
                    return false;
                }

                auto mAttachment = UniqueRef< TAttchment >(new TAttchment(nameOnDesk, std::forward< Types&& >(_InTypes)...));
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
        [[nodiscard]] void attachAttachment(const char* alias, UniqueRef< attachment_type > attachment) {
            std::scoped_lock guard { mMutex };

            if (isDuplicateAlias< attach_created_attachment >(alias)) {
                throw std::exception { "An attachment is loaded with similar alias!" };
            }

            if (!attachment->onLoad()) {
                throw std::exception { "An attachment failed when loading!" };
            }

            mRegistry.insert(std::make_pair(alias, std::move(attachment)));
        }
        [[nodiscard]] bool detachAttachment(const char* alias) noexcept {
            std::scoped_lock guard { mMutex };
            if (!hasAttachment(alias)) {
                NUTS_ENGINE_WARN("An attachment with name {} is not loaded!", alias);
                return false;
            }
            mRegistry.at(alias)->onUnload();
            mRegistry.erase(alias);
            return true;
        }
        void detachAllAttachments() noexcept {
            std::scoped_lock guard { mMutex };
            for (auto& [key, value] : mRegistry) {
                value->onUnload();
            }
            mRegistry.clear();
        }
        [[nodiscard]] bool hasAttachment(const char* alias) const noexcept {
            try {
                return mRegistry.contains(alias);
            } catch (std::exception& e) {
                NUTS_ENGINE_ERROR("HasAttachment failed with exception: {}", e.what());
                return false;
            }
        }
        [[nodiscard]] const attachment_type* const getAttachment(const char* alias) const noexcept {
            try {
                return mRegistry.at(alias).get();
            } catch (std::out_of_range& e) {
                NUTS_ENGINE_ERROR("getAttachment failed with exception: {}", e.what());
                return nullptr;
            }
        }

        // Better to return a const ref, to avoid copies ??
        [[nodiscard]] const HashMap< String, UniqueRef< attachment_type > >& getAttachments() const noexcept {
            return mRegistry;
        }

      protected:
        template < class T >
        inline bool isDuplicateAlias(const char* alias, const char* nameOnDesk = "None") noexcept {
            typename decltype(mRegistry)::iterator result {};
            if constexpr (std::same_as< T, attach_from_desk >) {
                result = std::find_if(mRegistry.begin(), mRegistry.end(), [&](auto& itr) {
                    if (itr.first == alias)
                        return true;
                    if (std::strcmp(itr.second->getAttachmentName(), nameOnDesk) == 0)
                        return true;
                    return false;
                });
            } else if constexpr (std::same_as< T, attach_created_attachment >) {
                result = std::find_if(mRegistry.begin(), mRegistry.end(), [&](auto& itr) {
                    if (itr.first == alias)
                        return true;
                    return false;
                });
            }

            if (result != mRegistry.end()) {
                if (result->first == alias) {
                    NUTS_ENGINE_WARN("An attachment with similar alias as {} is already loaded!", alias);
                }
                return true;
            }
            return false;
        }
        // Alias -- Attachment
        HashMap< String, UniqueRef< attachment_type > > mRegistry;
        std::mutex                                      mMutex;
    };
} // namespace nuts

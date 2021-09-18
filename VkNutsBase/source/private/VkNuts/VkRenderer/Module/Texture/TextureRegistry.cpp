// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/VkRenderer/Module/Texture/TextureRegistry.h>

namespace nuts {

    void TextureRegistry::init(const RegistryInitializer* const init) noexcept {
        // TODO: do a runtime type check with dynamic_cast
        auto init_ = static_cast< const TextureRegistryInitializer* const >(init);

        mDefaultDevice = init_->mDefaultDevice;
        mAllocator     = init_->mAllocator;
    }
    bool TextureRegistry::createImage(const char* alias, const vk::ImageCreateInfo& imageCreateInfo, const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept {
        if (!hasAttachment(alias)) {
            return false;
        }
        std::scoped_lock guard { mMutex };
        return mRegistry.at(alias)->createImage(imageCreateInfo, memoryPropertyFlags);
    }
    bool TextureRegistry::createTexture(const char* alias, const vk::ImageCreateInfo& imageCreateInfo, const vk::ImageViewCreateInfo& imageViewCreateInfo,
                                        const vk::MemoryPropertyFlags memoryPropertyFlags) noexcept {
        if (!hasAttachment(alias)) {
            return false;
        }
        std::scoped_lock guard { mMutex };
        return mRegistry.at(alias)->createTexture(imageCreateInfo, imageViewCreateInfo, memoryPropertyFlags);
    }
    bool TextureRegistry::destroyTexture(const char* alias) noexcept {
        if (!hasAttachment(alias)) {
            return false;
        }

        std::scoped_lock guard { mMutex };
        mRegistry.at(alias)->destroyTexture();
        return true;
    }
    void TextureRegistry::setSampler(const char* alias, vk::Sampler sampler) {
        if (!hasAttachment(alias)) {
            throw std::out_of_range { std::string { "Texture with alias: " } + alias + " was not found!" };
        }

        std::scoped_lock guard { mMutex };
        mRegistry.at(alias)->setSampler(sampler);
    }

} // namespace nuts

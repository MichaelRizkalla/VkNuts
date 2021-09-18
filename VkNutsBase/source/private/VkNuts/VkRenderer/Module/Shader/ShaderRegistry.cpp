// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <VkNuts/VkRenderer/Module/Shader/ShaderRegistry.h>

namespace nuts {

    void ShaderRegistry::init(const RegistryInitializer* const init) noexcept {
        // TODO: do a runtime type check with dynamic_cast
        mDefaultDevice = static_cast< const ShaderRegistryInitializer* const >(init)->mDefaultDevice;
    }
    bool ShaderRegistry::createVkShader(const char* alias) noexcept {
        if (!hasAttachment(alias)) {
            return false;
        }
        std::lock_guard guard { mMutex };
        return mRegistry.at(alias)->createVkShader(mDefaultDevice);
    }
    bool ShaderRegistry::destroyVkShader(const char* alias) noexcept {
        if (!hasAttachment(alias)) {
            return false;
        }
        std::lock_guard guard { mMutex };
        mRegistry.at(alias)->destroyVkShader();
        return true;
    }

} // namespace nuts

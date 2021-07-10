#pragma once
// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Utilities/Utilities.hpp>

namespace nuts {

    /// <summary>
    /// This class is following the example mentioned at:
    /// https://github.com/nvpro-samples/shared_sources/blob/master/nvvk/samplers_vk.hpp
    /// And copies the hash function from it in "sampler_hash_fn" below
    /// </summary>
    struct VkSamplerManager {
        DEFAULT_CLASS(VkSamplerManager)
        using sampler_count = uint64_t;
        using sampler_index = uint64_t;

        NUTS_API [[nodiscard]] inline void init(vk::Device device) noexcept;
        NUTS_API [[nodiscard]] inline void finalize() noexcept;

        NUTS_API [[nodiscard]] inline vk::Sampler createSampler(const vk::SamplerCreateInfo& samplerCreateInfo) noexcept;
        NUTS_API [[nodiscard]] inline void        destroySampler(vk::Sampler sampler) noexcept;

      private:
        struct CreateInfo {
            vk::SamplerCreateInfo                samplerCreateInfo;
            vk::SamplerReductionModeCreateInfo   samplerReductionModeCreateInfo;
            vk::SamplerYcbcrConversionCreateInfo samplerYcbcrConversionCreateInfo;

            CreateInfo() { std::memset(this, 0, sizeof(decltype(this))); }

            bool operator==(const CreateInfo& rhs) const { return std::memcmp(this, &rhs, sizeof(decltype(this))) == 0; }
            bool operator!=(const CreateInfo& rhs) const { return !(*this == rhs); }
        };
        struct sampler_hash_fn {
            std::size_t operator()(const CreateInfo& samplerCreateInfo) const {
                std::hash< uint32_t > hasher;
                const uint32_t*       data = (const uint32_t*)&samplerCreateInfo;
                size_t                seed = 0;
                for (size_t i = 0; i < sizeof(CreateInfo) / sizeof(uint32_t); i++) {
                    // https://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html
                    seed ^= hasher(data[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                }
                return seed;
            }
        };

        struct Entry {
            vk::Sampler   sampler { nullptr };
            sampler_index nextFreeIndex { static_cast< sampler_index >(-1) };
            sampler_count refCount { 0 };
            CreateInfo    createInfo {};
        };

        HashMap< CreateInfo, sampler_index, sampler_hash_fn > mCreateInfos {};
        HashMap< vk::Sampler, sampler_count >                 mSamplers {};
        Vector< Entry >                                       mSamplerEntries {};
        sampler_index                                         mFreeIndex { static_cast< sampler_index >(-1) };
        vk::Device                                            mDevice { nullptr };
    };
} // namespace nuts

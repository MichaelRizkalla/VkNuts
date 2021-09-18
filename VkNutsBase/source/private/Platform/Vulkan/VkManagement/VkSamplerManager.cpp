// clang-format off
#include <NutsPCH.h>
// clang-format on

#include <Platform/Vulkan/VulkanHelper.h>
#include <Platform/Vulkan/VkManagement/VkSamplerManager.h>

namespace nuts {

    void VkSamplerManager::init(vk::Device device) noexcept { mDevice = device; }
    void VkSamplerManager::finalize() noexcept {
        for (auto&& createInfo : mCreateInfos) {
            auto samplerEntry = mSamplerEntries.at(createInfo.second);
            mDevice.destroySampler(samplerEntry.sampler, nullptr);
        }
        mCreateInfos.clear();
        mSamplers.clear();
        mSamplerEntries.clear();
        mFreeIndex = static_cast< sampler_index >(-1);
    }
    vk::Sampler VkSamplerManager::createSampler(const vk::SamplerCreateInfo& samplerCreateInfo) noexcept {
        assert(!mDevice && "Please initialize VulkanSampler before using it");
        CreateInfo createInfo {};
        createInfo.samplerCreateInfo = samplerCreateInfo;

        auto next = reinterpret_cast< const vk::BaseInStructure* >(samplerCreateInfo.pNext);
        while (next) {
            switch (next->sType) {
                case vk::StructureType::eSamplerReductionModeCreateInfo:
                    createInfo.samplerReductionModeCreateInfo = *reinterpret_cast< const vk::SamplerReductionModeCreateInfo* >(next);
                    break;
                case vk::StructureType::eSamplerYcbcrConversionCreateInfo:
                    createInfo.samplerYcbcrConversionCreateInfo = *reinterpret_cast< const vk::SamplerYcbcrConversionCreateInfo* >(next);
                    break;
                default:
                    assert(0 && "Incorrect sampler create info chain");
            }
            next = next->pNext;
        }

        createInfo.samplerCreateInfo.pNext                = nullptr;
        createInfo.samplerReductionModeCreateInfo.pNext   = nullptr;
        createInfo.samplerYcbcrConversionCreateInfo.pNext = nullptr;

        auto mCreateInfo = mCreateInfos.find(createInfo);
        if (mCreateInfo == mCreateInfos.end()) {
            sampler_index index = 0;
            if (mFreeIndex != static_cast< sampler_index >(-1)) {
                index      = mFreeIndex;
                mFreeIndex = mSamplerEntries[index].nextFreeIndex;
            } else {
                index = static_cast< sampler_index >(mSamplerEntries.size());
                mSamplerEntries.resize(mSamplerEntries.size() + 1);
            }

            vk::Sampler sampler;
            auto        result = mDevice.createSampler(&samplerCreateInfo, nullptr, &sampler);
            assert(result == vk::Result::eSuccess);

            mSamplerEntries[index].refCount   = 1;
            mSamplerEntries[index].sampler    = sampler;
            mSamplerEntries[index].createInfo = createInfo;

            mCreateInfos.try_emplace(createInfo, index);
            mSamplers.try_emplace(sampler, index);

            return sampler;
        } else {
            mSamplerEntries[mCreateInfo->second].refCount++;
            return mSamplerEntries[mCreateInfo->second].sampler;
        }
    }
    void VkSamplerManager::destroySampler(vk::Sampler sampler) noexcept {
        auto mSampler = mSamplers.find(sampler);
        assert(mSampler != mSamplers.end());

        sampler_index index = mSampler->second;
        Entry&        entry = mSamplerEntries[index];

        assert(entry.sampler == sampler);
        assert(entry.refCount);

        entry.refCount--;

        if (!entry.refCount) {
            mDevice.destroySampler(sampler, nullptr);
            entry.sampler       = nullptr;
            entry.nextFreeIndex = mFreeIndex;
            mFreeIndex          = index;

            mCreateInfos.erase(entry.createInfo);
            mSamplers.erase(sampler);
        }
    }

} // namespace nuts

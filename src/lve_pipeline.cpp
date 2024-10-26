#include "lve_pipeline.hpp"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace lve
{
LvePipeline::LvePipeline(LveDevice& device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineCreateInfo& createInfo) : device{device} {
    CreatePipeline(vertFilePath, fragFilePath, createInfo);
}

std::vector<char> LvePipeline::ReadFile(const std::string& filePath) {
    std::ifstream file{filePath, std::ios::ate | std::ios::binary};

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

void LvePipeline::CreatePipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineCreateInfo& createInfo) {
    auto vertCode = ReadFile(vertFilePath);
    auto fragCode = ReadFile(fragFilePath);
}

void LvePipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    if (vkCreateShaderModule(device.device, &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module!");
    }
};

PipelineCreateInfo LvePipeline::DefaultPipelineCreateInfo(uint32_t width, uint32_t height) {
    PipelineCreateInfo createInfo{};

    return createInfo;
};
}
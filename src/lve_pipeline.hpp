#pragma once

#include "lve_device.hpp"

#include <string>
#include <vector>

namespace lve {
struct PipelineCreateInfo {};

class LvePipeline {
public:
    LvePipeline(LveDevice& device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineCreateInfo& createInfo);

    LvePipeline(const LvePipeline&) = delete;
    void operator=(const LvePipeline&) = delete;

    static PipelineCreateInfo DefaultPipelineCreateInfo(uint32_t width, uint32_t height);
private:
    LveDevice &device;
    VkPipeline pipeline;
    VkShaderModule vertShader;
    VkShaderModule fragShader;

    static std::vector<char> ReadFile(const std::string& filePath);

    void CreatePipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineCreateInfo& createInfo);
    void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
};
}
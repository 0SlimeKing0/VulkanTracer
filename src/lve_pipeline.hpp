#pragma once

#include "lve_device.hpp"

#include <spdlog/spdlog.h>

#include <string>
#include <vector>

namespace lve {
struct PipelineConfigInfo {
  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
  VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
  VkPipelineMultisampleStateCreateInfo multisampleInfo{};
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};

class LvePipeline {
 public:
  LvePipeline(
      LveDevice& device,
      const std::string& vertFilepath,
      const std::string& fragFilepath,
      const PipelineConfigInfo& configInfo);

  LvePipeline(const LvePipeline&) = delete;
  LvePipeline &operator=(const LvePipeline&) = delete;

  void Bind(VkCommandBuffer commandBuffer);
  void CleanUp();

  static PipelineConfigInfo DefaultPipelineConfigInfo(uint32_t width, uint32_t height);
 private:
  static std::vector<char> ReadFile(const std::string& filepath);

  void CreateGraphicsPipeline(
     const std::string& vertFilepath,
      const std::string& fragFilepath,
      const PipelineConfigInfo& configInfo);

  void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

  LveDevice& device;
  VkPipeline graphicsPipeline;
  VkShaderModule vertShaderModule;
  VkShaderModule fragShaderModule;
};
}
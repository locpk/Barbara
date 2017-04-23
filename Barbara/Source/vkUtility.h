#pragma once
#include "SharedDefine.h"

namespace VkUtilities
{


	static const std::vector<const char*> deviceExtensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	static const std::vector<const char*> validationLayers =
	{
		"VK_LAYER_LUNARG_standard_validation",
	};
#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif



	struct QueueFamilyIndices
	{
		int graphicsFamily = -1;
		int presentFamily = -1;
		int transferFamily = -1;
		int computeFamily = -1;

		bool isComplete()
		{
			return graphicsFamily >= 0 && presentFamily >= 0 && transferFamily >= 0;
		}

		bool isComputeAvaliable()
		{
			return computeFamily >= 0;
		}
	};

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};


#pragma region DebugHelper
	VkResult CreateDebugReportCallbackEXT(
		VkInstance instance,
		const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDebugReportCallbackEXT* pCallback);

	void DestroyDebugReportCallbackEXT(
		VkInstance instance,
		VkDebugReportCallbackEXT callback,
		const VkAllocationCallbacks* pAllocator);

	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugReportFlagsEXT flags,
		VkDebugReportObjectTypeEXT objType,
		uint64_t obj,
		size_t location,
		int32_t code,
		const char* layerPrefix,
		const char* msg,
		void* userData);

#pragma endregion




	VkFormat findSupportedFormat(VkPhysicalDevice& physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	inline VkFormat findDepthFormat(VkPhysicalDevice& physicalDevice)
	{
		return findSupportedFormat(physicalDevice,
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	};
	inline bool hasStencilComponent(VkFormat format)
	{
		return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
	};

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkValidationLayerSupport();
	bool checkGLFWRequiredExtensions();
	std::vector<const char*> getRequiredExtensions();

	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VDeleter<VkSurfaceKHR>& surface);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VDeleter<VkSurfaceKHR>& surface);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	bool isDeviceSuitable(VkPhysicalDevice device, VDeleter<VkSurfaceKHR>& surface);

	uint32_t findMemoryType(VkPhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
	void createBuffer(VkPhysicalDevice& physicalDevice, VDeleter<VkDevice>& device, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VDeleter<VkBuffer>& buffer, VDeleter<VkDeviceMemory>& bufferMemory);
	void createImage(VkPhysicalDevice& physicalDevice, VDeleter<VkDevice>& device, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VDeleter<VkImage>& image, VDeleter<VkDeviceMemory>& imageMemory);
	void createImageView(VDeleter<VkDevice>& device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VDeleter<VkImageView>& imageView);

	void transitionImageLayout(VDeleter<VkDevice>& device, VDeleter<VkCommandPool>& commandPool, VkQueue& queue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
	VkCommandBuffer beginSingleTimeCommands(VDeleter<VkDevice>& device, VDeleter<VkCommandPool>& commandPool);
	void endSingleTimeCommands(VkCommandBuffer commandBuffer, VDeleter<VkDevice>& device, VDeleter<VkCommandPool>& commandPool, VkQueue& queue);

	void copyBuffer(VDeleter<VkDevice>& device, VDeleter<VkCommandPool>& commandPool, VkQueue& queue, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void copyImage(VDeleter<VkDevice>& device, VDeleter<VkCommandPool>& commandPool, VkQueue& queue, VkImage srcImage, VkImage dstImage, uint32_t width, uint32_t height);


	void createShaderModule(VDeleter<VkDevice>& device, const std::vector<char>& code, VDeleter<VkShaderModule>& shaderModule);

	//Refactoring
	void allocateDescriptorSets(VDeleter<VkDevice>& device, const VkDescriptorSetLayout* descriptorSetLayout, VkDescriptorPool descriptorPool, VkDescriptorSet* descriptorSet);
	void buildDescriptorPool(VDeleter<VkDevice>& device, VkDescriptorPool* descriptorPool, VkDescriptorType type, uint32_t descriptorCount);

	void buildDescriptorSetLayout(VDeleter<VkDevice>& device, VkDescriptorSetLayout* descriptorSetLayout,
		uint32_t bindingNumber, uint32_t descriptorCount, VkDescriptorType descriptorType,
		VkShaderStageFlagBits stageFlags = VK_SHADER_STAGE_ALL,
		const VkSampler* immutableSamplers = nullptr);

	void createSampler(VDeleter<VkDevice>& device, VkSampler* sampler,
		VkFilter filter, VkSamplerAddressMode samplerAddressMode);
}
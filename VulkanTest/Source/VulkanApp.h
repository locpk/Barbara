#pragma once

#define GLFW_INCLUDE_VULKAN
#include <vector>
#include <GLFW/glfw3.h>
#include "VDeleter.h"

const int WIDTH = 1280;
const int HEIGHT = 720;


const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

static std::vector<const char*> validationLayers =
{
	"VK_LAYER_LUNARG_standard_validation",
};
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


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




struct QueueFamilyIndices 
{
	int graphicsFamily = -1;
	int presentFamily = -1;

	bool isComplete()
	{
		return graphicsFamily >= 0 && presentFamily >= 0;
	}
};

struct SwapChainSupportDetails 
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};



class VulkanApp
{
public:
	void run();
	void recreateSwapChain();
private:
	GLFWwindow* window;

	void initWindow();
	void initVulkan();
	void mainLoop();

	void createInstance();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSurface();
	void createSwapChain();
	void createImageViews();
	void createRenderPass();
	void createGraphicsPipeline();
	void createShaderModule(const std::vector<char>& code, VDeleter<VkShaderModule>& shaderModule);
	void createFramebuffers();
	void createCommandPool();
	void createCommandBuffers();
	void createSemaphores();

	

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkValidationLayerSupport();
	bool checkGLFWRequiredExtensions();
	std::vector<const char*> getRequiredExtensions();
	void setupDebugCallback();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	bool isDeviceSuitable(VkPhysicalDevice device);


	void draw();

private:
	//Order matters
	VDeleter<VkInstance> instance{ vkDestroyInstance };
	VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };
	VDeleter<VkDevice> device{ vkDestroyDevice };
	VDeleter<VkSurfaceKHR> surface{ instance, vkDestroySurfaceKHR };
	VDeleter<VkSwapchainKHR> swapChain{ device, vkDestroySwapchainKHR };
	std::vector<VkImage> swapChainImages;
	std::vector<VDeleter<VkImageView>> swapChainImageViews;
	VDeleter<VkRenderPass> renderPass{ device, vkDestroyRenderPass };
	VDeleter<VkPipelineLayout> pipelineLayout{ device, vkDestroyPipelineLayout };
	VDeleter<VkPipeline> graphicsPipeline{ device, vkDestroyPipeline };
	std::vector<VDeleter<VkFramebuffer>> swapChainFramebuffers;
	VDeleter<VkCommandPool> commandPool{ device, vkDestroyCommandPool };
	std::vector<VkCommandBuffer> commandBuffers;
	VDeleter<VkSemaphore> imageAvailableSemaphore{ device, vkDestroySemaphore };
	VDeleter<VkSemaphore> renderFinishedSemaphore{ device, vkDestroySemaphore };


	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VDeleter<VkDebugReportCallbackEXT> callback{ instance,DestroyDebugReportCallbackEXT };

};


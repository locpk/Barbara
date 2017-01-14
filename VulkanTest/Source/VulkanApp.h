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

private:
	GLFWwindow* window;

	void initWindow();
	void initVulkan();
	void mainLoop();

	void createInstance();
	void pickPhysicalDevice();
	void createLogicalDevice();
	void createSurface();

	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	bool checkValidationLayerSupport();
	bool checkGLFWRequiredExtensions();
	std::vector<const char*> getRequiredExtensions();
	void setupDebugCallback();
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	bool isDeviceSuitable(VkPhysicalDevice device);

private:
	VDeleter<VkInstance> instance{ vkDestroyInstance };
	VkPhysicalDevice physicalDevice{ VK_NULL_HANDLE };
	VDeleter<VkDevice> device{ vkDestroyDevice };
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VDeleter<VkSurfaceKHR> surface{ instance, vkDestroySurfaceKHR };

	VDeleter<VkDebugReportCallbackEXT> callback{ instance,DestroyDebugReportCallbackEXT };

};


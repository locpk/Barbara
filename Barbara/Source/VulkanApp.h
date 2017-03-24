#pragma once
#include "vkUtility.h"
using namespace VkUtilities;
#include"Model.h"
#include "camera.hpp"
#include "SimpleCamera.hpp"





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
	void createDescriptorSetLayout();
	void createDescriptorPool();
	void createDescriptorSet();
	void createGraphicsPipeline();
	void createShaderModule(const std::vector<char>& code, VDeleter<VkShaderModule>& shaderModule);
	void createFramebuffers();
	void createCommandPools();
	void createCommandBuffers();
	void createSemaphores();
	void createDepthResources();
	void createTextureSampler();



	void InitUniformBuffer();
	void setupDebugCallback();



	void update();
	void draw();
	void loadModel();

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
	VDeleter<VkDescriptorSetLayout> descriptorSetLayout{ device, vkDestroyDescriptorSetLayout };
	VDeleter<VkDescriptorPool> descriptorPool{ device, vkDestroyDescriptorPool };
	VDeleter<VkPipelineLayout> pipelineLayout{ device, vkDestroyPipelineLayout };
	VDeleter<VkPipeline> graphicsPipeline{ device, vkDestroyPipeline };
	std::vector<VDeleter<VkFramebuffer>> swapChainFramebuffers;
	VDeleter<VkCommandPool> commandPool{ device, vkDestroyCommandPool };
	VDeleter<VkCommandPool> transferCommandPool{ device, vkDestroyCommandPool };
	std::vector<VkCommandBuffer> commandBuffers;
	VDeleter<VkSemaphore> imageAvailableSemaphore{ device, vkDestroySemaphore };
	VDeleter<VkSemaphore> renderFinishedSemaphore{ device, vkDestroySemaphore };


	VDeleter<VkImage> depthImage{ device, vkDestroyImage };
	VDeleter<VkDeviceMemory> depthImageMemory{ device, vkFreeMemory };
	VDeleter<VkImageView> depthImageView{ device, vkDestroyImageView };


	Camera camera;
	SimpleCamera sCamera;
	std::unique_ptr<Model> testMesh;

	//Deferred 
	std::array<VDeleter<VkImage>, 3> frameBufferObjects;
	std::array<VDeleter<VkDeviceMemory>, 3> FOBMemories;
	std::array<VDeleter<VkImageView>, 3> FOBViews;


	VDeleter<VkSampler> textureSampler{ device, vkDestroySampler };


	VDeleter<VkBuffer> uniformStagingBuffer{ device, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> uniformStagingBufferMemory{ device, vkFreeMemory };
	VDeleter<VkBuffer> uniformBuffer{ device, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> uniformBufferMemory{ device, vkFreeMemory };

	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkQueue computeQueue;
	VkQueue transferQueue;
	VkDescriptorSet descriptorSet;

	VDeleter<VkDebugReportCallbackEXT> callback{ instance,DestroyDebugReportCallbackEXT };

};


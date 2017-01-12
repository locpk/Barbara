#include "VulkanApp.h"
#include <vector>
#include <iostream>
#include <algorithm>
void VulkanApp::run()
{
	initWindow();
	initVulkan();
	mainLoop();
}

void VulkanApp::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}


bool CheckGLFWRequiredExtensions(unsigned int glfwExtensionCount, const char** glfwExtensions, std::vector<VkExtensionProperties>& extensions)
{
	unsigned int count = 0;
	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		for (auto& e : extensions)
		{
			if (0 == std::strcmp(glfwExtensions[i], e.extensionName))
			{
				count++;
				break;
			}
		}
		
	}
	return (count == glfwExtensionCount);
}

void VulkanApp::createInstance()
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan App";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledLayerCount = 0;
	if (VK_SUCCESS == vkCreateInstance(&createInfo, nullptr, instance.replace()))
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
		std::cout << "Avaliable Extensions:\n";
		for (auto& e : extensions)
		{
			std::cout << "\t" << e.extensionName << "\n";
		}

		if (CheckGLFWRequiredExtensions(glfwExtensionCount, glfwExtensions, extensions))
		{
			std::cout << "Clear!";
		}
	}
	
}

void VulkanApp::initVulkan()
{
	createInstance();
}

void VulkanApp::mainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}
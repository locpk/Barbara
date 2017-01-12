#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VDeleter.h"
const int WIDTH = 800;
const int HEIGHT = 600;
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

private:
	VDeleter<VkInstance> instance{ vkDestroyInstance };
};


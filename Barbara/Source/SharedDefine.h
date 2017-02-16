#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "VDeleter.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

//STD
#include <iostream>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <string>
#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>
#pragma once
#include "SharedDefine.h"


class Mesh
{
private:
	const std::string meshName;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	VDeleter<VkDevice>& device;
	VDeleter<VkBuffer> viBuffer{ device, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> viBufferMemory{ device, vkFreeMemory };
private:
	void CreateMeshBuffer(VkPhysicalDevice& physicalDevice, VDeleter<VkCommandPool>& commandPool, VkQueue& queue);
	
	
public:
	Mesh(VDeleter<VkDevice>& _device, std::string _meshName = "CESAR");
	~Mesh();

	void LoadMesh(VkPhysicalDevice& physicalDevice, VDeleter<VkCommandPool>& commandPool, VkQueue& queue);

	inline const VDeleter<VkBuffer>& GetBuffer() const
	{
		return viBuffer;
	}

	inline const std::string& GetMeshPath() const
	{
		static const std::string path =  "Source/models/" + meshName + ".obj";
		return path;
	}

	inline const std::vector<Vertex>& GetVertices() const
	{
		return vertices;
	}
	inline const std::vector<uint32_t>& GetIndices() const
	{
		return indices;
	}
};


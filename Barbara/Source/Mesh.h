#pragma once
#include "SharedDefine.h"


class Mesh
{
private:
	const std::string meshName;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	std::vector<VDeleter<VkImage>> textures;
	std::vector<VDeleter<VkDeviceMemory>> textureImageMemories;
	std::vector<VDeleter<VkImageView>> textureImageViews;

	VDeleter<VkDevice>& device;
	VDeleter<VkBuffer> viBuffer{ device, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> viBufferMemory{ device, vkFreeMemory };
private:
	void CreateMeshBuffer(VkPhysicalDevice& physicalDevice, VDeleter<VkCommandPool>& commandPool, VkQueue& queue);
	void LoadTexture(VkPhysicalDevice& physicalDevice, VDeleter<VkCommandPool>& commandPool, VkQueue& queue, const std::string& filename, size_t& index);

public:
	Mesh(VDeleter<VkDevice>& _device, std::string _meshName = "CESAR");
	~Mesh();

	void LoadMesh(VkPhysicalDevice& physicalDevice, VDeleter<VkCommandPool>& commandPool, VkQueue& queue);


	inline const VDeleter<VkBuffer>& GetBuffer() const
	{
		return viBuffer;
	}

	inline const std::vector<VDeleter<VkImageView>>& GetTextureViews() const
	{
		return textureImageViews;
	}

	inline const std::string& GetMeshPath() const
	{
		static const std::string path = "Source/models/" + meshName + ".obj";
		return path;
	}

	inline static const std::string& GetMaterialPath()
	{
		static const std::string path = "Source/materials/";
		return path;
	}

	inline static const std::string& GetTexturePath()
	{
		static const std::string path = "Source/textures/";
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


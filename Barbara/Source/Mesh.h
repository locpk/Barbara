#pragma once
#include "SharedDefine.h"
#include "tiny_obj_loader.h"

class Mesh
{
private:
	static VDeleter<VkBuffer> s_indexBuffer; //Shared index buffer
	static VDeleter<VkDeviceMemory> s_indexBufferMemory;
private:
	VDeleter<VkDevice>& device;
	const std::string meshName;
	int32_t materialID;


	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;


	//int indexBufferOffset;
	VDeleter<VkBuffer> viBuffer{ device, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> viBufferMemory{ device, vkFreeMemory };
private:
	

public:
	Mesh(VDeleter<VkDevice>& _device, std::string _meshName = "CESAR");
	~Mesh();

	Mesh(Mesh&& other);

	void LoadMesh( tinyobj::attrib_t& attrib, tinyobj::shape_t& shape);
	void CreateMeshBuffer(VkPhysicalDevice& physicalDevice, VDeleter<VkCommandPool>& commandPool, VkQueue& queue);


	inline const int32_t GetMaterialID() const
	{
		return materialID;
	}

	inline const VDeleter<VkBuffer>& GetBuffer() const
	{
		return viBuffer;
	}

	inline const std::string& GetMeshPath() const
	{
		static const std::string path = "Source/models/" + meshName + ".obj";
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


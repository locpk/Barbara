#pragma once
#include "SharedDefine.h"

class Mesh;
class Material;
static const std::string MESH_FOLDER { "Source/models/" };
static const std::string MATERIAL_FOLDER { "Source/materials/" };
class Model
{
private:
	const std::string modelName;

	std::vector<Mesh> meshes;
	std::vector<Material> materials;
public:
	Model(VDeleter<VkDevice>& _device, VkPhysicalDevice& physicalDevice, VDeleter<VkCommandPool>& commandPool, VkQueue& queue, std::string _modelName);
	~Model();

	const VDeleter<VkBuffer>& GetFirstMeshBuffer() const;
	const std::vector<Vertex>& GetFirstMeshVertices() const;
	const std::vector<uint32_t>& GetFirstMeshIndices() const;
	const std::vector<VDeleter<VkImageView>>& GetFirstMaterialTextureViews() const;
};


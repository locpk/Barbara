#pragma once
#include "SharedDefine.h"

class Mesh;
class Material;
static const std::string MESH_FOLDER{ "Source/models/" };
static const std::string MATERIAL_FOLDER{ "Source/materials/" };
class Model
{
private:
	const std::string modelName;

	std::vector<Mesh> meshes;
	std::vector<Material> materials;
public:
	Model(VDeleter<VkDevice>& _device, VkPhysicalDevice& physicalDevice, VDeleter<VkCommandPool>& commandPool, VkQueue& queue, std::string _modelName);
	~Model();


	size_t GetMeshCount() const;

	size_t GetMaterialCount() const;

	size_t GetMeshVerticesCount(size_t index) const;
	size_t GetMeshIndicesCount(size_t index) const;

	const VDeleter<VkBuffer>& GetMeshBuffer(size_t index) const;
	const std::vector<VDeleter<VkImageView>>& GetMaterials(size_t index) const;

	const std::vector<VDeleter<VkImageView>>& GetFirstMaterialTextureViews() const;
};


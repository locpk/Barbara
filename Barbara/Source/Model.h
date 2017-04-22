#pragma once
#include "SharedDefine.h"

class Mesh;
class Material;

class Model
{
private:
	static constexpr char MESH_FOLDER[] = "Source/models/" ;
	static constexpr char MATERIAL_FOLDER[] =  "Source/materials/" ;
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

	uint32_t GetMeshMaterialID(size_t index) const;
	const VDeleter<VkBuffer>& GetMeshBuffer(size_t index) const;
	const std::vector<VDeleter<VkImageView>>& GetMaterials(size_t index) const;
};


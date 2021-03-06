#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


Model::Model(VDeleter<VkDevice>& _device, VkPhysicalDevice& physicalDevice, VDeleter<VkCommandPool>& commandPool, VkQueue& queue, std::string _modelName) : modelName(_modelName)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> mats;
	std::string err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &mats, &err, std::string(MESH_FOLDER + _modelName).c_str(), MATERIAL_FOLDER))
	{
		throw std::runtime_error(err);
	}

	//Meshes
	meshes.reserve(shapes.size());
	for (auto& shape : shapes)
	{
		Mesh newMesh{ _device,shape.name, static_cast<uint32_t>(shape.mesh.material_ids[0]) };
		newMesh.LoadMesh(attrib, shape);
		newMesh.CreateMeshBuffer(physicalDevice, commandPool, queue);
		meshes.emplace_back(std::move(newMesh));
	}

	//Materials
	materials.reserve(mats.size());
	for (auto& material : mats)
	{
		//Only load in diffuse textures for now
		Material newMaterial{ _device,material.name };
		materials.emplace_back(std::move(newMaterial));
		materials[materials.size() - 1].LoadTexture(physicalDevice, commandPool, queue, material.diffuse_texname, Material::DIFFUSE);
	}


}


Model::~Model()
{
}


uint32_t Model::GetMeshMaterialID(size_t index) const
{
	return meshes[index].GetMaterialID();
}

size_t Model::GetMeshCount() const
{
	return meshes.size();
}

size_t Model::GetMaterialCount() const
{
	return materials.size();
}


const VDeleter<VkBuffer>& Model::GetMeshBuffer(size_t index) const
{
	return meshes[index].GetBuffer();
};

const std::vector<VDeleter<VkImageView>>& Model::GetMaterials(size_t index) const
{
	return materials[index].GetTextureViews();
};

size_t Model::GetMeshVerticesCount(size_t index) const
{
	return meshes[index].GetVertices().size();
}

size_t Model::GetMeshIndicesCount(size_t index) const
{
	return meshes[index].GetIndices().size();
}


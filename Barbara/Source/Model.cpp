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

	if (!tinyobj::LoadObj(&attrib, &shapes, &mats, &err, std::string(MESH_FOLDER + _modelName).c_str(), MATERIAL_FOLDER.c_str()))
	{
		throw std::runtime_error(err);
	}

	//Meshes
	meshes.reserve(shapes.size());
	for (auto& shape : shapes)
	{
		Mesh newMesh{ _device,shape.name };
		newMesh.LoadMesh(attrib, shape);
		newMesh.CreateMeshBuffer(physicalDevice, commandPool, queue);
		meshes.emplace_back(std::move(newMesh));
	}
	for (auto& mesh : meshes)
	{
		//mesh.CreateMeshBuffer(physicalDevice, commandPool, queue);
	}

	//Materials
	materials.reserve(mats.size());
	for (auto& material : mats)
	{
		Material newMaterial{ _device,material.name };
		materials.emplace_back(std::move(newMaterial));
		materials[materials.size() - 1].LoadTexture(physicalDevice, commandPool, queue, material.diffuse_texname, Material::DIFFUSE);
	}


}


Model::~Model()
{
}


const VDeleter<VkBuffer>& Model::GetFirstMeshBuffer() const
{
	return meshes[0].GetBuffer();
}
const std::vector<Vertex>& Model::GetFirstMeshVertices() const
{
	return meshes[0].GetVertices();
}
const std::vector<uint32_t>& Model::GetFirstMeshIndices() const
{
	return meshes[0].GetIndices();
}

const std::vector<VDeleter<VkImageView>>& Model::GetFirstMaterialTextureViews() const
{
	return materials[0].GetTextureViews();
}

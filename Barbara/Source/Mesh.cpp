#include "Mesh.h"
#include "vkUtility.h"



Mesh::Mesh(VDeleter<VkDevice>& _device, std::string _meshName) : meshName(_meshName), device(_device)
{

}

Mesh::Mesh(Mesh&& other) : device(other.device), meshName(other.meshName)
{

	vertices = std::move(other.vertices);
	indices = std::move(other.indices);
	viBuffer = other.viBuffer;
	viBufferMemory = other.viBufferMemory;

	other.viBuffer.reset();
	other.viBufferMemory.reset();

}


void Mesh::LoadMesh(tinyobj::attrib_t& attrib, tinyobj::shape_t& shape)
{

	std::unordered_map<Vertex, int> uniqueVertices = {};
	for (const auto& index : shape.mesh.indices)
	{
		Vertex vertex = {};
		vertex.pos =
		{
			attrib.vertices[3 * index.vertex_index + 0],
			attrib.vertices[3 * index.vertex_index + 1],
			attrib.vertices[3 * index.vertex_index + 2]
		};

		vertex.texCoord =
		{
			attrib.texcoords[2 * index.texcoord_index + 0],
			attrib.texcoords[2 * index.texcoord_index + 1]
		};

		vertex.color = { 1.0f, 1.0f, 1.0f };

		if (uniqueVertices.count(vertex) == 0)
		{
			uniqueVertices[vertex] = static_cast<int>(vertices.size());
			vertices.push_back(vertex);
		}

		indices.push_back(uniqueVertices[vertex]);

	}


}







void Mesh::CreateMeshBuffer(VkPhysicalDevice& physicalDevice, VDeleter<VkCommandPool>& commandPool, VkQueue& queue)
{
	VkDeviceSize vertexBuffserSize = sizeof(vertices[0]) * vertices.size();
	VkDeviceSize indexBuffserSize = sizeof(indices[0]) * indices.size();
	VkDeviceSize totalSize = vertexBuffserSize + indexBuffserSize;

	VDeleter<VkBuffer> stagingBuffer{ device, vkDestroyBuffer };
	VDeleter<VkDeviceMemory> stagingBufferMemory{ device, vkFreeMemory };
	VkUtilities::createBuffer(physicalDevice, device, totalSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, totalSize, 0, &data);
	void* indexdataEntry = static_cast<void*>(static_cast<char*>(data) + vertexBuffserSize);
	memcpy(data, vertices.data(), (size_t)vertexBuffserSize);
	memcpy(indexdataEntry, indices.data(), (size_t)indexBuffserSize);
	vkUnmapMemory(device, stagingBufferMemory);

	VkUtilities::createBuffer(physicalDevice, device, totalSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, viBuffer, viBufferMemory);

	VkUtilities::copyBuffer(device, commandPool, queue, stagingBuffer, viBuffer, totalSize);
}

Mesh::~Mesh()
{
}

#include "Mesh.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "vkUtility.h"



Mesh::Mesh(VDeleter<VkDevice>& _device, std::string _meshName) : meshName(_meshName), device(_device)
{

}


void Mesh::LoadMesh(VkPhysicalDevice& physicalDevice, VDeleter<VkCommandPool>& commandPool, VkQueue& queue)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, GetMeshPath().c_str(), GetMaterialPath().c_str()))
	{
		throw std::runtime_error(err);
	}

	std::unordered_map<Vertex, int> uniqueVertices = {};
	for (const auto& shape : shapes) {
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
	CreateMeshBuffer(physicalDevice, commandPool, queue);


	//Load textures
	textures.resize(materials.size(), VDeleter<VkImage>(device, vkDestroyImage));
	textureImageMemories.resize(materials.size(), VDeleter<VkDeviceMemory>(device, vkFreeMemory));
	textureImageViews.resize(materials.size(), VDeleter<VkImageView>(device, vkDestroyImageView));
	for (size_t i = 0; i < materials.size(); i++)
	{
		LoadTexture(physicalDevice, commandPool, queue, materials[i].diffuse_texname, i);
	}

}


void Mesh::LoadTexture(VkPhysicalDevice& physicalDevice, VDeleter<VkCommandPool>& commandPool, VkQueue& queue, const std::string& filename, size_t& index)
{
	VDeleter<VkImage>&  textureImage = textures[index];
	VDeleter<VkDeviceMemory>& textureImageMemory = textureImageMemories[index];
	VDeleter<VkImageView>&  textureImageView = textureImageViews[index];

	int texWidth, texHeight, texChannels;
	//Flip for Vulkan
	stbi_set_flip_vertically_on_load(true);

	auto pixels = stbi_load(std::string(GetTexturePath() + filename).c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = texWidth * texHeight * 4;
	if (nullptr == pixels)
	{
		throw std::runtime_error("failed to load texture image!");
	}


	VDeleter<VkImage> stagingImage{ device, vkDestroyImage };
	VDeleter<VkDeviceMemory> stagingImageMemory{ device, vkFreeMemory };

	VkUtilities::createImage(physicalDevice, device, texWidth, texHeight, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingImage, stagingImageMemory);



	VkImageSubresource subresource = {};
	subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subresource.mipLevel = 0;
	subresource.arrayLayer = 0;

	VkSubresourceLayout stagingImageLayout;
	vkGetImageSubresourceLayout(device, stagingImage, &subresource, &stagingImageLayout);

	void* data;
	vkMapMemory(device, stagingImageMemory, 0, imageSize, 0, &data);
	if (stagingImageLayout.rowPitch == texWidth * 4)
	{
		memcpy(data, pixels, (size_t)imageSize);
	}
	else
	{
		uint8_t* dataBytes = reinterpret_cast<uint8_t*>(data);

		for (int y = 0; y < texHeight; y++)
		{
			memcpy(
				&dataBytes[y * stagingImageLayout.rowPitch],
				&pixels[y * texWidth * 4],
				texWidth * 4
			);
		}
	}

	vkUnmapMemory(device, stagingImageMemory);
	stbi_image_free(pixels);

	VkUtilities::createImage(physicalDevice, device,
		texWidth, texHeight,
		VK_FORMAT_R8G8B8A8_UNORM,
		VK_IMAGE_TILING_OPTIMAL,
		VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		textureImage,
		textureImageMemory
	);

	VkUtilities::transitionImageLayout(device, commandPool, queue, stagingImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
	VkUtilities::transitionImageLayout(device, commandPool, queue, textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	VkUtilities::copyImage(device, commandPool, queue, stagingImage, textureImage, texWidth, texHeight);
	VkUtilities::transitionImageLayout(device, commandPool, queue, textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	VkUtilities::createImageView(device, textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, textureImageView);


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

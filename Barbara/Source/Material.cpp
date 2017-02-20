#include "Material.h"
#include "vkUtility.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Material::Material(VDeleter<VkDevice>& _device, std::string _matName) :device(_device), matName(_matName)
{
	textures.resize(TextureIndex::MAX_COUNT, VDeleter<VkImage>(device, vkDestroyImage));
	textureImageMemories.resize(TextureIndex::MAX_COUNT, VDeleter<VkDeviceMemory>(device, vkFreeMemory));
	textureImageViews.resize(TextureIndex::MAX_COUNT, VDeleter<VkImageView>(device, vkDestroyImageView));
}




Material::~Material()
{
}



void Material::LoadTexture(VkPhysicalDevice& physicalDevice, VDeleter<VkCommandPool>& commandPool, VkQueue& queue, const std::string& filename, const TextureIndex& index)
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
	//Free stbi buffer
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
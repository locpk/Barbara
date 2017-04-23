#pragma once
#include "SharedDefine.h"

class Material
{
public:
	enum  TextureIndex : uint16_t
	{
		DIFFUSE = 0, MAX_COUNT
	};
private:
	VDeleter<VkDevice>& device;
	const std::string matName;

	std::vector<VDeleter<VkImage>> textures;
	std::vector<VDeleter<VkDeviceMemory>> textureImageMemories;
	std::vector<VDeleter<VkImageView>> textureImageViews;
public:
	Material(VDeleter<VkDevice>& _device, std::string _matName);
	~Material() = default;

	void LoadTexture(VkPhysicalDevice& physicalDevice, VDeleter<VkCommandPool>& commandPool, VkQueue& queue, const std::string& filename, const TextureIndex& index);

	inline const std::string& GetMaterialPath() const
	{
		static const std::string path = "Source/materials/" + matName + ".mtl";
		return path;
	}

	inline static const std::string& GetTexturePath()
	{
		static const std::string path = "Source/textures/";
		return path;
	}

	inline const std::vector<VDeleter<VkImageView>>& GetTextureViews() const
	{
		return textureImageViews;
	}
};


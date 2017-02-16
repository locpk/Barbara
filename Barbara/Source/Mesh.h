#pragma once
#include "SharedDefine.h"


class Mesh
{
private:
	const std::string meshName;
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
private:
	void LoadMesh();
public:
	Mesh(std::string _meshName = "CESAR");
	~Mesh();

	

	inline const std::string& GetMeshPath()
	{
		static const std::string path =  "Source/models/" + meshName + ".obj";
		return path;
	}

	inline const std::vector<Vertex>& GetVertices()
	{
		return vertices;
	}
	inline const std::vector<uint32_t>& GetIndices()
	{
		return indices;
	}
};


#pragma once
#include <string>
#include <vector>

struct ObjectData {
	std::string name;
	std::vector<float> verteces;
	std::vector<float> uvCoordinates;

	ObjectData(std::vector<float>& verts, std::vector<float>& uvs, std::string n) {
		verteces = verts;
		uvCoordinates = uvs;
		name = n;
	}
};

static class ObjectImporter {
public:
	static std::vector<float> ImportFromFile(std::string& filePath);
};
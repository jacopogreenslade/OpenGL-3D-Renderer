#include "ObjectImporter.h"
#include <fstream>
#include <sstream>
#include <iostream>

/*
	For now let's just get verteces
*/
std::vector<float> ObjectImporter::ImportFromFile(std::string & filePath)
{
	std::ifstream stream(filePath); // Explicit constructor syntax
	std::string line;
	std::vector<float> verts;
	std::vector<float> uvs;

	if (!stream.is_open()) {
		std::cout << "No open filestream!" << std::endl;
	}

	while (getline(stream, line)) {
		std::stringstream ss;
		ss << line;
		
		std::string type;
		ss >> type;
		char junk; // out line starts to this

		if (type == "v") {
			float x, y, z;
			ss >> x >> y >> z;

			verts.push_back(x);
			verts.push_back(y);
			verts.push_back(z);
		}
	}

	// ObjectData obj(verts, uvs, "hello");
	return verts;
}

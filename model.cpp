#include"model.h"
#include<fstream>
#include<sstream>
#include<string>
#include<stdexcept>
Model::~Model() = default;
Model::Model(const char* filename) {
	std::ifstream in(filename);
	if (!in) {
		throw std::runtime_error(std::string("Failed to open:") + filename);//Check
	}
	std::string line;
	while (std::getline(in, line)) {//read the file each line
		if (line.empty() || line[0] == '#')continue;
		std::istringstream iss(line);
		std::string prefix;
		if (!(iss >> prefix))continue;//judge whether success or not
		if (prefix == "v") {
			float x, y, z;
			if (iss >> x >> y >> z) {
				verts_.push_back(Vec3f(x, y, z));
			}
			else {
				continue;
			}
		}
		else if (prefix == "vt") {
			float u, v;
			if (iss >> u >> v)uv_.push_back(Vec2f(u, v));
		}
		else if (prefix == "vn") {
			float nx, ny, nz;
			if (iss >> nx >> ny >> nz)normals_.push_back(Vec3f(nx, ny, nz));
		}
		else if (prefix == "f") {//Attention!there's only index inside f
			std::vector<int>face;
			std::vector<int>face_vt;
			std::vector<int>face_vn;
			std::string token;
			while (iss >> token) {
				size_t p1 = token.find('/');
				if (p1 == std::string::npos) {//not found
					int vi = std::stoi(token) - 1;
					face.push_back(vi);
				}
				else
				{
					size_t p2 = token.find('/', p1 + 1);
					std::string sv = token.substr(0, p1);
					std::string svt;
					std::string svn;
					if (p2 == std::string::npos) {//not found the second
						svt = token.substr(p1 + 1);
					}
					else {
						svt = token.substr(p1 + 1, p2 - p1 - 1);
						svn = token.substr(p2 + 1);
					}
					try {
						int vi = -1, vti = -1, vni = -1;

						//analyse sv
						if (!sv.empty()) {
							vi = std::stoi(sv);
							if (vi < 0) vi = static_cast<int>(verts_.size()) + vi; //deal with negative index
							else vi -= 1; // 1-based -> 0-based
							face.push_back(vi); // save
						}

						// analyse svt
						if (!svt.empty()) {
							vti = std::stoi(svt);
							if (vti < 0) vti = static_cast<int>(uv_.size()) + vti;
							else vti -= 1;
							face_vt.push_back(vti);
						}

						if (!svn.empty()) {
							vni = std::stoi(svn);
							if (vni < 0) vni = static_cast<int>(normals_.size()) + vni;
							else vni -= 1;
							face_vn.push_back(vni);
						}

					}
					catch (const std::exception& e) {
						throw std::runtime_error(std::string("Failed to parse face token '") + token + "': " + e.what());
					}
				}
			}
			if (!face.empty()) {//Why I have a face to save the data,but I also need to push them into another container?
				faces_.push_back(face);
				faces_vt_.push_back(face_vt);
				faces_vn_.push_back(face_vn);
			}
		}
	}
}

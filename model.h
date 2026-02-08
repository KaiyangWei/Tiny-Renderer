//In this file,I read and save the data inside obj file,such as vertex,texture and normal vectors.

#ifndef MODEL_
#define MODEL_
#include"geometry.h"
#include<vector>

class Model
{
private:
	    //Save real coordinates
		std::vector<Vec3f> verts_;
		std::vector<Vec2f> uv_;
		std::vector<Vec3f> normals_;
		//Save index
		std::vector<std::vector<int>>faces_;
		std::vector<std::vector<int>> faces_vt_;
		std::vector<std::vector<int>> faces_vn_;
public:
	Model(const char* filename);
	~Model();
	//Read total 
	int nverts() const { return static_cast<int>(verts_.size()); }
	int nfaces() const { return static_cast<int>(faces_.size()); }
	//Read real coordinates
	const Vec3f& vert(int i)const { return verts_.at(i); }
	const Vec2f& uv(int i)const { return uv_.at(i); }
	const Vec3f& normal(int i)const { return normals_.at(i); }
	//Read index
	const std::vector<int>& face(int i) const { return faces_.at(i); }
	const std::vector<int>& face_vt(int i) const { return faces_vt_.at(i); }
	const std::vector<int>& face_vn(int i) const { return faces_vn_.at(i); }
};
//This is one of the most dificult part I think.As I studied C programming and had such little experience in dealing with file,stream and string in my University's programming classes,I can say
//I know nothing about it except string a="...".So I struggled a lot in it,and I am still learning this part.
#endif // !MODEL_

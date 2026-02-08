//In this file,I definite four matrix,which is the most important matrix in CG to acheive that transport a item inside world to our screen,using a lot knowledge in linear algebra.Luckily,I studied well in this subject.
//And also creat my own customizable shader,convenient for me to achieve another goal in the future
#ifndef OUR_GL_
#define OUR_GL_
#include"geometry.h"
#include"tgaimage.h"
Matrix ModelView(float scale, float angle, Vec3f trans, Vec3f axis);
Matrix Viewport(float width, float height, float depth);
Matrix Lookat(Vec3f up, Vec3f center, Vec3f eye);
Matrix Projection(Vec3f eye, Vec3f center);
Vec3f barycentric(Vec3f* pts, Vec3f P);
struct Ishader {
	virtual ~Ishader() = default;//Ishader should be given detailed function in main.cpp
	virtual Vec4f vertex(int iface, int nthvert) = 0;
	virtual bool fragment(Vec3f bar, TGAColor& color) {
			float sum = bar.x + bar.y + bar.z;//sum must be 1
			if (std::abs(sum - 1.0f) > 1e-5f) {//true means abandon
				return true;
			}
			if (bar.x < 0 || bar.y < 0 || bar.z < 0 || bar.x>1 || bar.y>1 || bar.z>1) {
				return true;
			}
			color = TGAColor(255, 0, 0, 255);
			return false;
	}
};
void triangle(const Vec4f* clip, Ishader& shader, TGAImage& image, float* zbuffer);
#endif//!OUR_GL_









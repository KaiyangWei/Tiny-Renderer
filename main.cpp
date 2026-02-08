//As I create shader in our_gl,I should state the function in main,and regulate various parameters
#include "our_gl.h"
#include "model.h"
#include <vector>
#include <limits>
#include <iostream>
#include <direct.h>
#include <algorithm>

Vec3f eye(0, 0, 1);
Vec3f center(0, 0, 0);
Vec3f up(0, 1, 0);
Vec3f light_dir(0.0f, 0.0f, 1.0f);

const float width = 800;
const float height = 800;
const Matrix VP = Viewport(width, height, 1);
const Matrix View = Lookat(up, center, eye);
const Matrix Proj = Projection(eye, center);

int main()
{
	char cwd[_MAX_PATH];
	_getcwd(cwd, _MAX_PATH);
	std::cout << "Working Dir: " << cwd << "\n";//I didn't load tga file successfully,so I intended to find path

	TGAImage Image(width, height, TGAImage::RGB);
	Model model("african_head.obj");

	TGAImage diffuse;
	if (!diffuse.read_tga_file("african_head_diffuse.tga")) {
		std::cerr << "read file failed\n";
		return 1;
	}
	diffuse.flip_vertically();
	std::vector<float> zbuffer(width * height, -std::numeric_limits<float>::infinity());//init to -inf,so first depth test always passes

	struct Myshader : public Ishader
	{
		Model* model_ptr = nullptr;
		TGAImage* diffuse_ptr = nullptr;
		Vec3f vert[3];
		Vec2f uv[3];
		Vec3f normal[3];
		float w[3];

		virtual Vec4f vertex(int iface, int nthvert) override {
			//read indices in this face
			const std::vector<int>& f = model_ptr->face(iface);
			const std::vector<int>& fuv = model_ptr->face_vt(iface);
			const std::vector<int>& fn = model_ptr->face_vn(iface);

			if (nthvert < 0 || nthvert >= (int)f.size()) return Vec4f();
			if (nthvert < (int)fuv.size()) uv[nthvert] = model_ptr->uv(fuv[nthvert]);
			if (nthvert < (int)fn.size()) normal[nthvert] = model_ptr->normal(fn[nthvert]);

			Vec3f v = model_ptr->vert(f[nthvert]);
			Vec4f v4(v.x, v.y, v.z, 1.0f);//creat homogeneous coordinates

			Vec4f clip = (Proj * (View * v4));//into clipping space(clip is still homogeneous)
			w[nthvert] = clip.w;//save w, later use 1/w for perspective correction interpolation

			Vec3f ndc(clip.x / clip.w, clip.y / clip.w, clip.z / clip.w);//Normalized Device Coordinate
			Vec4f screen = VP * Vec4f(ndc.x, ndc.y, ndc.z, 1.0f);//into screen space
			Vec3f p(screen.x / screen.w, screen.y / screen.w, screen.z / screen.w);

			return Vec4f(p.x, p.y, p.z, 1);
		}

		virtual bool fragment(Vec3f bar, TGAColor& color) override {
			float invw0 = 1.0f / w[0];
			float invw1 = 1.0f / w[1];
			float invw2 = 1.0f / w[2];

			float denom = bar.x * invw0 + bar.y * invw1 + bar.z * invw2;//perspective correction interpolation's denominator
			if (std::abs(denom) < 1e-6f) return true;

			Vec2f uvp = (uv[0] * (bar.x * invw0) +
				uv[1] * (bar.y * invw1) +
				uv[2] * (bar.z * invw2)) * (1.0f / denom);//color perspective correction interpolation

			Vec3f n = (normal[0] * (bar.x * invw0) +
				normal[1] * (bar.y * invw1) +
				normal[2] * (bar.z * invw2)) * (1.0f / denom);//normal vector perspective correction interpolation
			n = n.normalize();
			float intensity = std::max(0.0f, Vec3f::dot(n, light_dir));//Lambert lighting

			int tx = std::min(diffuse_ptr->get_width() - 1, std::max(0, int(uvp.x * diffuse_ptr->get_width())));//get color in tga file
			int ty = std::min(diffuse_ptr->get_height() - 1, std::max(0, int(uvp.y * diffuse_ptr->get_height())));
			TGAColor base = diffuse_ptr->get(tx, ty);
			color = TGAColor(
				(unsigned char)(base.r * intensity),
				(unsigned char)(base.g * intensity),
				(unsigned char)(base.b * intensity),
				base.a
			);
			return false;
		}
	};

	Myshader shader;
	shader.model_ptr = &model;//connect pointers
	shader.diffuse_ptr = &diffuse;

	for (int i = 0; i < model.nfaces(); i++) {//traverse all faces(triangles)
		Vec4f clip[3];
		for (int j = 0; j < 3; j++) {
			clip[j] = shader.vertex(i, j);
		}
		triangle(clip, shader, Image, zbuffer.data());
	}
	Image.flip_vertically();
	if (!Image.write_tga_file("output.tga")) {
		std::cerr << "write_tga_file failed\n";
		return 1;
	}

	return 0;
}
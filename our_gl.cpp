#include"geometry.h"
#include"our_gl.h"
#include"model.h"
#include<cmath>
#include<algorithm>
Model* model_ptr;
Matrix ModelView(float scale, float angle, Vec3f trans,Vec3f axis)//Control the rotation ,scaling, transport of an item
{
	Matrix Scale;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j)Scale[i][j] = scale;
		}
	}
	Matrix Rotate;
	const float pi = 3.1415926535897932384626433;
	float rad = angle * (pi / 180);
	float c = cos(rad);
	float s = sin(rad);
	axis = axis.normalize();
	//using Rodrigues' rotation formula,but I think there must be simpler way to creat this matrix
	Rotate[0][0] = c + axis.x * axis.x * (1 - c);
	Rotate[1][0] = axis.x * axis.y * (1 - c) + axis.z * s;//Ðý×ª·½Ïò´æÒÉ
	Rotate[2][0] = axis.x * axis.z * (1 - c) - axis.y * s;
	Rotate[0][1] = axis.x * axis.y * (1 - c) - axis.z * s;
	Rotate[1][1] = c + axis.y * axis.y * (1 - c);
	Rotate[2][1] = axis.y * axis.z * (1 - c) + axis.x * s;
	Rotate[0][2] = axis.x * axis.z * (1 - c) + axis.y * s;
	Rotate[1][2] = axis.y * axis.z * (1 - c) - axis.x * s;
	Rotate[2][2] = c + axis.z * axis.z * (1 - c);
	Matrix Trans;
	Trans[0][3] = trans.x;
	Trans[1][3] = trans.y;
	Trans[2][3] = trans.z;
	return Trans * (Rotate * Scale);//THE ORDER MUSTN'T BE CHANGED (scale->rotate->translate)
}

Matrix Viewport(float width,float height,float depth)
{
	//Put the item on the screen (NDC -> screen)
	Matrix View;
	View[0][0] = width / 2;
	View[0][3] = width / 2;
	View[1][1] = height / 2;
	View[1][3] = height / 2;
	View[2][2] = depth / 2;
	View[2][3] = depth / 2;
	return View;
}

Matrix Projection(Vec3f eye, Vec3f center)
{
	Matrix Proj;
	Proj[3][2] = (-1.f) / (eye - center).norm();//Control the size of the item through w (perspective)
	return Proj;
}

Matrix Lookat(Vec3f up, Vec3f center, Vec3f eye)//Change the old axis to the new
{
	Vec3f z = (eye - center).normalize();//z is opposite of vision
	Vec3f x = (up ^ z).normalize();
	Vec3f y = (z ^ x).normalize();

	Matrix View;
	for (int i = 0; i < 3; i++) {
		View[0][i] = x[i];
		View[1][i] = y[i];
		View[2][i] = z[i];
	}

	View[0][3] = -Vec3f::dot(x, eye);//move eye to origin in view space
	View[1][3] = -Vec3f::dot(y, eye);
	View[2][3] = -Vec3f::dot(z, eye);

	return View;
}




Vec3f barycentric(Vec3f* pts, Vec3f P)//compute barycentric coordinates
{
	Vec3f res = Vec3f(pts[1][0] - pts[0][0], pts[2][0] - pts[0][0], P[0] - pts[0][0]) ^ Vec3f(pts[1][1] - pts[0][1], pts[2][1] - pts[0][1], P[1] - pts[0][1]);
	if (std::abs(res[2]) < 1e-2) return Vec3f(-1, 1, 1);//degenerate triangle
	Vec3f u = Vec3f(1 + (res[0] + res[1]) / res[2], -res[0] / res[2], -res[1] / res[2]);
	return u;
}

void triangle(const Vec4f* clip, Ishader& shader, TGAImage& image,float* zbuffer)//*****CORE FUNCTION
{
	Vec3f pts[3] = {
		Vec3f(clip[0].x, clip[0].y, clip[0].z),
		Vec3f(clip[1].x, clip[1].y, clip[1].z),
		Vec3f(clip[2].x, clip[2].y, clip[2].z),
	};//three points of a triangle
	float w = image.get_width();
	float h= image.get_height();

	Vec2f bboxmin, bboxmax;
	bboxmax.x = 0;
	bboxmax.y = 0;
	bboxmin.x = w - 1;
	bboxmin.y = h - 1;
	for (int i = 0; i < 3; i++)
	{
		bboxmin.x = std::max(0.f, std::min(bboxmin.x, pts[i].x));//compute bbox to reduce workload
		bboxmin.y = std::max(0.f, std::min(bboxmin.y, pts[i].y));
		bboxmax.x = std::min(w, std::max(bboxmax.x, pts[i].x));
		bboxmax.y = std::min(h, std::max(bboxmax.y, pts[i].y));
	}
	for (int i = bboxmin.x; i <= bboxmax.x; i++)//inside the box
	{
		for (int j = bboxmin.y; j < bboxmax.y; j++)
		{
			Vec3f P((float)i + 0.5f, (float)j + 0.5f, 0.0f);//find the center of pixels
			Vec3f bar = barycentric(pts, P);
			if (bar.x < 0 || bar.y < 0 || bar.z < 0)continue;//out side the triangle

			float z = pts[0].z * bar.x + pts[1].z * bar.y + pts[2].z * bar.z;//z barycentric interpolation
			int idx = i + j * w;//1D index for zbuffer

			if (zbuffer[idx]<z)//depth test
			{
				TGAColor color;
				bool discard = shader.fragment(bar, color);//get color
				if (!discard) {
					zbuffer[idx] = z;
					image.set(i, j, color);
				}
			}

		}
	}
}
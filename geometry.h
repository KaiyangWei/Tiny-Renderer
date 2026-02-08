//in this file,i creat vectors of 2,3,4 dimentions,and 4X4 matrix.Meanwhile,add+-*/and another common function,to make another code's compute more likely to be mathematical papers
#ifndef GEOMETRY_
#define GEOMETRY_
#include<cmath>
struct Matrix;//When I first compiled the code,there is a wrong that v2m()can't work completely because of the position of definition of Matrix
struct Vec2f
{
	float x, y;
	Vec2f() :x(0), y(0) {};
	Vec2f(float _x, float _y) :x(_x),y(_y) {};
	//Function overloading
	inline Vec2f operator -(const Vec2f& v)const {
		return Vec2f(x - v.x, y - v.y);
	}
	inline Vec2f operator +(const Vec2f& v)const {
		return Vec2f(x + v.x, y + v.y);
	}
	inline Vec2f operator *(const float v)const {
		return Vec2f(x * v, y * v);
	}
	static float dot(const Vec2f& v1, const Vec2f& v2) {
		return v1.x * v2.x + v1.y * v2.y ;
	}
	//I have a question that can these functions be written in another form?
	inline float norm()const {
		return std::sqrt(x * x + y * y );
	}
	inline Vec2f normalize()const {
		float n = norm();
		if (n > 1e-5f)return Vec2f(x / n, y / n);
		return Vec2f(0, 0);
	}
	inline float operator [](int n)const {
		if (n == 0)return x;
		if (n == 1)return y;
		return y;
	}
};

struct Vec3f
{
	float x, y, z;
	Vec3f():x(0), y(0), z(0) {};
	Vec3f(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {};
	//same as Vec2f
	inline Vec3f operator -(const Vec3f& v)const {
		return Vec3f(x - v.x, y - v.y, z - v.z);
	}
	inline Vec3f operator +(const Vec3f& v)const {
		return Vec3f(x + v.x, y + v.y, z + v.z);
	}
	inline Vec3f operator *(const float v)const {
		return Vec3f(x * v, y * v, z * v);
	}
	static float dot(const Vec3f& v1, const Vec3f& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	inline Vec3f operator ^(const Vec3f& v)const {
		return Vec3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	//same as Vec2f
	inline float norm()const {
		return std::sqrt(x * x + y * y + z * z);
	}
	inline Vec3f normalize()const {
		float n = norm();
		if (n > 1e-5f)return Vec3f(x / n, y / n, z / n);
		return Vec3f(0, 0, 0);
	}
	inline float operator [](int n)const {
		if (n == 0)return x;
		if (n == 1)return y;
		return z;
	}
	Matrix v2m() const;
};

struct Vec4f
{
	float x, y, z, w;
	Vec4f():x(0), y(0), z(0),w(1) {};
	Vec4f(float _x, float _y, float _z, float _w) :x(_x), y(_y), z(_z), w(_w) {};
	//same as Vec2f
	inline Vec4f operator -(const Vec4f& v)const {
		return Vec4f(x - v.x, y - v.y, z - v.z, w - v.w);
	}
	inline Vec4f operator +(const Vec4f& v)const {
		return Vec4f(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	inline Vec4f operator *(const float v)const {
		return Vec4f(x * v, y * v, z * v, w * v);
	}
	static float dot(const Vec4f& v1, const Vec4f& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	inline Vec4f operator ^(const Vec4f& v)const {
		return Vec4f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x,1);
	}
	//same as Vec2f
	inline float norm()const {
		return std::sqrt(x * x + y * y + z * z);
	}
	inline Vec4f normalize()const {
		float n = norm();
		if (n > 1e-5f)return Vec4f(x / n, y / n, z / n, 1);
		return Vec4f(0, 0, 0, 1);
	}
	inline float operator [](int n)const {
		if (n == 0)return x;
		if (n == 1)return y;
		return z;
	}
};

struct Matrix
{
	float m[4][4];
	//initialize matrix to be an identity matrix
	Matrix() {
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (j == i)m[i][j] = 1;
				else m[i][j] = 0;
			}
		}
	} 
	inline float* operator[](int i) { return m[i]; }
	inline const float* operator[](int i)const { return m[i]; }

	inline Matrix operator*(const Matrix& o)const {
		Matrix r;
		for (int i = 0; i < 4; i++)
		{
			for (int j=0; j < 4; j++)
			{
				r.m[i][j] = 0.0f;
				for (int k = 0; k < 4; k++)
				{
					r.m[i][j] += m[i][k] * o.m[k][j];
				}
			}
		}
		return r;
	}

	inline Vec4f operator*(const Vec4f& v) const {
		return Vec4f(
			m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
			m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
			m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
			m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
		);
	}

	inline Matrix transpose() const {
		Matrix r;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				r.m[i][j] = m[j][i];
			}
		}
		return r;
	}

	//m2v and v2m are now being abandoned,as for the future,I don't know
	inline Vec3f m2v() const {
		float w = m[3][3];
		if (std::abs(w) < 1e-5f) return Vec3f(m[0][3], m[1][3], m[2][3]); //prevent divided by 0
		return Vec3f(m[0][3] / w, m[1][3] / w, m[2][3] / w);
	}
};
inline Matrix Vec3f::v2m() const {
	Matrix res;
	res[0][3] = x;
	res[1][3] = y;
	res[2][3] = z;
	res[3][3] = 1.f;
	return res;
}



#endif // !GEOMETRY_

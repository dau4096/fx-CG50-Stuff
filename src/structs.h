#ifndef STRUCTS_H
#define STRUCTS_H


constexpr float EPSILON = 1e-4f;
constexpr float INF = 0xFFFFFF;
static inline float absV(float value) {return (value<0) ? -value : value;}
static inline bool nearlyEqual(float a, float b) {
    return absV(a - b) < EPSILON;
}


struct Vec2 {
	float x, y;

	constexpr Vec2() : x(0.0f), y(0.0f) {}
	constexpr Vec2(float x, float y)
		: x(x), y(y) {}


	float operator[](int index) const {
		if (index == 0) {return x;}
		if (index == 1) {return y;}
		return INF;
	}

	float& operator[](int index) {
		if (index == 0) {return x;}
		if (index == 1) {return y;}
	}

	Vec2 operator+(const Vec2& other) {
		return {x + other.x, y + other.y};
	}

	Vec2 operator-(const Vec2& other) {
		return {x - other.x, y - other.y};
	}

	Vec2 operator*(float scalar) {
		return {x * scalar, y * scalar};
	}

	Vec2 operator/(float scalar) {
		return {x / scalar, y / scalar};
	}

	bool operator==(const Vec2& other) {
		return nearlyEqual(x, other.x) && nearlyEqual(y, other.y);
	}

	float length() {
		return x*x + y*y;
	}

	Vec2 normalise() {
		Vec2 newVec = Vec2(x,y);
		float len = newVec.length();
		if (nearlyEqual(len, 0.0f)) {return Vec2();}
		return newVec / len;
	}

	Vec2 abs() {
		return Vec2(absV(x), absV(y));
	}
};

struct Vec3 {
	float x, y, z;

	constexpr Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	constexpr Vec3(float x, float y, float z)
		: x(x), y(y), z(z) {}


	float& operator[](int index) {
		if (index == 0) {return x;}
		if (index == 1) {return y;}
		if (index == 2) {return z;}
	}

	float operator[](int index) const {
		if (index == 0) {return x;}
		if (index == 1) {return y;}
		if (index == 2) {return z;}
		return INF;
	}

	Vec3 operator+(const Vec3& other) {
		return {x + other.x, y + other.y, z + other.z};
	}

	Vec3 operator-(const Vec3& other) {
		return {x - other.x, y - other.y, z - other.z};
	}

	Vec3 operator*(float scalar) {
		return {x * scalar, y * scalar, z * scalar};
	}

	Vec3 operator/(float scalar) {
		return {x / scalar, y / scalar, z / scalar};
	}

	bool operator==(const Vec3& other) {
		return nearlyEqual(x, other.x) && nearlyEqual(y, other.y) && nearlyEqual(z, other.z);
	}

	float length() {
		return x*x + y*y + z*z;
	}

	Vec3 normalise() {
		Vec3 newVec = Vec3(x,y,z);
		float len = newVec.length();
		if (nearlyEqual(len, 0.0f)) {return Vec3();}
		return newVec / len;
	}

	Vec3 abs() {
		return Vec3(absV(x), absV(y), absV(z));
	}
};

struct Mat4 {
	float data[16];

	Mat4(
		float a0=0.0f, float a1=0.0f, float a2=0.0f, float a3=0.0f,
		float b0=0.0f, float b1=0.0f, float b2=0.0f, float b3=0.0f,
		float c0=0.0f, float c1=0.0f, float c2=0.0f, float c3=0.0f,
		float d0=0.0f, float d1=0.0f, float d2=0.0f, float d3=0.0f
	) {
		data[ 0] = a0; data[ 4] = a1; data[ 8] = a2; data[12] = a3;
		data[ 1] = b0; data[ 5] = b1; data[ 9] = b2; data[13] = b3;
		data[ 2] = c0; data[ 6] = c1; data[10] = c2; data[14] = c3;
		data[ 3] = d0; data[ 7] = d1; data[11] = d2; data[15] = d3;
	}

	float& operator[](int index) {
		return data[index];
	}

	float operator[](int index) const {
		return data[index];
	}

	Mat4 operator*(const Mat4& other) const {
		Mat4 result;
		for (int c=0; c<4; c++) {
			for (int r=0; r<4; r++) {
				for (int k=0; k<4; k++) {
					result[c*4+r] = result[c*4+r] + data[k*4+r] * other[c*4+k];
				}
			}
		}
		return result;
	}
};

static inline Mat4 mat4Identity() {
	return Mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}


struct Camera {
	Vec3 position;
	Vec2 angle;
	float fov, near, far;

	Camera() : position(), angle(), fov(), near(), far() {}

	Camera(Vec3 pos, Vec2 angle, float fov, float near, float far) 
		: position(pos), angle(angle),
		  fov(fov), near(near), far(far) {}
};


struct TriScreen {
	Vec2 vertices[3];
	Vec2 edges[3];
	unsigned short colour;
	bool valid;

	TriScreen() : vertices(), edges(), colour(), valid(false) {}

	TriScreen(Vec2 theseVertices[3], unsigned short colour)
		: vertices(), edges(), colour(colour), valid(true) {
		  	vertices[0] = theseVertices[0];
		  	vertices[1] = theseVertices[1];
		  	vertices[2] = theseVertices[2];

		  	edges[0] = vertices[1]-vertices[0];
		  	edges[1] = vertices[2]-vertices[1];
			edges[2] = vertices[0]-vertices[2];		  	
		  }
};

/* //Later.
struct Vertex {
	Vec3 position;
	Vec3 colour;

	Vertex() : position(), colour() {}

	Vertex(Vec3 position, Vec3 colour)
		: position(position),
		  colour(colour) {}
};
*/


#endif
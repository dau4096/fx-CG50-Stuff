#ifndef STRUCTS_H
#define STRUCTS_H


#define EPSILON 1e-4f
#define INF 0xFFFFFF
namespace maths {
	inline float min(float A, float B) {return (A<B) ? A : B;}
	inline float max(float A, float B) {return (A>B) ? A : B;}
	inline float clamp(float value, float minV, float maxV) {return (value < maxV) ? ((value > minV) ? value : minV) : maxV;}
	inline float abs(float value) {return (value<0) ? -value : value;}
	inline float floor(float value) {return static_cast<float>((value >= 0) ? static_cast<int>(value) : static_cast<int>(value) - (value != static_cast<int>(value)));}
	inline float round(float value) {return floor(value + 0.5f);}
	inline float ceil(float value) {return static_cast<float>((value <= 0) ? static_cast<int>(value) : static_cast<int>(value) + (value != static_cast<int>(value)));}
	inline float fract(float value) {return (value>0) ? (value - floor(value)) : (ceil(value) - value);}
	inline bool nearlyEqual(float a, float b) {return abs(a - b) < EPSILON;}
	float sqrtApprox(float value) {//Uses Newton-Raphson for an estimate of the square root.
		#define EPSILON_LOCAL 1e-2f /* 1-hundreths of precision. */
		#define MAX_SQRT_ITERATIONS 25u
		
		float guess = value * 0.5f;  //x0 = x/2
		unsigned int iterations = 0u;

		//Iterate until the guess is sufficiently accurate
		while (true) {
			float newGuess = 0.5f * (guess + value / guess);  //Update guess using Newton's method
			iterations++;
			if (abs(newGuess - guess) < EPSILON_LOCAL || iterations >= MAX_SQRT_ITERATIONS) {
				break;  //Stop when the guess is sufficiently accurate
			}
			guess = newGuess;
		}

		return guess;
	}
}
using namespace maths;
float tmp = INF;


struct Vec2 {
	float x, y;

	constexpr Vec2() : x(0.0f), y(0.0f) {}
	constexpr Vec2(float x, float y)
		: x(x), y(y) {}
	Vec2(const struct Vec3& v);
	Vec2(const struct Vec4& v);


	float operator[](int index) const {
		if (index == 0) {return x;}
		if (index == 1) {return y;}
		return INF;
	}

	float& operator[](int index) {
		if (index == 0) {return x;}
		if (index == 1) {return y;}
		return tmp;
	}

	Vec2 operator+(Vec2 other) {
		return {x + other.x, y + other.y};
	}

	void operator+=(Vec2 other) {
		x += other.x;
		y += other.y;
	}

	Vec2 operator-(Vec2 other) {
		return {x-other.x, y-other.y};
	}

	void operator-=(Vec2 other) {
		x -= other.x;
		y -= other.y;
	}

	Vec2 operator*(float scalar) {
		return {x*scalar, y*scalar};
	}

	void operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
	}

	Vec2 operator/(float scalar) {
		return {x/scalar, y/scalar};
	}

	void operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
	}

	bool operator==(const Vec2 other) {
		return nearlyEqual(x, other.x) && nearlyEqual(y, other.y);
	}

	float dot(Vec2 other) {
		return x*x + y*y;
	}

	float length() {
		return sqrtApprox(this->dot(*this));
	}

	float determinant(Vec2 other) {
		return (x*other.y) - (y*other.x);
	}

	Vec2 normalise() {
		Vec2 newVec = Vec2(x,y);
		float len = newVec.length();
		if (nearlyEqual(len, 0.0f)) {return Vec2();}
		return newVec / len;
	}

	Vec2 abs() {
		return Vec2(maths::abs(x), maths::abs(y));
	}
};

struct Vec3 {
	float x, y, z;

	constexpr Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
	constexpr Vec3(float x, float y, float z)
		: x(x), y(y), z(z) {}
	constexpr Vec3(Vec2 other, float z=0.0f)
		: x(other.x), y(other.y), z(z) {}
	Vec3(const struct Vec4& v);

	float operator[](int index) const {
		if (index == 0) {return x;}
		if (index == 1) {return y;}
		if (index == 2) {return z;}
		return INF;
	}


	float& operator[](int index) {
		if (index == 0) {return x;}
		if (index == 1) {return y;}
		if (index == 2) {return z;}
		return tmp;
	}

	Vec3 operator+(Vec3 other) {
		return {x+other.x, y+other.y, z+other.z};
	}

	void operator+=(Vec3 other) {
		x += other.x;
		y += other.y;
		z += other.z;
	}

	Vec3 operator-(Vec3 other) {
		return {x-other.x, y-other.y, z-other.z};
	}

	void operator-=(Vec3 other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}

	Vec3 operator*(float scalar) {
		return {x*scalar, y*scalar, z*scalar};
	}

	void operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
	}

	Vec3 operator/(float scalar) {
		return {x/scalar, y/scalar, z/scalar};
	}

	void operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
	}

	bool operator==(Vec3 other) {
		return nearlyEqual(x, other.x) && nearlyEqual(y, other.y) && nearlyEqual(z, other.z);
	}

	float dot(Vec3 other) {
		return x*x + y*y + z*z;
	}

	float length() {
		return sqrtApprox(this->dot(*this));
	}

	Vec3 cross(Vec3 other) {
		return Vec3(
			(y*other.z)-(z*other.y),
			(z*other.x)-(x*other.z),
			(x*other.y)-(y*other.x)
		);
	}

	Vec3 normalise() {
		Vec3 newVec = Vec3(x,y,z);
		float len = newVec.length();
		if (nearlyEqual(len, 0.0f)) {return Vec3();}
		return newVec / len;
	}

	Vec3 abs() {
		return Vec3(maths::abs(x), maths::abs(y), maths::abs(z));
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

struct Vec4 {
	float x, y, z, w;

	constexpr Vec4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	constexpr Vec4(float x, float y, float z, float w)
		: x(x), y(y), z(z), w(w) {}
	constexpr Vec4(Vec2 other, float z=0.0f, float w=0.0f)
		: x(other.x), y(other.y), z(z), w(w) {}
	constexpr Vec4(Vec2 other, Vec2 otherAlt)
		: x(other.x), y(other.y), z(otherAlt.x), w(otherAlt.y) {}
	constexpr Vec4(Vec3 other, float w=0.0f)
		: x(other.x), y(other.y), z(other.z), w(w) {}

	float operator[](int index) const {
		if (index == 0) {return x;}
		if (index == 1) {return y;}
		if (index == 2) {return z;}
		if (index == 3) {return w;}
		return INF;
	}


	float& operator[](int index) {
		if (index == 0) {return x;}
		if (index == 1) {return y;}
		if (index == 2) {return z;}
		if (index == 3) {return w;}
		return tmp;
	}

	Vec4 operator+(Vec4 other) {
		return {x+other.x, y+other.y, z+other.z, w+other.w};
	}

	void operator+=(Vec4 other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
	}

	Vec4 operator-(Vec4 other) {
		return {x-other.x, y-other.y, z-other.z, w-other.w};
	}

	void operator-=(Vec4 other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
	}

	Vec4 operator*(float scalar) {
		return {x*scalar, y*scalar, z*scalar, w*scalar};
	}

	void operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
	}

	Vec4 operator*(Mat4& matrix) {
		return {
			(matrix[0]*x) + (matrix[4]*y) + (matrix[ 8]*z) + (matrix[12]*w),
			(matrix[1]*x) + (matrix[5]*y) + (matrix[ 9]*z) + (matrix[13]*w),
			(matrix[2]*x) + (matrix[6]*y) + (matrix[10]*z) + (matrix[14]*w),
			(matrix[3]*x) + (matrix[7]*y) + (matrix[11]*z) + (matrix[15]*w)
		};
	}

	Vec4 operator/(float scalar) {
		return {x/scalar, y/scalar, z/scalar, w/scalar};
	}

	void operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
	}

	bool operator==(const Vec4 other) {
		return nearlyEqual(x, other.x) && nearlyEqual(y, other.y) && nearlyEqual(z, other.z) && nearlyEqual(w, other.w);
	}

	float dot(Vec4 other) {
		return x*x + y*y + z*z + w*w;
	}

	float length() {
		return sqrtApprox(this->dot(*this));
	}

	Vec4 normalise() {
		Vec4 newVec = Vec4(x,y,z,w);
		float len = newVec.length();
		if (nearlyEqual(len, 0.0f)) {return Vec4();}
		return newVec / len;
	}

	Vec4 abs() {
		return Vec4(maths::abs(x), maths::abs(y), maths::abs(z), maths::abs(w));
	}
};
inline Vec2::Vec2(const Vec3& v) : x(v.x), y(v.y) {}
inline Vec2::Vec2(const Vec4& v) : x(v.x), y(v.y) {}

inline Vec3::Vec3(const Vec4& v) : x(v.x), y(v.y), z(v.z) {}


struct Camera {
	Vec3 position;
	Vec2 angle;
	float fov, near, far;

	Camera() : position(), angle(), fov(), near(), far() {}

	Camera(Vec3 pos, Vec2 angle, float fov, float near, float far) 
		: position(pos), angle(angle),
		  fov(fov), near(near), far(far) {}
};



struct TriData {
	int startX, endX;
	unsigned int triIndex;
	float depth;

	TriData() : startX(), endX(), triIndex() {}

	TriData(int sX, int eX, unsigned int tIdx, float d)
		: startX(sX), endX(eX), triIndex(tIdx), depth(d) {}
};

static inline bool compareTriData(TriData* a, TriData* b) {
	return a->depth < b->depth;
}


struct Span {
	Vec2 start;
	unsigned int length;
	int triIndex;

	inline void _clampSpanValues(int X, int Y, int len) {
		start = Vec2(
			maths::clamp(X, 0, static_cast<int>(LCD_WIDTH_PX-1)),
			maths::clamp(Y, 0, static_cast<int>(LCD_HEIGHT_PX-1))
		);
		int endX = maths::clamp(X + len, 0, static_cast<int>(LCD_WIDTH_PX));
		length = static_cast<unsigned int>(maths::max(0, endX - start.x));
	}

	Span() : start(), length(0), triIndex(0) {}

	Span(int X, int Y, int len, int tIdx)
		: triIndex(tIdx) {
			_clampSpanValues(X, Y, len);
		}

	Span(float X, int Y, int len, int tIdx)
		: triIndex(tIdx) {
			_clampSpanValues(int(maths::round(X)), Y, len);
		}

	Span(TriData& thisTri, unsigned int yScan)
		: triIndex(thisTri.triIndex) {
			_clampSpanValues(thisTri.startX, yScan, thisTri.endX - thisTri.startX);
	}
};


namespace maths {
	//Vector struct operations
	//vec2;
	inline Vec2 min(Vec2 A, Vec2 B) {return Vec2(maths::min(A.x, B.x), maths::min(A.y, B.y));}
	inline Vec2 max(Vec2 A, Vec2 B) {return Vec2(maths::max(A.x, B.x), maths::max(A.y, B.y));}
	inline Vec2 round(Vec2 A) {return Vec2(maths::round(A.x), maths::round(A.y));}

	//vec3;
	inline Vec3 min(Vec3 A, Vec3 B) {return Vec3(maths::min(A.x, B.x), maths::min(A.y, B.y), maths::min(A.z, B.z));}
	inline Vec3 max(Vec3 A, Vec3 B) {return Vec3(maths::max(A.x, B.x), maths::max(A.y, B.y), maths::max(A.z, B.z));}
	inline Vec3 round(Vec3 A) {return Vec3(maths::round(A.x), maths::round(A.y), maths::round(A.z));}
}



inline Vec3 reformatVec3(Vec3 in) {
	return maths::round(in);
}

inline Vec3 findLowest(Vec3 a, Vec3 b) {
	if (a.y > b.y) {return reformatVec3(b);}
	return reformatVec3(a);
}
inline Vec3 findHighest(Vec3 a, Vec3 b) {
	if (a.y < b.y) {return reformatVec3(b);}
	return reformatVec3(a);
}

struct Edge {
	Vec2 start, end;
	float sZ, eZ; //Z Values for ends.
	float dx, currentX, dz, currentZ;
	int triIndex;
	bool isLeftEdge;

	Edge() : start(), end(), dx(), currentX(), triIndex(), isLeftEdge() {}

	Edge(Vec3 s, Vec3 e, int tIdx, bool isLeft)
		: triIndex(tIdx), sZ(s.z), eZ(e.z),
		  currentZ(sZ), isLeftEdge(isLeft) {
		  	Vec3 low = findLowest(s, e);
		  	Vec3 high = findHighest(s, e);

		  	start = Vec2(low.x, low.y);
		  	end = Vec2(high.x, high.y);
		  	currentX = high.x;

			Vec3 delta = high - low;
			dx = (abs(delta.y) >= 1) ? (delta.x / delta.y) : 0.0f;
			dz = (abs(delta.y) >= 1) ? (delta.z / delta.y) : 0.0f;
		}

	void calculateYScanValues(unsigned int yScan) {
		float dy = static_cast<float>(yScan - start.y);
		currentX = floor(start.x + (dx * dy) + 0.5f);
		currentZ = sZ + (dz * dy);
	}
};


#endif
#pragma once
#include <math.h>

template <typename T>
class Vector3
{
	//******member*******
private :
	T x;
	T y;
	T z;

public:
	//******constructor******
	Vector3(void)
	{
		x = (T)0;
		y = (T)0;
		z = (T)0;
	}
	Vector3(Vector3 const& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	Vector3(T const& x, T const& y, T const& z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
public:
	//Get/Set Methods
	void SetX(T v) { this->x = v; }
	void y(T v) { this->y = v; }
	void z(T v) { this->z = v; }
	T x() { return this->x; }
	T y() { return this->y; }
	T z() { return this->z; }
	void Set(T x, T y, T z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
public:
	//Utility Methods...
	void Clear() {
		this->x = (T)0;
		this->y = (T)0;
		this->z = (T)0;
	}
	void Print(void) {
		printf("R3 Vector : (%f, %f, %f)\n", x, y, z);
	}

	void Normalize(void) {
		T norm = GetNorm();
		x = x / norm;
		y = y / norm;
		z = z / norm;
	}
	T GetNorm() {
		return (T)sqrt(x * x + y * y + z * z);
	}
	T Length(void) {
		return GetNorm();
	}

public:
	//Productions...
	T Dot(Vector3<T>& v) {
		return (T)(x * v.x() + y * v.y() + z * v.z());
	}
	Vector3<T> Cross(Vec3<T>& v) {
		Vec3<T> vector;
		vector.x(y * v.z() - (z * v.y()));
		vector.y(z * v.x() - (x * v.z()));
		vector.z(x * v.y() - (y * v.x()));
		return vector;
	}
	T GetAngle(Vector3<T>& v)
	{
		T sin, cos;
		cos = Dot(v);				// ||u|| * ||v|| * cos(theta)
		sin = (Cross(v)).Length();	// ||u|| * ||v|| * sin(theta)
		T angle = (T)atan2(sin, cos);  //tan = sin/cos
		return angle;
	}
	Vector3<T> GetOrtho()
	{
		if (x != 0) 	{
			return Vector3(-y, x, 0);
		}
		else {
			return Vector3(1, 0, 0);
		}
	}
	//Operator Overloads
public:
	bool operator==(Vector3 const& v) const {
		return ((x == v.x()) && (y == v.y()) && (z == v.z()))
	}
	bool operator!=(Vector3 const& v) const {
		return !((*this) == v)
	}
	Vector3<T>& operator=(Vector3 const& v) {
		x = v.x();
		y = v.y();
		z = v.z();
		return *this;
	}
	Vector3<T>& operator+=(Vector3 const& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
};
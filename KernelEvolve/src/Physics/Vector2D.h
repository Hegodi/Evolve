#pragma once
#include <math.h>

#define EPSILON 0.0000001

template<typename T>
class Vector2D {
public:
	Vector2D(T ix, T iy) : x(ix), y(iy) {}
	Vector2D(T ia) : x(ia), y(ia) {}
	Vector2D() : x(0), y(0) {} ;


	//operators
	Vector2D operator+(const Vector2D& right) const
	{
		Vector2D<T> temp(*this);
		temp += right;
		return temp;
	}

	Vector2D operator-(const Vector2D& right) const
	{
		Vector2D<T> temp(*this);
		temp -= right;
		return temp;
	}

	Vector2D operator*(const Vector2D& right) const
	{
		Vector2D<T> temp(*this);
		temp *= right;
		return temp;
	}

	Vector2D operator*(const T& right) const
	{
		Vector2D<T> temp(*this);
		temp *= right;
		return temp;
	}

	Vector2D operator/(const T& right) const
	{
		Vector2D<T> temp(*this);
		temp /= right;
		return temp;
	}

	Vector2D& operator+=(const Vector2D& right)
	{
		x += right.x;
		y += right.y;
		return *this;
	}

	Vector2D& operator-=(const Vector2D& right)
	{
		x -= right.x;
		y -= right.y;
		return *this;

	}

	Vector2D& operator*=(const Vector2D& right)
	{
		x *= right.x;
		y *= right.y;
		return *this;
	}

	Vector2D& operator*=(const T& right)
	{
		x *= right;
		y *= right;
		return *this;
	}

	Vector2D& operator/=(const T& right)
	{
		x /= right;
		y /= right;
		return *this;
	}

	T lengthSqr() const
	{
		return x * x + y * y;
	}

	T length() const
	{
		return sqrt(lengthSqr());
	}

	void normalize()
	{
		T len = length();
		x /= len;
		y /= len;
	}

	void invert()
	{
		x *= -1;
		y *= -1;
	}

	Vector2D perpendicular()
	{
		return Vector2D<T>(-y, x);
	}

	static T dotProduct(const Vector2D& left, const Vector2D& right)
	{
		return left.x * right.x + left.y * right.y;
	}

	static Vector2D& Lerp(const Vector2D& source, const Vector2D& target, float factor)
	{
		Vector2D<T> ret;
		ret.x = source.x * (1.0f - factor) + target.x * factor;
		ret.y = source.y * (1.0f - factor) + target.y * factor;
		return ret;
	}

public:
	T x;
	T y;
};

typedef Vector2D<float> Vec2f;

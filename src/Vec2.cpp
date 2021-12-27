#include "Vec2.h"

Vec2::Vec2()
	: x(0), y(0)
{
}

Vec2::Vec2(float xin, float yin)
	: x(xin), y(yin)
{
}

float Vec2::dist(const Vec2& rhs) const
{
	float dist = sqrtf((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
	return dist;
}

Vec2 Vec2::operator+(const Vec2& rhs) const
{
	Vec2 ret = Vec2(x + rhs.x, y + rhs.y);
	return ret;
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
	Vec2 ret = Vec2(x - rhs.x, y - rhs.y);
	return ret;
}

Vec2 Vec2::operator/(const Vec2& rhs) const
{
	Vec2 ret = Vec2(x / rhs.x, y / rhs.y);
	return ret;
}

Vec2 Vec2::operator/(const float& rhs) const
{
	Vec2 ret = Vec2(x / rhs, y / rhs);
	return ret;
}

Vec2 Vec2::operator*(const Vec2& rhs) const
{
	Vec2 ret = Vec2(x * rhs.x, y * rhs.y);
	return ret;
}

void Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator-=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator*=(const Vec2& rhs)
{
	x *= rhs.x;
	y *= rhs.y;
}

void Vec2::operator/=(const Vec2& rhs)
{
	x /= rhs.x;
	y /= rhs.y;
}

bool Vec2::operator==(const Vec2& rhs) const
{
	return ((x == rhs.x) && (y == rhs.y));
}

bool Vec2::operator!=(const Vec2& rhs) const
{
	return ((x != rhs.x) && (y != rhs.y));
}

std::ostream& operator<<(std::ostream& os, const Vec2& vec)
{
	os << vec.x << ", " << vec.y;
	return os;
}

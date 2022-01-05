#pragma once

#include "Common.h"

class Vec2
{
public:
	Vec2();
	Vec2(float xin, float yin);

	float dist(const Vec2& rhs) const;

	Vec2 operator + (const Vec2& rhs) const;
	Vec2 operator - (const Vec2& rhs) const;
	Vec2 operator / (const Vec2& rhs) const;
	Vec2 operator / (const float& rhs) const;
	Vec2 operator * (const Vec2& rhs) const;

	void operator += (const Vec2& rhs);
	void operator -= (const Vec2& rhs);
	void operator *= (const Vec2& rhs);
	void operator /= (const Vec2& rhs);

	bool operator == (const Vec2& rhs) const;
	bool operator != (const Vec2& rhs) const;

	inline static float perpDot(const Vec2& a, const Vec2& b) { return (a.y * b.x) - (a.x * b.y); };
	inline static float dot(const Vec2& a, const Vec2& b) { return (a.x * b.x) + (a.y * b.y); };
	static Vec2 rotate(const Vec2& vec, float rad);

	friend std::ostream& operator<<(std::ostream& os, const Vec2& vec);

	double x;
	double y;

private:
};


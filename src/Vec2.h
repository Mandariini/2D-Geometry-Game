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

	friend std::ostream& operator<<(std::ostream& os, const Vec2& vec);

	double x;
	double y;

private:
};


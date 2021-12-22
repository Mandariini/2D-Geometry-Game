#pragma once
class Vec2
{
public:
	Vec2();
	Vec2(float xin, float yin);

	float dist(const Vec2& rhs) const;

	Vec2 operator + (const Vec2& rhs) const;
	Vec2 operator - (const Vec2& rhs) const;
	Vec2 operator / (const Vec2& rhs) const;
	Vec2 operator * (const Vec2& rhs) const;

	void operator += (const Vec2& rhs);
	void operator -= (const Vec2& rhs);
	void operator *= (const Vec2& rhs);
	void operator /= (const Vec2& rhs);

	bool operator == (const Vec2& rhs) const;
	bool operator != (const Vec2& rhs) const;

	double x;
	double y;

private:
};


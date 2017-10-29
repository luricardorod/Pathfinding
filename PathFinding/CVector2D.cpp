#include "stdafx.h"
#include "CVector2D.h"
#include <cmath>

CVector2D::CVector2D(const CVector2D &V)
{
	x = V.x;
	y = V.y;
}
CVector2D::CVector2D(float fx, float fy)
{
	x = fx;
	y = fy;
}

CVector2D::CVector2D()
{
}

CVector2D::~CVector2D()
{
}

CVector2D CVector2D::operator* (CVector2D  &A)
{
	return CVector2D(A.x*x, A.y*y);
}

CVector2D CVector2D::operator* (float s)
{
	return CVector2D(x*s, y*s);
}

CVector2D & CVector2D::operator*=(float s)
{
	x *= s;
	y *= s;
	return (*this);
}

CVector2D CVector2D::operator/ (float s)
{
	return (*this)*(1 / s);
}

CVector2D & CVector2D::operator/=(float s)
{
	x /= s;
	y /= s;
	return (*this);
}

CVector2D CVector2D::operator+(CVector2D& A)
{
	return CVector2D(A.x + x, A.y + y);
}

CVector2D & CVector2D::operator+=(CVector2D & A)
{
	x += A.x;
	y += A.y;
	return (*this);
}

CVector2D CVector2D::operator-(CVector2D& A)
{
	return CVector2D(x - A.x, y - A.y);
}

float CVector2D::Dot(CVector2D &A)
{
	return A.x*x + A.y*y;
}

float CVector2D::Truncate(float maxValue)
{
	if (Magnitude() > maxValue)
	{
		return maxValue;
	}
	else if (Magnitude() < 0) {
		return 0.0f;
	}
	return Magnitude();
}

float CVector2D::Magnitude()
{
	return sqrt(Dot((*this)));
}
CVector2D CVector2D::Normalize()
{
	float magnitude = Magnitude();
	if (magnitude)
	{
		return (*this) / magnitude;
	}
	return CVector2D(0, 0);
}


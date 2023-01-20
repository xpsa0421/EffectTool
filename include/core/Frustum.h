#pragma once
#include "CoreMath.h"
//#include "Box.h"

enum COREPOSITON
{
	_BACK = 0,
	_FRONT,
	_ONPLANE,
	_SPANNING,
};
struct Plane
{
	float a, b, c, d;
	Vector _planeVec;
	void Create(Vector v0, Vector v1, Vector v2) {
		Vector p1 = v1 - v0;
		Vector p2 = v2 - v0;
		Vector normal;
		normal = Vector::Cross(p1, p2);
		normal = Vector::Normalise(normal);
		a = normal.x;
		b = normal.y;
		c = normal.z;
		d = -(Vector::Dot(normal, v0));
		_planeVec.x = a;
		_planeVec.y = b;
		_planeVec.z = c;
	}
	void Create(Vector n, Vector v0)
	{
		n = Vector::Normalise(n);
		a = n.x;
		b = n.y;
		c = n.z;
		d = -(Vector::Dot(n, v0));
	}
};
class Frustum
{
	Matrix _viewMat;
	Matrix _projMat;
public:
	Vector _frustum[8];
	Plane _plane[6];
public:
	void CreateFrustum(Matrix* view, Matrix* proj);
	bool ClassifyPoint(Vector v);
	//todo : 박스 frustum 추가
	//COREPOSITION ClassifyBox(Box box);
};



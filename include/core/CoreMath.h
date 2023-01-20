#pragma once
#include <Windows.h>
#include <iostream>
#include <math.h>
#include <DirectXMath.h>
#define EPSILON 0.001f
#define PI 3.141592f
#define DegreeToRadian(x)  (x *(PI / 180.0f))
#define RadianToDegree(x) (x *(180.0f / PI))

struct Matrix;
struct Quaternion;

struct Float2
{
	union
	{
		struct {
			float x;
			float y;
		};
		float v[2];
	};
};

struct Float3
{
	union
	{
		struct {
			float x;
			float y;
			float z;
		};
		float v[3];
	};
};

struct Float4
{
	union
	{
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		float v[4];
	};
};

struct Vector2 : public DirectX::XMFLOAT2
{

public:
	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& v);
public:
	static float	Length(Vector2& v);
	static float	LengthSq(Vector2& v);
	static Vector2	Normalise(Vector2& v);
public:
	Vector2	operator +  (Vector2& v);
	Vector2	operator -  (Vector2& v);
	Vector2	operator *  (float scala);
	Vector2	operator /  (float scala);
	Vector2	operator += (Vector2& v);
	Vector2	operator -= (Vector2& v);
	Vector2	operator *= (float scala);
	bool	operator == (Vector2& v);
	bool	operator <= (Vector2& v);
	bool	operator >= (Vector2& v);
};

struct Vector : public DirectX::XMFLOAT3
{
public:
	Vector();
	Vector(float x, float y, float z);
	Vector(const Vector& v);
public:
	static float	Length(Vector& v);
	static float	LengthSq(Vector& v);
	static float	AngleBetweenVectors(Vector& v1, Vector& v2);
	static Vector	Cross(Vector& v1, Vector& v2);
	static float	Dot(Vector& v1, Vector& v2);
	static Vector	Normalise(Vector& v);
	static Vector	TransformCoord(Vector& v, Matrix& m);
	static Vector	Lerp(Vector& v1, Vector& v2, float t);
public:
	float	operator [] (int i);
	Vector	operator +  (Vector& v);
	Vector	operator -  (Vector& v);
	Vector	operator *  (float scala);
	Vector	operator /  (float scala);
	Vector	operator *= (float scala);
	Vector	operator += (Vector v);
	Vector	operator -= (Vector& v);
	bool	operator == (Vector& v);
	bool	operator <= (Vector& v);
	bool	operator >= (Vector& v);
public:
	Vector	operator *	(Matrix& m);
};

struct Vector4 : public DirectX::XMFLOAT4
{
public:
	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4(const Vector4& v);
public:
	static float	Length(Vector4& v);
	static float	LengthSq(Vector4& v);
	static Vector4	Normalise(Vector4& v);
public:
	Vector4	operator +  (Vector4& v);
	Vector4	operator -  (Vector4& v);
	Vector4	operator *  (Vector4& v);
	Vector4	operator *  (float scala);
	Vector4	operator /  (float scala);
	Vector4	operator *= (float scala);
	Vector4	operator += (Vector4& v);
	Vector4	operator -= (Vector4& v);
	bool	operator == (Vector4& v);
	bool	operator <= (Vector4& v);
	bool	operator >= (Vector4& v);
};


struct float3x3
{
	union
	{
		struct {
			float _11, _12, _13;
			float _21, _22, _23;
			float _31, _32, _33;
		};
		float m[3][3];
	};
};
struct float4x4
{
	union
	{
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};

struct Matrix : public DirectX::XMFLOAT4X4
{
public:
	Matrix();
	Matrix(float4x4 contents);
public:
	static Matrix	PerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ);
	static Matrix	OrthographicLH(float viewWidth, float viewHeight, float nearZ, float farZ);
	static Matrix	OrthographicOffCenterLH(float viewLeft, float viewRight,
		float viewBottom, float viewTop, float nearZ, float farZ);
public:
	static Matrix	Identity();
	static Matrix	Multiply(Matrix& mA, Matrix& mB);
	static Matrix	Inverse(Matrix& m);
	static Matrix	Transpose(Matrix& m);
	static Matrix	Translation(float offsetX, float offsetY, float offsetZ);
	static Matrix	Scaling(float sx, float sy, float sz);
	static Matrix	RotationX(float radians);
	static Matrix	RotationY(float radians);
	static Matrix	RotationZ(float radians);
	static Matrix	RotationQuaternion(Quaternion &q);
	static Matrix	AffineTransformation(
		CONST Vector scaling,
		CONST Vector* rotationCentre, CONST Quaternion* rotationQuat,
		CONST Vector* translation);
	static Matrix	RotationRollPitchYaw(float roll, float pitch, float yaw);
	static Matrix	LookAtLH(const Vector& pos, const Vector& target, const Vector& up);
	static bool		Decompose(Vector& scale, Quaternion& rotation, Vector& translation, Matrix& mat);

public:
	Matrix	operator* (Matrix& m);
};

struct Matrix3 : public DirectX::XMFLOAT3X3
{
public:
	Matrix3();
	Matrix3(float3x3 contents);
public:
	Matrix3	GetIdentity();
	void	Transpose();
	void	RotateX(float rad);
	void	RotateY(float rad);
	void	RotateZ(float rad);
	void	Scale(float x, float y);
	void	Translate(float x, float y);
public:
	Matrix3	operator* (Matrix3& m);
};

struct Quaternion : public DirectX::XMFLOAT4
{
public:
	Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(float scalar, Vector v);
	Quaternion(Vector4 v);
public:
	static void			RotationRollPitchYaw(Quaternion* q, float roll, float pitch, float yaw);
	static void			RotationMatrix(Quaternion& q, Matrix& m);
	static Quaternion	Slerp(Quaternion& q1, Quaternion& q2, float t);

};


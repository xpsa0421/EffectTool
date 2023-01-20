#include "CoreMath.h"

//-------------------------------------------------
// member functions of Vector2
Vector2::Vector2()
{
	x = y = 0.0f;
}

Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2::Vector2(const Vector2& v)
{
	this->x = v.x;
	this->y = v.y;
}

/*
* Computes the length of a 2D vector.
*/
float Vector2::Length(Vector2& v)
{
	DirectX::XMVECTOR V = DirectX::XMLoadFloat2(&v);
	DirectX::XMVECTOR RET = DirectX::XMVector2Length(V);
	return DirectX::XMVectorGetX(RET);
}

/*
* Computes the square of the length of a 2D vector.
*/
float Vector2::LengthSq(Vector2& v)
{
	DirectX::XMVECTOR V = DirectX::XMLoadFloat2(&v);
	DirectX::XMVECTOR RET = DirectX::XMVector2LengthSq(V);
	return DirectX::XMVectorGetX(RET);
}

/*
* Returns the normalized version of a 3D vector.
*/
Vector2 Vector2::Normalise(Vector2& v)
{
	DirectX::XMVECTOR V = DirectX::XMLoadFloat2(&v);
	V = DirectX::XMVector2Normalize(V);

	Vector2 ret;
	DirectX::XMStoreFloat2(&ret, V);
	return ret;
}

Vector2 Vector2::operator + (Vector2& v)
{
	return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator - (Vector2& v)
{
	return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator * (float scala)
{
	return Vector2(x * scala, y * scala);
}

Vector2 Vector2::operator / (float scala)
{
	return Vector2(x / scala, y / scala);
}

Vector2 Vector2::operator += (Vector2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2 Vector2::operator -= (Vector2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2 Vector2::operator *= (float scala)
{
	x *= scala;
	y *= scala;
	return *this;
}

bool Vector2::operator == (Vector2& v)
{
	if ((fabs(x - v.x) < EPSILON)
		&& (fabs(y - v.y) < EPSILON))
	{
		return true;
	}
	return false;
}

bool Vector2::operator <= (Vector2& v)
{
	if ((x <= v.x) && (y <= v.y)) return true;
	return false;
}

bool Vector2::operator >= (Vector2& v)
{
	if ((x >= v.x) && (y >= v.y)) return true;
	return false;
}


//-------------------------------------------------
// member functions of Vector
Vector::Vector()
{
	x = y = z = 0.0f;
}

Vector::Vector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::Vector(const Vector& v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}

/*
* Computes the length of a 3D vector.
*/
float Vector::Length(Vector& v)
{
	DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&v);
	DirectX::XMVECTOR RET = DirectX::XMVector3Length(V);
	return DirectX::XMVectorGetX(RET);
}

/*
* Computes the square of the length of a 3D vector.
*/
float Vector::LengthSq(Vector& v)
{
	DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&v);
	DirectX::XMVECTOR RET = DirectX::XMVector3LengthSq(V);
	return DirectX::XMVectorGetX(RET);
}

/*
* Computes the radian angle between two 3D vectors.
*/
float Vector::AngleBetweenVectors(Vector& v1, Vector& v2)
{
	DirectX::XMVECTOR V1 = DirectX::XMLoadFloat3(&v1);
	DirectX::XMVECTOR V2 = DirectX::XMLoadFloat3(&v2);
	DirectX::XMVECTOR RET = DirectX::XMVector3AngleBetweenVectors(V1, V2);
	return DirectX::XMVectorGetX(RET);
}

/*
* Computes the cross product between two 3D vectors.
*/
Vector Vector::Cross(Vector& v1, Vector& v2)
{
	Vector ret;
	DirectX::XMVECTOR V1 = DirectX::XMLoadFloat3(&v1);
	DirectX::XMVECTOR V2 = DirectX::XMLoadFloat3(&v2);
	DirectX::XMStoreFloat3(&ret, DirectX::XMVector3Cross(V1, V2));
	return ret;
}

/*
* Computes the dot product between 3D vectors.
*/
float Vector::Dot(Vector& v1, Vector& v2)
{
	DirectX::XMVECTOR V1	= DirectX::XMLoadFloat3(&v1);
	DirectX::XMVECTOR V2	= DirectX::XMLoadFloat3(&v2);
	DirectX::XMVECTOR RET	= DirectX::XMVector3Dot(V1, V2);
	return DirectX::XMVectorGetX(RET);
}

/*
* Returns the normalized version of a 3D vector.
*/
Vector Vector::Normalise(Vector& v)
{
	DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&v);
	V = DirectX::XMVector3Normalize(V);

	Vector ret;
	DirectX::XMStoreFloat3(&ret, V);
	return ret;
}

/*
* Transforms a 3D vector by a given matrix, projecting the result back into w = 1.
* 
* @param	v 3D vector
* @param	m transformation matrix
*/
Vector Vector::TransformCoord(Vector& v, Matrix& m)
{
	DirectX::XMVECTOR V = DirectX::XMLoadFloat3(&v);
	DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&m);
	DirectX::XMVECTOR RET = DirectX::XMVector3TransformCoord(V, M);

	Vector ret;
	DirectX::XMStoreFloat3(&ret, RET);
	return ret;
}

/*
 * Performs a linear interpolation between two vectors.
 *
 * @param	v1 the vector to interpolate from
 * @param	v2 the vector to interpolate to
 * @param	t the interpolation control factor
 * @return	the vector containing the interpolation
 */
Vector Vector::Lerp(Vector& v1, Vector& v2, float t)
{
	DirectX::XMVECTOR V1 = DirectX::XMLoadFloat3(&v1);
	DirectX::XMVECTOR V2 = DirectX::XMLoadFloat3(&v2);
	DirectX::XMVECTOR RET = DirectX::XMVectorLerp(V1, V2, t);
		
	Vector ret;
	DirectX::XMStoreFloat3(&ret, RET);
	return ret;
}

float Vector::operator [] (int i)
{
	if (i == 0) return x;
	if (i == 1) return y;
	if (i == 2) return z;
	return 0.0f;
}

Vector Vector::operator + (Vector& v)
{
	return Vector(x + v.x, y + v.y, z + v.z);
}

Vector Vector::operator - (Vector& v)
{
	return Vector(x - v.x, y - v.y, z - v.z);
}

Vector Vector::operator * (float scala)
{
	return Vector(x * scala, y * scala, z * scala);
}

Vector Vector::operator / (float scala)
{
	return Vector(x / scala, y / scala, z / scala);
}

Vector Vector::operator += (Vector v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector Vector::operator -= (Vector& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector Vector::operator *= (float scala)
{
	x *= scala;
	y *= scala;
	z *= scala;
	return *this;
}

bool Vector::operator == (Vector& v)
{
	if ((fabs(x - v.x) < EPSILON)
		&& (fabs(y - v.y) < EPSILON)
		&& (fabs(z - v.z) < EPSILON))
	{
		return true;
	}
	return false;
}

bool Vector::operator <= (Vector& v)
{
	if ((x <= v.x) && (y <= v.y) && (z <= v.z)) return true;
	return false;
}

bool Vector::operator >= (Vector& v)
{
	if ((x >= v.x) && (y >= v.y) && (z >= v.z)) return true;
	return false;
}

Vector Vector::operator * (Matrix& m)
{
	Vector v;
	v.x = x * m._11 + y * m._21 + z * m._31 + 1.0f * m._41;
	v.y = x * m._12 + y * m._22 + z * m._32 + 1.0f * m._42;
	v.z = x * m._13 + y * m._23 + z * m._33 + 1.0f * m._43;
	return v;
}

//-------------------------------------------------
// member functions of Vector4
Vector4::Vector4()
{
	x = y = z = w = 0.0f;
}

Vector4::Vector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4::Vector4(const Vector4& v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = v.w;
}

/*
* Computes the length of a 4D vector.
*/
float Vector4::Length(Vector4& v)
{
	DirectX::XMVECTOR V = DirectX::XMLoadFloat4(&v);
	DirectX::XMVECTOR RET = DirectX::XMVector4Length(V);
	return DirectX::XMVectorGetX(RET);
}

/*
* Computes the square of the length of a 4D vector.
*/
float Vector4::LengthSq(Vector4& v)
{
	DirectX::XMVECTOR V = DirectX::XMLoadFloat4(&v);
	DirectX::XMVECTOR RET = DirectX::XMVector4LengthSq(V);
	return DirectX::XMVectorGetX(RET);
}

/*
* Returns the normalized version of a 4D vector.
*/
Vector4 Vector4::Normalise(Vector4& v)
{
	DirectX::XMVECTOR V = DirectX::XMLoadFloat4(&v);
	V = DirectX::XMVector4Normalize(V);

	Vector4 ret;
	DirectX::XMStoreFloat4(&ret, V);
	return ret;
}

Vector4 Vector4::operator + (Vector4& v)
{
	return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vector4 Vector4::operator - (Vector4& v)
{
	return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

Vector4 Vector4::operator * (Vector4& v)
{
	return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

Vector4 Vector4::operator * (float scala)
{
	return Vector4(x * scala, y * scala, z * scala, w * scala);
}

Vector4 Vector4::operator / (float scala)
{
	return Vector4(x / scala, y / scala, z / scala, w / scala);
}

Vector4 Vector4::operator += (Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

Vector4 Vector4::operator -= (Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

Vector4 Vector4::operator *= (float scala)
{
	x *= scala;
	y *= scala;
	z *= scala;
	w *= scala;
	return *this;
}

bool Vector4::operator == (Vector4& v)
{
	if ((fabs(x - v.x) < EPSILON)
		&& (fabs(y - v.y) < EPSILON)
		&& (fabs(z - v.z) < EPSILON)
		&& (fabs(w - v.w) < EPSILON))
	{
		return true;
	}
	return false;
}

bool Vector4::operator <= (Vector4& v)
{
	if ((x <= v.x) && (y <= v.y) && (z <= v.z) && (w <= v.w)) return true;
	return false;
}

bool Vector4::operator >= (Vector4& v)
{
	if ((x >= v.x) && (y >= v.y) && (z >= v.z) && (w >= v.w)) return true;
	return false;
}


//-------------------------------------------------
// member functions of Matrix

Matrix::Matrix()
{
	_12 = _13 = _14 = 0.0f;
	_21 = _23 = _24 = 0.0f;
	_31 = _32 = _34 = 0.0f;
	_41 = _42 = _43 = 0.0f;
	_11 = _22 = _33 = _44 = 1.0f;
}

Matrix::Matrix(float4x4 contents)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			m[i][j] = contents.m[i][j];
		}
	}
}

Matrix Matrix::Inverse(Matrix& m)
{
	Matrix res;
	DirectX::FXMMATRIX M = DirectX::XMLoadFloat4x4(&m);
	DirectX::XMVECTOR det;
	DirectX::XMStoreFloat4x4(&res, DirectX::XMMatrixInverse(&det, M));
	return res;
}

Matrix Matrix::Transpose(Matrix& m)
{
	Matrix outM;
	DirectX::FXMMATRIX M = DirectX::XMLoadFloat4x4(&m);
	DirectX::XMStoreFloat4x4(&outM, DirectX::XMMatrixTranspose(M));
	return outM;
}

Matrix Matrix::Translation(float offsetX, float offsetY, float offsetZ)
{
	Matrix res;
	DirectX::XMStoreFloat4x4(&res, DirectX::XMMatrixTranslation(offsetX, offsetY, offsetZ));
	return res;
}

Matrix Matrix::operator* (Matrix& matrix)
{
	Matrix mat;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			mat.m[i][j] =
				m[i][0] * matrix.m[0][j] +
				m[i][1] * matrix.m[1][j] +
				m[i][2] * matrix.m[2][j] +
				m[i][3] * matrix.m[3][j];
		}
	}
	return mat;
}

Matrix Matrix::LookAtLH(const Vector& pos, const Vector& target, const Vector& up)
{
	DirectX::XMVECTOR POS = DirectX::XMLoadFloat3(&pos);
	DirectX::XMVECTOR TARGET = DirectX::XMLoadFloat3(&target);
	DirectX::XMVECTOR UP = DirectX::XMLoadFloat3(&up);

	Matrix ret;
	DirectX::XMStoreFloat4x4(&ret, DirectX::XMMatrixLookAtLH(POS, TARGET, UP));
	return ret;
}

bool Matrix::Decompose(Vector& scale, Quaternion& rotation, Vector& translation, Matrix& mat)
{
	DirectX::XMVECTOR S, R, T;
	DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&mat);
	if (DirectX::XMMatrixDecompose(&S, &R, &T, M))
	{
		DirectX::XMStoreFloat3(&scale, S);
		DirectX::XMStoreFloat4(&rotation, R);
		DirectX::XMStoreFloat3(&translation, T);
		DirectX::XMStoreFloat4x4(&mat, M);
		return true;
	}
	else return false;
}

/*
* Builds a left-handed perspective projection matrix based on a field of view.
*/
Matrix Matrix::PerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ)
{
	Matrix ret;
	DirectX::XMMATRIX RET = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&ret, RET);
	return ret;
}

Matrix Matrix::OrthographicLH(float viewWidth, float viewHeight, float nearZ, float farZ)
{
	Matrix ret;
	DirectX::XMMATRIX RET = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&ret, RET);
	return ret;
}

Matrix Matrix::OrthographicOffCenterLH(float viewLeft, float viewRight,
	float viewBottom, float viewTop, float nearZ, float farZ)
{
	Matrix ret;
	DirectX::XMMATRIX RET = DirectX::XMMatrixOrthographicOffCenterLH(
		viewLeft, viewRight, viewBottom, viewTop, nearZ, farZ);
	DirectX::XMStoreFloat4x4(&ret, RET);
	return ret;
}

/**
 * Builds the identity matrix.
 *
 * @return	the identity matrix
 */
Matrix Matrix::Identity()
{
	Matrix ret;
	DirectX::XMStoreFloat4x4(&ret, DirectX::XMMatrixIdentity());
	return ret;
}

/*
Builds and returns a matrix multiplication of (mA, mB)
*/
Matrix Matrix::Multiply(Matrix& mA, Matrix& mB)
{
	DirectX::XMMATRIX MA = DirectX::XMLoadFloat4x4(&mA);
	DirectX::XMMATRIX MB = DirectX::XMLoadFloat4x4(&mB);
	DirectX::XMMATRIX MOUT = DirectX::XMMatrixMultiply(MA, MB);

	Matrix outM;
	DirectX::XMStoreFloat4x4(&outM, MOUT);
	return outM;
}

/*
Builds and returns a matrix which scales by (sx, sy, sz)
*/
Matrix Matrix::Scaling(float sx, float sy, float sz)
{
	Matrix outM;
	DirectX::XMStoreFloat4x4(&outM, DirectX::XMMatrixScaling(sx, sy, sz));
	return outM;
}

Matrix Matrix::RotationX(float radians)
{
	Matrix outM;
	DirectX::XMStoreFloat4x4(&outM, DirectX::XMMatrixRotationX(radians));
	return outM;
}

Matrix Matrix::RotationY(float radians)
{
	Matrix outM;
	DirectX::XMStoreFloat4x4(&outM, DirectX::XMMatrixRotationY(radians));
	return outM;
}

Matrix Matrix::RotationZ(float radians)
{
	Matrix outM;
	DirectX::XMStoreFloat4x4(&outM, DirectX::XMMatrixRotationZ(radians));
	return outM;
}

/**
 * Builds a rotation matrix from a quaternion.
 *
 * @param	q the quaternion defining the rotation
 * @return	converted rotation matrix
 */
Matrix Matrix::RotationQuaternion(Quaternion& q)
{
	Matrix outM;
	DirectX::XMVECTOR Q = DirectX::XMLoadFloat4(&q);
	DirectX::XMMATRIX OUTM = DirectX::XMMatrixRotationQuaternion(Q);
	DirectX::XMStoreFloat4x4(&outM, OUTM);
	return outM;
}

/*
Returns the affine transformation matrix,
built from the scaling, rotation, and translation information
*/
Matrix Matrix::AffineTransformation(
	CONST Vector scaling,
	CONST Vector* rotationCentre, CONST Quaternion* rotationQuat,
	CONST Vector* translation)
{
	DirectX::XMVECTOR s, rc, r, t;

	s = DirectX::XMLoadFloat3(&scaling);
	if (rotationCentre == NULL)
		rc = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	else
		rc = DirectX::XMLoadFloat3(rotationCentre);
	r = DirectX::XMLoadFloat4(rotationQuat);
	t = DirectX::XMLoadFloat3(translation);

	Matrix outM;
	DirectX::XMMATRIX XMMat = DirectX::XMMatrixAffineTransformation(s, rc, r, t);
	DirectX::XMStoreFloat4x4(&outM, XMMat);

	return outM;
}

Matrix Matrix::RotationRollPitchYaw(float roll, float pitch, float yaw)
{
	Matrix outM;
	DirectX::XMStoreFloat4x4(&outM, DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll));
	return outM;
}



//-------------------------------------------------
// member functions of Matrix3

Matrix3::Matrix3()
{
	*this = GetIdentity();
}

Matrix3::Matrix3(float3x3 contents)
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			m[i][j] = contents.m[i][j];
		}
	}
}

Matrix3 Matrix3::GetIdentity()
{
	Matrix3 m;
	m._12 = m._13 = 0.0f;
	m._21 = m._23 = 0.0f;
	m._31 = m._32 = 0.0f;
	m._11 = m._22 = m._33 = 1.0f;
	return m;
}

void Matrix3::Transpose()
{
	Matrix3 m;
	m._11 = _11; m._12 = _21; m._13 = _31;
	m._21 = _12; m._22 = _22; m._23 = _32;
	m._31 = _13; m._32 = _23; m._33 = _33;
	*this = m;
}

void Matrix3::RotateX(float rad)
{
	float cosTheta = cos(rad);
	float sinTheta = sin(rad);
	_22 = cosTheta;	 _23 = sinTheta;
	_32 = -sinTheta; _33 = cosTheta;
}

void Matrix3::RotateY(float rad)
{
	float cosTheta = cos(rad);
	float sinTheta = sin(rad);
	_11 = cosTheta;	 _13 = -sinTheta;
	_31 = sinTheta;	 _33 = cosTheta;
}

void Matrix3::RotateZ(float rad)
{
	float cosTheta = cos(rad);
	float sinTheta = sin(rad);
	_11 = cosTheta;	 _12 = sinTheta;
	_21 = -sinTheta; _22 = cosTheta;
}

void Matrix3::Scale(float x, float y)
{
	_11 = x;
	_22 = y;
}

void Matrix3::Translate(float x, float y)
{
	_31 = x;
	_32 = y;
}

Matrix3 Matrix3::operator* (Matrix3& matrix)
{
	Matrix3 mat;
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			mat.m[i][j] =
				m[i][0] * matrix.m[0][j] +
				m[i][1] * matrix.m[1][j] +
				m[i][2] * matrix.m[2][j];
		}
	}
	return mat;
}

//-------------------------------------
// Quaternion
// ------------------------------------
// CONSTRUCTORS

Quaternion::Quaternion()
{
	x, y, z, w = 0.0f;
}

Quaternion::Quaternion(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Quaternion::Quaternion(float scalar, Vector v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = scalar;
}

Quaternion::Quaternion(Vector4 v)
{
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = v.w;
}

// OVERLOADED OPERATORS


// USEFUL FUNCTIONS

// create quaternion from roll, pitch, and yaw value
void Quaternion::RotationRollPitchYaw(Quaternion *q, float roll, float pitch, float yaw)
{
	DirectX::XMStoreFloat4(q, DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
}

/*
Creates quaternion from rotation matrix
*/
void Quaternion::RotationMatrix(Quaternion& q, Matrix& m)
{
	DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(&m);
	DirectX::XMStoreFloat4(&q, DirectX::XMQuaternionRotationMatrix(M));
}


/**
 * Interpolates between two unit quaternions, using spherical linear interpolation.
 *
 * @param	q1 the unit quaternion to interpolate from
 * @param	q2 the unit quaternion to interpolate to
 * @param	t the interpolation control factor
 * @return	interpolated quaternion 
 *		undefined if q1 and q2 are not unit quaternions
 */
Quaternion Quaternion::Slerp(Quaternion& q1, Quaternion& q2, float t)
{
	DirectX::XMVECTOR Q1 = DirectX::XMLoadFloat4(&q1);
	DirectX::XMVECTOR Q2 = DirectX::XMLoadFloat4(&q2);

	Quaternion ret;
	XMStoreFloat4(&ret, DirectX::XMQuaternionSlerp(Q1, Q2, t));
	return ret;
}
#include "Frustum.h"

void Frustum::CreateFrustum(Matrix* view, Matrix* proj)
{
	_viewMat = *view;
	_projMat = *proj;
	Matrix inViewProjMat = _viewMat * _projMat;
	inViewProjMat = Matrix::Inverse(inViewProjMat);

	_frustum[0] = Vector(-1.0f, 1.0f, 0.0f);
	_frustum[1] = Vector(1.0f, 1.0f, 0.0f);
	_frustum[2] = Vector(-1.0f, -1.0f, 0.0f);
	_frustum[3] = Vector(1.0f, -1.0f, 0.0f);

	_frustum[4] = Vector(-1.0f, 1.0f, 1.0f);
	_frustum[5] = Vector(1.0f, 1.0f, 1.0f);
	_frustum[6] = Vector(-1.0f, -1.0f, 1.0f);
	_frustum[7] = Vector(1.0f, -1.0f, 1.0f);

	for (int ver = 0; ver < 8; ver++) 
	{
		_frustum[ver] = Vector::TransformCoord(_frustum[ver], inViewProjMat);
	}

	_plane[0].Create(_frustum[0], _frustum[2], _frustum[1]);	//near
	_plane[1].Create(_frustum[4], _frustum[5], _frustum[7]);	//far
	_plane[2].Create(_frustum[4], _frustum[6], _frustum[2]);	//left
	_plane[3].Create(_frustum[1], _frustum[3], _frustum[7]);	//right
	_plane[4].Create(_frustum[0], _frustum[1], _frustum[5]);	//top
	_plane[5].Create(_frustum[6], _frustum[7], _frustum[2]);	//bottom
	
}

bool Frustum::ClassifyPoint(Vector v)
{
	for (int planeIndex = 0; planeIndex < 6; planeIndex++) 
	{
		float dis =
			_plane[planeIndex].a * v.x +
			_plane[planeIndex].b * v.y +
			_plane[planeIndex].c * v.z +
			_plane[planeIndex].d;
		if (dis < 0)
		{
			return false;
		}
	}
	return true;
}
// box 추가 후 수정 예정

//COREPOSITION Frustum::ClassifyBox(Box obb)
//{
//	float BoxToPlaneDis = 0.0f;
//	float Distance = 0.0f;
//	float DisX = 0.0f;
//	float DisY = 0.0f;
//	float DisZ = 0.0f;
//	H_Vector3 Dir;
//
//	X_POSITION Check = _FRONT;
//	for (int p = 0; p < 6; p++) {
//		Dir = obb._Axis[0] * obb._Extent[0];
//		DisX = fabs(D3DXVec3Dot(&_Plane[p]._PlaneVec, &Dir));
//		Dir = obb._Axis[1] * obb._Extent[1];
//		DisY = fabs(D3DXVec3Dot(&_Plane[p]._PlaneVec, &Dir));
//		Dir = obb._Axis[2] * obb._Extent[2];
//		DisZ = fabs(D3DXVec3Dot(&_Plane[p]._PlaneVec, &Dir));
//
//		Distance = DisX + DisY + DisZ;
//
//		BoxToPlaneDis = D3DXVec3Dot(&_Plane[p]._PlaneVec, &obb._Center) + _Plane[p].d;
//
//		if (BoxToPlaneDis > 0) {
//			if (BoxToPlaneDis < Distance) {
//				Check = _SPANNING;
//				break;
//			}
//		}
//		else if (BoxToPlaneDis < 0) {
//			Check = _BACK;
//			if (BoxToPlaneDis > -Distance) {
//				Check = _SPANNING;
//			}
//			break;
//		}
//
//	}
//	return Check;
//}

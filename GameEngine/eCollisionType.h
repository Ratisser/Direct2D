#pragma once

enum class eCollisionType
{
	// 2D�� 3D���� �浹�� �ȵǰ� ����̴ϴ�.
	Point2D,
	CirCle,
	Rect,
	OrientedRect,

	Point3D,
	Sphere3D,
	AABBBox3D,
	OBBBox3D,
	MAX,
};
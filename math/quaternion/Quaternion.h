#pragma once
#include "MatrixCalculation.h"

struct Quaternion
{
	float x;
	float y;
	float z;
	float w;
};

//積
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs);
//単位を返す
Quaternion IdentityQuaternion();
//共役を返す
Quaternion Conjugate(const Quaternion& quaternion);
//normを返す
float Norm(const Quaternion& quaternion);
//正規化したQuaternionを返す
Quaternion Normalize(const Quaternion& quaternion);
//逆Quaternionを返す
Quaternion Inverse(const Quaternion& quaternion);


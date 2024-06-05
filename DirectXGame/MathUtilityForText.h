#pragma once

#include <math.h>
#include "Matrix4x4.h"
#include "Vector3.h"

Matrix4x4 MekeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

//　代入演算子オーバーロード
//　Vector3の足算
Vector3& operator+=(Vector3& lhv, const Vector3& rhv);

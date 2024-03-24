#include "Rendering.h"

Matrix3x3 Rendering::Inverse(Matrix3x3 matrix) {

	Matrix3x3 result{};
	float cofactor = 1.0f / (matrix.m[0][0] * matrix.m[1][1] * matrix.m[2][2] +
		matrix.m[0][1] * matrix.m[1][2] * matrix.m[2][0] +
		matrix.m[0][2] * matrix.m[1][0] * matrix.m[2][1] -
		matrix.m[0][2] * matrix.m[1][1] * matrix.m[2][0] -
		matrix.m[0][1] * matrix.m[1][0] * matrix.m[2][2] -
		matrix.m[0][0] * matrix.m[1][2] * matrix.m[2][1]
		);
	result.m[0][0] = cofactor * (matrix.m[1][1] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][1]);
	result.m[0][1] = -(cofactor * (matrix.m[0][1] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][1]));
	result.m[0][2] = cofactor * (matrix.m[0][1] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][1]);
	result.m[1][0] = -(cofactor * (matrix.m[1][0] * matrix.m[2][2] - matrix.m[1][2] * matrix.m[2][0]));
	result.m[1][1] = cofactor * (matrix.m[0][0] * matrix.m[2][2] - matrix.m[0][2] * matrix.m[2][0]);
	result.m[1][2] = -(cofactor * (matrix.m[0][0] * matrix.m[1][2] - matrix.m[0][2] * matrix.m[1][0]));
	result.m[2][0] = cofactor * (matrix.m[1][0] * matrix.m[2][1] - matrix.m[1][1] * matrix.m[2][0]);
	result.m[2][1] = -(cofactor * (matrix.m[0][0] * matrix.m[2][1] - matrix.m[0][1] * matrix.m[2][0]));
	result.m[2][2] = cofactor * (matrix.m[0][0] * matrix.m[1][1] - matrix.m[0][1] * matrix.m[1][0]);

	return result;

}

Matrix3x3 Rendering::Multiply(Matrix3x3 matrix1, Matrix3x3 matrix2) {

	Matrix3x3 result = {};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			result.m[i][j] = 0;
			for (int k = 0; k < 3; k++) {
				result.m[i][j] += matrix1.m[i][k] * matrix2.m[k][j];
			}
		}
	}
	return result;

}

Matrix3x3 Rendering::MakeTranslateMatrix(Vector2 translate) {
	Matrix3x3 result{
		1,0,0,
		0,1,0,
		translate.x,translate.y,1
	};
	return  result;
}

Matrix3x3 Rendering::MakeRotateMatrix(float theta) {
	Matrix3x3 result = { cosf(theta),sinf(theta),0,
		-sinf(theta),cosf(theta),0,
		0,0,1 };

	return result;
}

Matrix3x3 Rendering::MakeScaleMatrix(Vector2 scale) {
	Matrix3x3 result{
		scale.x,0,0,
		0,scale.y,0,
		0,0,1,
	};
	return result;
}

Vector2 Rendering::Transform(Vector2 vector, Matrix3x3 matrix) {

	Vector2 result = {};
	result.x = { vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0] };
	result.y = { vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1] };
	float w  = vector.x *  matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;

	return result;

}

Matrix3x3 Rendering::MakeOrthographicMatrix(Point vertex) {

	Matrix3x3 result{
		2.0f / (vertex.right - vertex.left),0,0,
		0,2.0f / (vertex.top - vertex.bottom),0,
		(vertex.left + vertex.right) / (vertex.left - vertex.right),(vertex.top + vertex.bottom) / (vertex.bottom - vertex.top),1
	};
	return result;

}

Matrix3x3 Rendering::MakeViewportMatrix(float left, float top, float width, float height) {

	Matrix3x3 result{
		width / 2.0f,0,0,
		0,-height / 2.0f,0,
		left + width / 2.0f,top + height / 2.0f,1
	};
	return result;

}

Matrix3x3 Rendering::MakeAffineMatrix(Affine affine) {
	
	return Multiply(Multiply(MakeScaleMatrix(affine.scale), MakeRotateMatrix(affine.theta)), MakeTranslateMatrix(affine.translate));
}

Vertex Rendering::Transform(Vertex local, Matrix3x3 wvpVpMatrix) {
	Vertex result;
	result.leftTop     = Transform(local.leftTop,     wvpVpMatrix);
	result.leftBottom  = Transform(local.leftBottom,  wvpVpMatrix);
	result.rightTop    = Transform(local.rightTop,    wvpVpMatrix);
	result.rightBottom = Transform(local.rightBottom, wvpVpMatrix);
	return result;
}
#include "Camera.h"

Camera::Camera() {
	affine_ = {
	   {1.0f,1.0f},
		0,
	   {480,200},
	};
	vertex_ = {
		 -640,
		  360,
		  640,
		 -360,
	};
	local_ = {
		{
		 { -10000,0 },
		 {  10000,0 },
		},
		{
		 { 0, -10000},
		 { 0,  10000},
		},
	};

	speed_ = 5.0f;
	scale_ = 1.0f / 100.0f;
	theta_ = 1.0f / 100.0f;

	screen_ = {};

	leftTop_ = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	cameraMatrix_ = {};

	viewMatrix_ = {};

	orthoMatrix_ = {};

	viewportMatrix_ = {};

	vpVpMatrix_ = {};
}

/// <summary>
/// カメラのワールドマトリックスの作成
/// </summary>
void Camera::CameraMatrix() {
	cameraMatrix_ = MakeAffineMatrix(affine_);
}

/// <summary>
/// ビューマトリックスの作成
/// </summary>
void Camera::ViewMatrix() {
	viewMatrix_ = Inverse(cameraMatrix_);
}

/// <summary>
/// 正規化デバイスマトリックスの作成
/// </summary>
void Camera::OrthoMatrix() {
	orthoMatrix_ = MakeOrthographicMatrix(vertex_);
}

/// <summary>
/// ビューポートマトリックスの作成
/// </summary>
void Camera::ViewportMatrix() {
	viewportMatrix_ = MakeViewportMatrix(leftTop_.left, leftTop_.top, leftTop_.width, leftTop_.height);
}

/// <summary>
/// vs = vpVp
/// </summary>
void Camera::MakeVpVpMatrix() {
	/*ワールド座標系*/
	vpVpMatrix_ = Multiply(viewMatrix_,orthoMatrix_);
	/*ビュー座標系*/
	vpVpMatrix_ = Multiply(vpVpMatrix_,viewportMatrix_);
}
/// <summary>
/// ラインをローカルからスクリーンに変換
/// </summary>
void Camera::CameraTransform() {
	screen_.lateral.begin = Transform(local_.lateral.begin, vpVpMatrix_);
	screen_.lateral.end   = Transform(local_.lateral.end,   vpVpMatrix_);
	screen_.vert.begin    = Transform(local_.vert.begin,    vpVpMatrix_);
	screen_.vert.end      = Transform(local_.vert.end,      vpVpMatrix_);
}
/// <summary>
/// ラインの描画
/// </summary>
void Camera::Draw() {
	Novice::DrawLine((int)screen_.lateral.begin.x, (int)screen_.lateral.begin.y, (int)screen_.lateral.end.x, (int)screen_.lateral.end.y, RED);
	Novice::DrawLine((int)screen_.vert.begin.x,    (int)screen_.vert.begin.y,    (int)screen_.vert.end.x,    (int)screen_.vert.end.y,    GREEN);
}

void Camera::CameraScale(char* keys) {
	if (keys[DIK_E] && !keys[DIK_LSHIFT]) {
		if (affine_.scale.x > 0.3f || affine_.scale.y > 0.3f) {
			affine_.scale.x -= scale_;
			affine_.scale.y -= scale_;
		}
	}
	if (keys[DIK_Q] && !keys[DIK_LSHIFT]) {
		affine_.scale.x += scale_;
		affine_.scale.y += scale_;
	}
}

void Camera::CameraRotation(char* keys) {
	if (keys[DIK_C] && !keys[DIK_LSHIFT]) {
		affine_.theta -= theta_;
	}
	if (keys[DIK_Z] && !keys[DIK_LSHIFT]) {
		affine_.theta += theta_;
	}
}

void Camera::CameraTranslate(char* keys){
	if (keys[DIK_LEFT]) {
		affine_.translate.x += speed_;
	}
	if (keys[DIK_RIGHT]) {
		affine_.translate.x -= speed_;
	}
	if (keys[DIK_UP]) {
		affine_.translate.y -= speed_;
	}
	if (keys[DIK_DOWN]) {
		affine_.translate.y += speed_;
	}
}

/// <summary>
/// カメラの移動
/// </summary>
void Camera::CameraMove(char *keys) {
	CameraScale(keys);
	CameraRotation(keys);
	CameraTranslate(keys);
}

void Camera::CameraUpdate() {
	CameraMatrix();
	ViewMatrix();
	OrthoMatrix();
	ViewportMatrix();
	MakeVpVpMatrix();
	CameraTransform();
}

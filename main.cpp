#include "Camera.h"

const char kWindowTitle[] = "GC1A_01_イイヅカ_ソラ_title";

const int BOX_SIZE = 100;

const Vector2 kGravity = { 0,-9.8f };

float Length(float newton) {
	return sqrtf(powf(newton, 2));
}
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, SCREEN_WIDTH, SCREEN_HEIGHT);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Camera* camera = new Camera;

	int texture = Novice::LoadTexture("white1x1.png");

	Box box;
	box.affine = { {1,1},0,{50,50} };
	box.local  = {
		{(float)-BOX_SIZE / 2.0f,(float)-BOX_SIZE  / 2.0f},
		{(float)-BOX_SIZE / 2.0f,(float) BOX_SIZE  / 2.0f},
		{(float) BOX_SIZE / 2.0f,(float)-BOX_SIZE  / 2.0f},
		{(float) BOX_SIZE / 2.0f,(float) BOX_SIZE  / 2.0f},
	};
	box.velocity     = {};
	box.acceleration = {};
	box.mass         = 1;
	box.screen       = {};
	box.world        = {};
	box.wvpVp        = {};
	box.color        = WHITE;

	float miu = 0.4f;
	float magnitude = 0;
	Vector2 direction = {0,0};
	Vector2 frictionalForce = {};
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		
		//座標変換↓
		camera->CameraUpdate();
		camera->CameraMove(keys);

		box.world  = camera->MakeAffineMatrix(box.affine);
		box.wvpVp  = camera->Multiply(box.world, camera->GetVpVpMatrix());
		box.screen = camera->Transform(box.local, box.wvpVp);
		//座標変換↑

		//初速度
		if (keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
			box.velocity.x = 70.0f;
		}

		//摩擦の計算
		if (fabs(box.velocity.x) > 0.01f || fabs(box.velocity.y) > 0.01f) {
			magnitude = miu * Length(-box.mass * kGravity.y);//F=u||N||
			direction.x = box.velocity.x / sqrtf((float)pow(box.velocity.x, 2))*-1;
			frictionalForce.x = magnitude * direction.x;

			box.acceleration.x = frictionalForce.x / box.mass;//a=F/m

			if (fabs(box.acceleration.x / 60.f) > fabs(box.velocity.x)) {
				box.acceleration.x = box.velocity.x * 60.0f;
			}
		}

		//移動
		box.velocity.x         += box.acceleration.x/60.0f;
		box.affine.translate.x += box.velocity.x/60.0f;

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		camera->Draw();
		Novice::DrawQuad(
			(int)box.screen.leftTop.x,     (int)box.screen.leftTop.y,
			(int)box.screen.rightTop.x,    (int)box.screen.rightTop.y,
			(int)box.screen.leftBottom.x,  (int)box.screen.leftBottom.y,
			(int)box.screen.rightBottom.x, (int)box.screen.rightBottom.y,
			0, 0, 1, 1, texture, box.color);
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	delete camera;
	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

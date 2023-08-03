#include "Player.h"
#include "newMath.h"

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}

	delete sprite2DReticle_;

}

void Player::Attack() {
	if (!isControl) {
		XINPUT_STATE joyState;
		if (!Input::GetInstance()->GetJoystickState(0, joyState)) {
			return;
		}
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			isAttack = true;
		} else {
			isAttack = false;
		}
	} else if (isControl){
		if (input_->PushKey(DIK_SPACE)) {
			isAttack = true;
		} else {
			isAttack = false;
		}
	}


	if (isAttack) {
		if (count == 0) {
			//弾の速度
			const float kBulletSpeed = 1.0f;
			//Vector3 velocity(0, 0, kBulletSpeed);
			Vector3 velocity;
			Vector3 Reticle3DPos;
			Reticle3DPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
			Reticle3DPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
			Reticle3DPos.z = worldTransform3DReticle_.matWorld_.m[3][2];
			//velocity = Math::TransformNormal(velocity, worldTransform_.matWorld_);

			velocity.x = Reticle3DPos.x - worldTransform_.matWorld_.m[3][0];
			velocity.y = Reticle3DPos.y - worldTransform_.matWorld_.m[3][1];
			velocity.z = Reticle3DPos.z - worldTransform_.matWorld_.m[3][2];
			velocity = Math::FVMultiply(kBulletSpeed, Math::Normalize(velocity));


			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, GetWorldPosition(), velocity);
			// 弾を登録
			//bullet_ = newBullet;
			bullets_.push_back(newBullet);
			count++;
		}
	} else {
		count = 0;
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle) {

	//NULLポインタチェック
	assert(model);
	
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	worldTransform_.translation_.z = 50.0f;

	input_ = Input::GetInstance();

	worldTransform3DReticle_.Initialize();

	uint32_t textureReticle = TextureManager::Load("reticle.png");

	sprite2DReticle_ = Sprite::Create(
	    textureReticle, {WinApp::kWindowWidth / 2, WinApp::kWindowHeight / 2}, {1, 1, 1, 1},
	    {0.5f, 0.5f});

};

void Player::OnCollision() {

}

Vector3 Player::GetWorldPosition(){
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::Update(ViewProjection& viewProjection) {
	XINPUT_STATE joyState;

	//デスフラグの立った球を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	worldTransform_.TransferMatrix();
	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		move.x += (float)joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed;
		move.y += (float)joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
		inputFloat3[0] = worldTransform_.translation_.x;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
		inputFloat3[0] = worldTransform_.translation_.x;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
		inputFloat3[1] = worldTransform_.translation_.y;
	} else if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
		inputFloat3[1] = worldTransform_.translation_.y;
	}

	const float kRotSpeed = 0.02f;

	//押した方向でベクトル移動
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed; 
	}

	// ImGui加算用
	worldTransform_.translation_.x = inputFloat3[0];
	worldTransform_.translation_.y = inputFloat3[1];

	// ベクターの加算
	worldTransform_.translation_ = Math::Add(worldTransform_.translation_, move);
	// アフィン変換行列の作成
	worldTransform_.matWorld_ = Math::MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// ImGuiスライダー
	ImGui::Begin("PlayerDebug");
	ImGui::Text("DebugCamera Toggle : ENTER");
	ImGui::Text("Change Mouse : Push M");
	ImGui::Text("Change Controller : Push C");
	ImGui::SliderFloat3("Positions", inputFloat3, -20.0f, 20.0f);
	// ImGui終わり
	ImGui::End();

	// 移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	Attack();
	worldTransform_.UpdateMatrix();
	
	MouseUpdate(viewProjection);

	/* const float KDistancePlayerTo3DReticle = 50.0f;
	Matrix4x4 Reticle3DTransMat = Math::MakeTranslateMatrix(worldTransform3DReticle_.translation_);
	Vector3 offset = {0, 0, 1.0f};
	Matrix4x4 worMat = Math::MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);
	offset = Math::TransformNormal(offset, worldTransform_.matWorld_);
	offset = Math::FVMultiply(KDistancePlayerTo3DReticle, Math::Normalize(offset));
	worldTransform3DReticle_.translation_.x = offset.x + worldTransform_.matWorld_.m[3][0]; 
	worldTransform3DReticle_.translation_.y = offset.y + worldTransform_.matWorld_.m[3][1]; 
	worldTransform3DReticle_.translation_.z = offset.z + worldTransform_.matWorld_.m[3][2]; 
	worldTransform3DReticle_.UpdateMatrix();

	Vector3 positionReticle;
	positionReticle.x = worldTransform3DReticle_.matWorld_.m[3][0];
	positionReticle.y = worldTransform3DReticle_.matWorld_.m[3][1];
	positionReticle.z = worldTransform3DReticle_.matWorld_.m[3][2];
	Matrix4x4 matViewPort =
	    Math::MakeViewPortMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	Matrix4x4 matViewProjectionViewPort =
	    Math::MMMultiply(viewProjection.matProjection, matViewPort);
	positionReticle = Math::TransformCoord(positionReticle, matViewProjectionViewPort);
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));*/

	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

};

void Player::Draw(ViewProjection viewProjection){

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);
	//弾描画
	/* if (bullet_) {
		bullet_->Draw(viewProjection);
	}*/

	//弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

};

void Player::DrawUI() { sprite2DReticle_->Draw(); }

void Player::MouseUpdate(ViewProjection& view) {

	if (input_->TriggerKey(DIK_M)) {
		isControl = true;
	} else if (input_->TriggerKey(DIK_C)) {
		isControl = false;
	}

	if (isControl) {
		POINT mousePosition;

		GetCursorPos(&mousePosition);

		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);

		sprite2DReticle_->SetPosition({float(mousePosition.x), float(mousePosition.y)});

	} else if (!isControl) {
		XINPUT_STATE joyState;
		Vector2 spritePosition = sprite2DReticle_->GetPosition();

		if (Input::GetInstance()->GetJoystickState(0, joyState)) {
			spritePosition.x += (float)joyState.Gamepad.sThumbRX / SHRT_MAX * 8.0f;
			spritePosition.y -= (float)joyState.Gamepad.sThumbRY / SHRT_MAX * 8.0f;

			sprite2DReticle_->SetPosition(spritePosition);
		}

	}

	Matrix4x4 viewPortMat = Math::MakeViewPortMatrix(
	    0, 0, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 1.0f);
	Matrix4x4 matVPV =
	    Math::MMMultiply(view.matView, Math::MMMultiply(view.matProjection, viewPortMat));
	Matrix4x4 matInverseVPV = Math::Inverse(matVPV);

	Vector3 posNear =
	    Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 0);
	Vector3 posFar =
	    Vector3(sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y, 1);

	posNear = Math::TransformCoord(posNear, matInverseVPV);
	posFar = Math::TransformCoord(posFar, matInverseVPV);

	Vector3 mouseDirection;
	mouseDirection.x = posFar.x - posNear.x;
	mouseDirection.y = posFar.y - posNear.y;
	mouseDirection.z = posFar.z - posNear.z;
	mouseDirection = Math::Normalize(mouseDirection);

	const float kDistanceTestObject = 100.0f;
	worldTransform3DReticle_.translation_.x =
	    Math::FVMultiply(kDistanceTestObject, mouseDirection).x + posNear.x;
	worldTransform3DReticle_.translation_.y =
	    Math::FVMultiply(kDistanceTestObject, mouseDirection).y + posNear.y;
	worldTransform3DReticle_.translation_.z =
	    Math::FVMultiply(kDistanceTestObject, mouseDirection).z + posNear.z;

	worldTransform3DReticle_.UpdateMatrix();

	ImGui::Begin("Player");
	ImGui::Text(
	    "2DReticle:(%f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
	    "3DReticle:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();

}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }
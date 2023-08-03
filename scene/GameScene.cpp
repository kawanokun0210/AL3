#include "GameScene.h"
#include "AxisIndicator.h"
#include "TextureManager.h"
#include <cassert>
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	for (Enemy* enemy : enemys_) {
		delete enemy;
	}
	//delete modelSkydome_;
	delete skydome_;
	delete railCamera_;

	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// テクスチャを読み込み
	textureHandle_ = TextureManager::Load("sample.png");
	// 3Dモデルの生成
	model_ = Model::Create();
	worldTransform_.Initialize();
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	// 敵の生成
	//enemy_ = new Enemy;
	//enemy_->SetPlayer(player_);
	//Vector3 position = {30.0f, 0.0f, 30.0f};
	// 敵初期化
	//enemy_->Initialize(model_, position);
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// アドレス渡し
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(modelSkydome_, {0, 80, 0});

	railCamera_ = new RailCamera();
	railCamera_->Initialize({0, 0, -100.0f}, player_->GetWorldMatrix().rotation_);

	player_->SetParent(&railCamera_->GetWorldTransform());

	//TextureManager::Load("beam.png");

	LoadEnemyPopData();

}

void GameScene::Update() {
	// 自キャラの更新

	player_->Update(viewProjection_);

	//enemy_->Update();

	UpDateEnemyPopCommands();

	enemys_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	for (Enemy* enemy : enemys_) {
		enemy->Update();
	}

	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	CheckAllCollisions();

	skydome_->Update();

//#ifdef _DEBUG

	if (input_->TriggerKey(DIK_RETURN) && isDebugCameraActive_ == false) {
		isDebugCameraActive_ = true;
	} else if (input_->TriggerKey(DIK_RETURN) && isDebugCameraActive_ == true) {
		isDebugCameraActive_ = false;
	}
	// カメラの処理
	if (isDebugCameraActive_ == true) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		railCamera_->Updata();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
		// ビュープロジェクション行列の更新と転送
		//viewProjection_.UpdateMatrix();
	}
//#endif

	//debugCamera_->Update();

}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	player_->Draw(viewProjection_);

	for (Enemy* enemy : enemys_) {
		enemy->Draw(viewProjection_);
	}

	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}

	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	//player_->DrawUI();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	//const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾
	posA = player_->GetWorldPosition();

	for (EnemyBullet* bullet : enemyBullets_) {
		posB = bullet->GetWorldPosition();

		float judge = (posB.x - posA.x) * (posB.x - posA.x) +
		              (posB.y - posA.y) * (posB.y - posA.y) + (posB.z - posA.z) * (posB.z - posA.z);

		float playerRad = 2.5f;
		float enemyRad = 2.5f;
		if (judge <= (playerRad + enemyRad) * (playerRad + enemyRad)) {
			player_->OnCollision();
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラ

	for (PlayerBullet* playerBullet : playerBullets) {
		posB = playerBullet->GetWorldPosition();
		for (EnemyBullet* enemyBullet : enemyBullets_) {
			posA = enemyBullet->GetWorldPosition();

			float judge = (posB.x - posA.x) * (posB.x - posA.x) +
			              (posB.y - posA.y) * (posB.y - posA.y) +
			              (posB.z - posA.z) * (posB.z - posA.z);

			float playerRad = 2.5f;
			float enemyRad = 2.5f;
			if (judge <= (playerRad + enemyRad) * (playerRad + enemyRad)) {
				playerBullet->OnCollision();
				enemyBullet->OnCollision();
			}
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラ
	for (Enemy* enemy : enemys_) {
		posB = enemy->GetWorldPosition();

		for (PlayerBullet* bullet : playerBullets) {
			posA = bullet->GetWorldPosition();

			float judge = (posB.x - posA.x) * (posB.x - posA.x) +
			              (posB.y - posA.y) * (posB.y - posA.y) +
			              (posB.z - posA.z) * (posB.z - posA.z);

			float playerRad = 2.5f;
			float enemyRad = 2.5f;
			if (judge <= (playerRad + enemyRad) * (playerRad + enemyRad)) {
				player_->OnCollision();
				bullet->OnCollision();
			}
		}
	}
#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	//リストに登録する
	enemyBullets_.push_back(enemyBullet);
}

void GameScene::AddEnemy(Enemy* enemy) { enemys_.push_back(enemy); }

void GameScene::EnemyIni(Model* model, const Vector3 position) { 
	Enemy* newEnemy = new Enemy();
	newEnemy->Initialize(model, position, this);
	newEnemy->SetPlayer(player_);
	AddEnemy(newEnemy);
}

void GameScene::LoadEnemyPopData() { 
	std::ifstream file;
	file.open("./Resources/enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();

	file.close();
}

void GameScene::UpDateEnemyPopCommands() {

	if (isWait_) {
		waitTimer_--;

		if (waitTimer_ <= 0) {
			isWait_ = false;
		}
		return;
	}

	std::string line;
	
	while (getline(enemyPopCommands, line)) {
		std::istringstream line_stream(line);

		std::string word;

		getline(line_stream, word, ',');

		if (word.find("//") == 0) {
			continue;
		} else if (word.find("POP") == 0) {
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			EnemyIni(model_, Vector3(x, y, z));
		} else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			int32_t waitTime = atoi(word.c_str());

			isWait_ = true;
			waitTimer_ = waitTime;
			break;
		}

	}

}
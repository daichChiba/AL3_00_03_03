#include "GameScene.h"
#include "MathUtilityForText.h"
#include "TextureManager.h"
#include <cassert>

// インストラクタ
GameScene::GameScene() {}
// デストラクタ
GameScene::~GameScene() {
	delete model_;
	// ここからAL3の02_01の22をやる
	delete player_;

	delete model3d_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete modelSkydome_;

	delete skydome_;

	delete mapChipField_;

	delete debugCamera_;
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// ブロック１個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}
	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
				worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
			}
		}
	}
}

void GameScene::Initialize() {

	//// ファイル名を指定してテクスチャを読み込む
	// textureHandle_ = TextureManager::Load("chopper.png");

	// 3Dモデルの作成
	model_ = Model::Create();

	// ビュープロジェクション
	viewProjection_.farZ = 600;
	viewProjection_.Initialize();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 3Dモデルデータの生成
	model3d_ = Model::CreateFromOBJ("cube");

	//　3Dplayerモデルデータの作成
	modelPlayer_ = Model::CreateFromOBJ("player");

	skydome_ = new Skydome;

	player_ = new Player; //
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(3, 3);
	player_->Initialize(modelPlayer_, &viewProjection_,playerPosition);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenerateBlocks();

	// 3Dモデルの生成(スカイドーム)
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);

	// 　天球の生成
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(640, 360);
	debugCamera_->SetFarZ(5000000);
}

void GameScene::Update() {
	 //自キャラの更新
	 player_->Update();

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			// 04/24 02_02の更新から始める

			if (!worldTransformBlock) {
				continue;
			}

			worldTransformBlock->UpdateMatrix();
		}
	}
	skydome_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // DEBUG
	   // カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		// デバックカメラのビュー行列
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		// デバックカメラのプロジェクション行列
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.UpdateMatrix();
	}
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


	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			}
			skydome_->Draw();
			player_->Draw();
			model3d_->Draw(*worldTransformBlock, debugCamera_->GetViewProjection());
		}
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

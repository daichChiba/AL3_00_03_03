#include "GameScene.h"
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
}

void GameScene::Initialize() {

	//// ファイル名を指定してテクスチャを読み込む
	// textureHandle_ = TextureManager::Load("chopper.png");

	// 3Dモデルの作成
	model_ = Model::Create();

	// ビュープロジェクション
	viewProjection_.Initialize();

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 3Dモデルデータの生成
	model3d_ = Model::CreateFromOBJ("cube");

	player_ = new Player; //
	player_->Initialize(model_, textureHandle_, &viewProjection_);

	// 要素数
	const uint32_t kNumBlockVirtical = 10;
	const uint32_t kNumBlockHorizontal = 20;

	// ブロック１個分の横幅
	const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;

	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical;++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockHorizontal; i++) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			worldTransformBlocks_[i][j] = new WorldTransform;
			worldTransformBlocks_[i][j]->Initialize();
			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * i;
			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * j;
		}
	}
}

void GameScene::Update() {

	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			// 04/24 02_02の更新から始める

			////平行移動
			Matrix4x4 result{
			    1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				worldTransformBlock->translation_.x,
				worldTransformBlock->translation_.y,
				worldTransformBlock->translation_.z,
			    1.0f};

			// 平行移動だけ表示
			worldTransformBlock->matWorld_ = result;

			worldTransformBlock->TransferMatrix();
		}
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

	// player_->Draw();
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			model3d_->Draw(*worldTransformBlock, viewProjection_);
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

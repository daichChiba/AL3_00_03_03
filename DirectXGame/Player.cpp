#include "Player.h"
#include "cassert"

// Initializeの関数定義
void Player::Initialize(Model* model, uint32_t textureHandle,ViewProjection* viewProjection) {

	//NULLポインタチェック
	assert(model);

	//データを引数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	worldTransform_.Initialize();
	viewProjection_ = viewProjection;
}

// Updateの関数定義
void Player::Update() {
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();

}

// Drawの関数定義
void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
 }

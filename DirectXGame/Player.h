#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Input.h"


enum class LRDirection {
	kRight,
	kLeft
};

/// <summary>
/// 自キャラ
/// <summary>
class Player {

public:
	/// <summary>
	/// 初期化
	/// <summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize(Model* model,ViewProjection* viewProjection,const Vector3& pos);

	/// <summary>
	/// 更新
	/// <summary>
	void Update();

	/// <summary>
	/// 描画
	/// <summary>
	void Draw();

	LRDirection lrDirection_ = LRDirection::kRight;

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	Vector3 velocity_ = {};

	static inline const float kAcceleration = 0.5f;
	static inline const float kAttenuation = 0.5f;
	static inline const float kLimitRunSpeed = 0.5f;

	// 旋回開始の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
};

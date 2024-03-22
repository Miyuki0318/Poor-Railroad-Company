/*!
@file Player.cpp
@brief プレイヤー
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Input;
	using namespace Utility;

	// 生成時の処理
	void Player::OnCreate()
	{
		// 継承元の生成時の処理を実行
		TemplateObject::OnCreate();
		SetTransParam();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<PNTStaticDraw>();
		m_ptrDraw->SetMeshResource(L"DEFAULT_CUBE");
		m_ptrDraw->SetDiffuse(COL_RED);

		// コリジョンOBBの追加
		m_ptrColl = AddComponent<CollisionObb>();

		// セレクトインディケーターの生成
		m_indicator = GetStage()->AddGameObject<SelectIndicator>(GetThis<Player>());

		// タグの設定
		AddTag(L"Player");
	}

	// 毎フレーム更新処理
	void Player::OnUpdate()
	{
		// Aボタンが入力されたら
		if (GetPushA()) OnPushA();

		// Xボタンが入力されたらクラフト
		if (GetPushX()) OnCraft();

		// 移動更新
		UpdateMove();

		// デバック用文字列
		Debug::Log(L"プレイヤーの座標 : ", GetPosition());
	}

	// Aボタン入力時
	void Player::OnPushA()
	{

	}

	// 採掘呼び出し
	void Player::OnMining()
	{

	}

	// クラフト呼び出し
	void Player::OnCraft()
	{

	}

	// 移動更新
	void Player::UpdateMove()
	{
		// LStickの入力があれば
		if (IsInputLStick())
		{
			// LStick入力量の取得
			Vec3 stickValue = Vec3(GetLStickValue().x, 0.0f, GetLStickValue().y);

			ControllerRotation(stickValue); // 回転関数
			ControllerMovement(stickValue);	// 移動関数
		}
	}

	// コントローラーによる回転
	void Player::ControllerRotation(const Vec3& stickValue)
	{
		// スティックの傾きに合わせてオブジェクトを回転させる
		float rotY = atan2f(-stickValue.z, stickValue.x);
		SetRotation(Vec3(0.0f, rotY, 0.0f));
	}
	
	// コントローラーによる移動
	void Player::ControllerMovement(const Vec3& stickValue)
	{
		// 現在の座標に入力量×速度×デルタタイムで加算
		Vec3 pos = GetPosition();
		pos += stickValue * m_speed * DELTA_TIME;
		
		// 座標の更新
		SetPosition(pos);
	}
}
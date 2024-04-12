/*!
@file TitlePlayer.cpp
@brief タイトル用プレイヤーオブジェクト
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Input;
	using namespace Utility;

	// 毎フレーム更新処理
	void TitlePlayer::OnUpdate()
	{
		// 移動と回転の更新処理
		UpdateMove();
		UpdateRotation();
		
		// デバック用文字列
		Debug::Log(L"プレイヤーの座標 : ", GetPosition());
	}

	// コンポーネント設定
	void TitlePlayer::CreateComponent()
	{
		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->SetMultiMeshResource(L"PLAYER");
		m_ptrDraw->SetSpecularColor(COL_BLACK);
		m_ptrDraw->SetOwnShadowActive(true);
		m_ptrDraw->SetLightingEnabled(false);

		// アニメーションの設定
		m_ptrDraw->AddAnimation(L"WALK", 0, 60, true);
		m_ptrDraw->ChangeCurrentAnimation(L"WALK");

		auto shadowMap = AddComponent<Shadowmap>();
		shadowMap->SetMultiMeshResource(L"PLAYER");
		shadowMap->SetMeshToTransformMatrix(m_modelMat);

		// コリジョンOBBの追加
		AddComponent<CollisionCapsule>();

		// 重力の追加
		AddComponent<Gravity>();
	}

	// 移動更新
	void TitlePlayer::UpdateMove()
	{
		// LStickの入力があるなら
		if (IsInputLStick())
		{
			// LStick入力量の取得
			Vec3 stickValue = Vec3(GetLStickValue().x, 0.0f, GetLStickValue().y);

			SetRotateTarget(stickValue); // 回転関数
			ControllerMovement(stickValue);	// 移動関数

			// アニメーションの更新
			m_ptrDraw->UpdateAnimation(DELTA_TIME * 2.0f);
		}
	}
}
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

	// 生成時の処理
	void TitlePlayer::OnCreate()
	{
		// 継承元の生成時の処理を実行
		TemplateObject::OnCreate();
		SetTransParam();

		// 描画コンポーネントの設定
		m_ptrDraw = AddComponent<BcPNTBoneModelDraw>();
		m_ptrDraw->SetMeshToTransformMatrix(m_modelMat);
		m_ptrDraw->SetMultiMeshResource(L"PLAYER");
		m_ptrDraw->SetSpecularColor(COL_BLACK);
		m_ptrDraw->SetOwnShadowActive(true);
		m_ptrDraw->SetLightingEnabled(false);

		// アニメーションの設定
		m_ptrDraw->AddAnimation(L"WALK", 0, 60, true);

		auto shadowMap = AddComponent<Shadowmap>();
		shadowMap->SetMultiMeshResource(L"PLAYER");
		shadowMap->SetMeshToTransformMatrix(m_modelMat);

		// コリジョンOBBの追加
		AddComponent<CollisionCapsule>();

		// 重力の追加
		AddComponent<Gravity>();

		// タグの設定
		AddTag(L"Player");
	}

	// 毎フレーム更新処理
	void TitlePlayer::OnUpdate()
	{
		// 移動と回転の更新処理
		UpdateMove();
		UpdateRotation();
		
		// デバック用文字列
		Debug::Log(L"プレイヤーの座標 : ", GetPosition());
	}

	// 移動更新
	void TitlePlayer::UpdateMove()
	{
		// LStickの入力があるなら
		bool isLStick = IsInputLStick();
		if (isLStick)
		{
			// LStick入力量の取得
			Vec3 stickValue = Vec3(GetLStickValue().x, 0.0f, GetLStickValue().y);

			SetRotateTarget(stickValue); // 回転関数
			ControllerMovement(stickValue);	// 移動関数
		}
	}

	// 回転先設定
	void TitlePlayer::SetRotateTarget(const Vec3& stickValue)
	{
		// コントローラーの入力から回転方向ベクトルを設定
		float rotY = atan2f(-stickValue.z, stickValue.x);
		m_rotTarget = Vec3(cos(rotY), 0.0f, sin(rotY));
	}

	// 回転更新処理
	void TitlePlayer::UpdateRotation()
	{
		// 回転方向ベクトルと現在の回転ベクトルの差分から回転量を設定
		Vec3 rot = m_rotTarget - m_currentRot;
		float rotate = rot.length() * (DELTA_TIME / m_rotSpeed);
		m_currentRot += Vec3(rot.normalize() * rotate);

		// 回転量を加算された現在の回転ベクトルのラジアンをY軸として設定
		float rotY = atan2f(m_currentRot.z, m_currentRot.x);
		SetRotation(Vec3(0.0f, rotY, 0.0f));
	}

	// コントローラーによる移動
	void TitlePlayer::ControllerMovement(const Vec3& stickValue)
	{
		// 現在の座標に入力量×速度×デルタタイムで加算
		Vec3 pos = GetPosition();
		pos += stickValue * m_moveSpeed * DELTA_TIME;

		// 座標の更新
		SetPosition(pos);

		// アニメーションの更新
		m_ptrDraw->UpdateAnimation(DELTA_TIME * 2.0f);
	}
}
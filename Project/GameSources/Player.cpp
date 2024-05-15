/*!
@file Player.cpp
@brief プレイヤー
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

#define MESH L"SM_PLAYER_"

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

		// コンポーネントの設定
		CreateComponent();

		// タグの設定
		AddTag(L"Player");
	}

	// アニメーションメッシュの切り替え
	void Player::SetAnimationMesh(ePAKey animation, float start)
	{
		if (!m_ptrDraw) return;
		if (!m_ptrShadow) return;

		// アニメーション付きメッシュの変更
		AnimationMap mesh = m_animationMap.at(animation);
		m_ptrDraw->SetMeshResource(MESH + mesh.animeKey);
		m_ptrDraw->AddAnimation(mesh.animeKey, 0, mesh.flameNum, mesh.loopActive);
		m_ptrDraw->ChangeCurrentAnimation(mesh.animeKey, start);

		// 影の更新
		m_ptrShadow->SetMeshResource(MESH + mesh.animeKey);
	}

	// 指定アニメーションになっているかの確認
	bool Player::IsAnimation(ePAKey animation)
	{
		if (!m_ptrDraw) return false;

		// 現在のアニメーションがアニメーションキーと一致しているか
		return m_ptrDraw->GetCurrentAnimation() == m_animationMap.at(animation).animeKey;
	}

	// 指定アニメーションになっているかの確認
	bool Player::IsAnimeEnd(ePAKey animation)
	{
		if (!IsAnimation(animation)) return false;

		// 現在のアニメーションがアニメーションキーと一致しているか
		return m_ptrDraw->IsTargetAnimeEnd();
	}

	// 回転先設定
	void Player::SetRotateTarget(const Vec3& stickValue)
	{
		// コントローラーの入力から回転方向ベクトルを設定
		float rotY = atan2f(-stickValue.z, stickValue.x);
		m_rotTarget = Vec3(cos(rotY), 0.0f, sin(rotY));
	}

	// 回転更新処理
	void Player::UpdateRotation()
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
	void Player::ControllerMovement(const Vec3& stickValue)
	{
		// 現在の座標に入力量×速度×デルタタイムで加算
		Vec3 pos = GetPosition();
		pos += stickValue * m_moveSpeed * DELTA_TIME;
		
		// csvグリッドとの衝突判定を実行
		GridHitResponse(pos);
	}

	// グリッドに衝突してないかの応答処理
	void Player::GridHitResponse(Vec3& pos)
	{
		// ステージcsv配列の取得
		const auto& stageMap = GetTypeStage<StageCSV>()->GetStageMap();

		// 列と行
		size_t row, col;
		row = ROW(round(pos.z + 0.5f));
		col = COL(round(pos.x));

		// 配列の範囲外じゃないかのチェック
		if (!WithInElemRange(row, col, stageMap)) return;

		// 各方向への応答処理
		GridHitFlontResponse(pos);
		GridHitBackResponse(pos);
		GridHitLeftResponse(pos);
		GridHitRightResponse(pos);

		// 座標の更新
		SetPosition(pos);
	}

	// 指定したグリッドが衝突判定を取るグリッドかを取得
	bool Player::GetIsImpassable(size_t row, size_t col)
	{
		// ステージcsv配列の取得
		const auto& stageMap = GetTypeStage<StageCSV>()->GetStageMap();

		// 配列の範囲外じゃないかのチェック
		if (!WithInElemRange(row, col, stageMap)) return false;

		// 通れないマスIDと一致するか
		eStageID id = STAGE_ID(stageMap.at(row).at(col));
		return m_impassableSet.find(id) != m_impassableSet.end();
	}

	// 前方向への衝突
	void Player::GridHitFlontResponse(Vec3& pos)
	{
		// 列と行
		size_t row, col;
		row = ROW(round(pos.z + 0.5f)) + 1;
		col = COL(round(pos.x));

		// 衝突判定を取るIDかのチェック
		if (!GetIsImpassable(row, col)) return;

		// 応答処理
		float gridZ = -float(row);
		if (pos.z - m_radius < gridZ)
		{
			pos.z = gridZ + m_radius;
		}
	}

	// 後方向への衝突
	void Player::GridHitBackResponse(Vec3& pos)
	{
		// 列と行
		size_t row, col;
		row = ROW(round(pos.z)) - 1;
		col = COL(round(pos.x));

		// 衝突判定を取るIDかのチェック
		if (!GetIsImpassable(row, col)) return;

		// 応答処理
		float gridZ = -float(row);
		if (pos.z + m_radius > gridZ)
		{
			pos.z = gridZ - m_radius;
		}
	}

	// 左方向への衝突
	void Player::GridHitLeftResponse(Vec3& pos)
	{
		// 列と行
		size_t row, col;
		row = ROW(round(pos.z));
		col = COL(round(pos.x + 0.5f)) - 1;

		// 衝突判定を取るIDかのチェック
		if (!GetIsImpassable(row, col)) return;

		// 応答処理
		float gridX = float(col);
		if (pos.x - m_radius < gridX)
		{
			pos.x = gridX + m_radius;
		}
	}

	// 右方向への衝突
	void Player::GridHitRightResponse(Vec3& pos)
	{
		// 列と行
		size_t row, col;
		row = ROW(round(pos.z));
		col = COL(round(pos.x)) + 1;

		// 衝突判定を取るIDかのチェック
		if (!GetIsImpassable(row, col)) return;

		// 応答処理
		float gridX = float(col);
		if (pos.x + m_radius > gridX)
		{
			pos.x = gridX - m_radius;
		}
	}
}
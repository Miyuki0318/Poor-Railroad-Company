/*!
@file Player.cpp
@brief プレイヤー
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

#define MESH L"SM_PLAYER_"

const int GRID_SIZE = 1;
const float GRID_HELF = 0.5f;

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

	// アニメーションの更新
	void Player::UpdateAnimation(float speedValue)
	{
		m_ptrDraw->UpdateAnimation(DELTA_TIME * speedValue);
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

	// 移動更新処理
	void Player::UpdateMove()
	{
		// サウンドアイテムが存在しない、またはリソースが空なら
		if (auto& item = m_walkSoundItem.lock()) if (item->m_AudioResource.lock()) return;

		// 歩いた時のSEを再生
		StartWalkSoundEffect();
	}

	// 回転更新処理
	void Player::UpdateRotation()
	{
		// 回転方向ベクトルと現在の回転ベクトルの差分から回転量を設定
		Vec3 rot = m_rotTarget - m_currentRot;
		float rotate = rot.length() * (DELTA_TIME / m_rotSpeed);
		m_currentRot += Vec3(rot.normalize() * rotate);
		m_moveValue += rotate / m_maxMove;

		// 回転量を加算された現在の回転ベクトルのラジアンをY軸として設定
		float rotY = atan2f(m_currentRot.z, m_currentRot.x);
		SetRotation(Vec3(0.0f, rotY, 0.0f));
	}
	
	// 歩いた時のSE再生
	void Player::StartWalkSoundEffect()
	{
		// 列と行
		size_t row, col;
		Vec3 pos = GetPosition();
		row = ROW(floor(pos.z + GRID_HELF));
		col = COL(floor(pos.x + GRID_HELF));

		const auto& groundMap = GetTypeStage<BaseStage>()->GetGroundMap();
		if (!WithInElemRange(row, col, groundMap)) return;

		eStageID groundID = STAGE_ID(groundMap.at(row).at(col));
		if (m_walkSEKeyMap.find(groundID) != m_walkSEKeyMap.end())
		{
			m_walkSoundItem = StartSE(m_walkSEKeyMap.at(groundID), 1.0f);
		}
	}

	// コントローラーによる移動
	void Player::ControllerMovement(const Vec3& stickValue)
	{
		// 現在の座標に入力量×速度×デルタタイムで加算
		Vec3 pos = GetPosition();
		Vec3 newPos = pos + stickValue * m_playerData.at(ePST::WalkSpeed).at(m_playerLevel) * m_acsel * DELTA_TIME;
		m_moveValue += (newPos - pos).lengthSqr();

		// csvグリッドとの衝突判定を実行
		GridHitResponse(newPos);
	}

	// グリッドに衝突してないかの応答処理
	void Player::GridHitResponse(Vec3& pos)
	{
		// ステージcsv配列の取得
		const auto& stagePtr = GetTypeStage<BaseStage>();
		const auto& stageMap = stagePtr->GetStageMap();
		const auto& positionMap = stagePtr->GetStagePosMap();

		// 列と行
		size_t row, col;
		row = ROW(floor(pos.z + GRID_HELF));
		col = COL(floor(pos.x + GRID_HELF));

		// 配列の範囲外じゃないかのチェック
		if (!WithInElemRange(row, col, stageMap)) return;

		// 衝突判定
		GridHitFlontResponse(pos, positionMap);
		GridHitBackResponse(pos, positionMap);
		GridHitLeftResponse(pos, positionMap);
		GridHitRightResponse(pos, positionMap);
		GridHitOliqueResponse(pos, positionMap);

		// ステージ配列の範囲外に行かない様に修正
		GridRangeResponse(pos, stageMap);

		// 座標の更新
		SetPosition(pos);
	}

	// 指定したグリッドが衝突判定を取るグリッドかを取得
	bool Player::GetIsImpassable(size_t row, size_t col)
	{
		// ステージcsv配列の取得
		const auto& stageMap = GetTypeStage<BaseStage>()->GetStageMap();
		const auto&	groundMap = GetTypeStage<BaseStage>()->GetGroundMap();

		// 配列の範囲外じゃないかのチェック
		if (!WithInElemRange(row, col, stageMap)) return false;
		if (!WithInElemRange(row, col, groundMap)) return false;

		// 通れないマスIDと一致するか
		eStageID stageID = STAGE_ID(stageMap.at(row).at(col));
		eStageID groundID = STAGE_ID(groundMap.at(row).at(col));

		// IDが通れないマスIDリストに登録されてないかのチェック
		return (m_impassableSet.find(stageID) != m_impassableSet.end()
			|| m_impassableSet.find(groundID) != m_impassableSet.end());
	}

	// 三平方の定理で押し出し処理
	void Player::GridSquareTheorem(Vec3& pos, float gridX, float gridZ)
	{
		// 三平方の定理で押し出し処理を求める
		float a = pos.x - gridX;
		float b = pos.z - gridZ;
		float c = a * a + b * b;
		if (c < m_radius * m_radius)
		{
			c = sqrtf(c);
			float s = m_radius - c;
			float dx = a / c;
			float dz = b / c;
			pos.x += dx * s;
			pos.z += dz * s;
		}
	}

	// 前方向への衝突
	void Player::GridHitFlontResponse(Vec3& pos, const vector<vector<Vec3>>& posMap)
	{
		// 列と行
		size_t row, col;
		row = ROW(floor(pos.z + GRID_HELF)) + GRID_SIZE;
		col = COL(floor(pos.x + GRID_HELF));

		// 衝突判定を取るIDかのチェック
		if (!GetIsImpassable(row, col)) return;

		// 応答処理
		float gridZ = posMap.at(row).at(col).z;
		if (pos.z - m_radius < gridZ)
		{
			pos.z = gridZ + m_radius;
		}
	}

	// 後方向への衝突
	void Player::GridHitBackResponse(Vec3& pos, const vector<vector<Vec3>>& posMap)
	{
		// 列と行
		size_t row, col;
		row = ROW(floor(pos.z + GRID_HELF)) - GRID_SIZE;
		col = COL(floor(pos.x + GRID_HELF));

		// 衝突判定を取るIDかのチェック
		if (!GetIsImpassable(row, col)) return;

		// 応答処理
		float gridZ = posMap.at(row).at(col).z;
		if (pos.z + m_radius > gridZ)
		{
			pos.z = gridZ - m_radius;
		}
	}

	// 左方向への衝突
	void Player::GridHitLeftResponse(Vec3& pos, const vector<vector<Vec3>>& posMap)
	{
		// 列と行
		size_t row, col;
		row = ROW(floor(pos.z + GRID_HELF));
		col = COL(floor(pos.x + GRID_HELF)) - GRID_SIZE;

		// 衝突判定を取るIDかのチェック
		if (!GetIsImpassable(row, col)) return;

		// 応答処理
		float gridX = posMap.at(row).at(col).x;
		if (pos.x - m_radius < gridX)
		{
			pos.x = gridX + m_radius;
		}
	}

	// 右方向への衝突
	void Player::GridHitRightResponse(Vec3& pos, const vector<vector<Vec3>>& posMap)
	{
		// 列と行
		size_t row, col;
		row = ROW(floor(pos.z + GRID_HELF));
		col = COL(floor(pos.x + GRID_HELF)) + GRID_SIZE;

		// 衝突判定を取るIDかのチェック
		if (!GetIsImpassable(row, col)) return;

		// 応答処理
		float gridX = posMap.at(row).at(col).x;
		if (pos.x + m_radius > gridX)
		{
			pos.x = gridX - m_radius;
		}
	}

	// 斜め方向への衝突
	void Player::GridHitOliqueResponse(Vec3& pos, const vector<vector<Vec3>>& posMap)
	{
		// 斜めのグリッド分ループ
		for (const auto& oblique : m_obliqueGridArray)
		{
			// 列と行
			size_t row, col;
			row = ROW(floor(pos.z + GRID_HELF)) + oblique.first;
			col = COL(floor(pos.x + GRID_HELF)) + oblique.second;

			// 衝突判定を取るIDかのチェック
			if (!GetIsImpassable(row, col)) continue;

			// 三平方の定理で押し出し処理を行う
			float gridX = posMap.at(row).at(col).x;
			float gridZ = posMap.at(row).at(col).z;
			GridSquareTheorem(pos, gridX, gridZ);
		}
	}

	// ステージ外への衝突
	void Player::GridRangeResponse(Vec3& pos, const vector<vector<int>>& stageMap)
	{
		// 列と行
		size_t row, col;
		row = ROW(floor(pos.z + GRID_HELF));
		col = COL(floor(pos.x + GRID_HELF));

		pos.x = max(0.0f, pos.x);
		pos.x = min(float(stageMap.at(row).size() - GRID_SIZE), pos.x);
		pos.z = min(-0.1f, pos.z);
		pos.z = max(-float(stageMap.size() - GRID_SIZE), pos.z);
	}
}
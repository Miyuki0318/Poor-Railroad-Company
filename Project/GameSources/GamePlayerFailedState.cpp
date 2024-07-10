/*!
@file GamePlayerFailedState.cpp
@brief プレイヤーのゲーム失敗状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "GamePlayerFailedState.h"
#include "GameTrain.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<GamePlayerFailedState> GamePlayerFailedState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<GamePlayerFailedState> instance(new GamePlayerFailedState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring GamePlayerFailedState::GetStateName() const
	{
		return L"失敗状態ステート";
	}

	// ステート開始時の処理
	void GamePlayerFailedState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// 歩くアニメーションにする
		if (!player->IsAnimation(ePAK::Walking))
		{
			player->SetAnimationMesh(ePAK::Walking);
		}

		// クラフト中なら初期化する
		player->ResetCrafting();

		const auto& stagePtr = player->GetStage();
		const auto& train = stagePtr->GetSharedGameObject<GameTrain>(L"Train");

		// 回転先をゴール演出位置方向にする
		float rotY = atan2f(train->GetPosition().z - player->m_position.z, train->GetPosition().x - player->m_position.x);
		player->m_rotTarget = Vec3(cos(rotY), 0.0f, -sin(rotY));
		m_rotate = Vec3(cos(player->GetRotation().y), 0.0f, sin(player->GetRotation().y));
		m_totalTime = 0.0f;
	}

	// ステート更新時の処理
	void GamePlayerFailedState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// アニメーションを更新
		Vec3 toRot = Utility::Lerp(m_rotate, player->m_rotTarget, m_totalTime);
		m_totalTime += DELTA_TIME / (m_rotate - player->m_rotTarget).length();

		// ローテーションの更新
		float rotY = atan2f(toRot.z, toRot.x);
		player->SetRotation(Vec3(0.0f, rotY, 0.0f));

		// 経過時間が1.0以上になったら
		if (m_totalTime >= 1.0f)
		{
			m_totalTime = 1.0f;

			// 倒れるアニメーションに変更
			if (!player->IsAnimation(ePAK::GameFailed))
			{
				player->SetAnimationMesh(ePAK::GameFailed);
			}
			player->UpdateAnimation(player->m_animationMap.at(ePAK::GameFailed).animeSpeed);
			return;
		}

		// アニメーションの更新
		player->UpdateAnimation();
	}

	// ステート終了時の処理
	void GamePlayerFailedState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Aボタン入力時
	void GamePlayerFailedState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Bボタン入力時
	void GamePlayerFailedState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Yボタン入力時
	void GamePlayerFailedState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Xボタン入力時
	void GamePlayerFailedState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}
}
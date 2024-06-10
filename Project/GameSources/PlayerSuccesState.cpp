/*!
@file PlayerSuccesState.cpp
@brief プレイヤーのゲームクリア状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<PlayerSuccesState> PlayerSuccesState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<PlayerSuccesState> instance(new PlayerSuccesState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring PlayerSuccesState::GetStateName() const
	{
		return L"クリア状態ステート";
	}

	// ステート開始時の処理
	void PlayerSuccesState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// アニメーションの変更
		if (!player->IsAnimation(ePAKey::Walking))
		{
			player->SetAnimationMesh(ePAKey::Walking);
		}

		// ゴール時の座標を設定
		player->m_goalPosition = player->GetPosition();
	}

	// ステート更新時の処理
	void PlayerSuccesState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// 歩くアニメーションなら
		if (player->IsAnimation(ePAKey::Walking))
		{
			// ゴール時の座標から演出用の座標に移動
			Vec3 pos = Utility::Lerp(player->m_goalPosition, player->m_goalStagingPosition, m_totalTime);
			m_totalTime += DELTA_TIME;
			m_totalTime = min(m_totalTime, 1.0f);

			// 経過時間が1.0以上ならクリア時のアニメーションに切り替える
			if (m_totalTime >= 1.0f) player->SetAnimationMesh(ePAKey::GameSucces);

			// 座標の更新
			player->SetPosition(pos);
		}

		// アニメーションを更新
		player->UpdateAnimation();
	}

	// ステート終了時の処理
	void PlayerSuccesState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Aボタン入力時
	void PlayerSuccesState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Bボタン入力時
	void PlayerSuccesState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Yボタン入力時
	void PlayerSuccesState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Xボタン入力時
	void PlayerSuccesState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}
}
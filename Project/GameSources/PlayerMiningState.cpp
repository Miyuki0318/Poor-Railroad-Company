/*!
@file PlayerMiningState.cpp
@brief プレイヤーの採掘状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<PlayerMiningState> PlayerMiningState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<PlayerMiningState> instance(new PlayerMiningState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring PlayerMiningState::GetStateName() const
	{
		return L"採掘状態ステート";
	}

	// ステート開始時の処理
	void PlayerMiningState::Enter(const shared_ptr<Player>& player)
	{
		// アニメーションの変更
		player->m_ptrDraw->ChangeCurrentAnimation(L"MINING");
	}

	// ステート更新時の処理
	void PlayerMiningState::Execute(const shared_ptr<Player>& player)
	{
		// アニメーション更新
		player->m_ptrDraw->UpdateAnimation(DELTA_TIME * 5.0f);

		// 採掘中の待機時間
		if (player->m_ptrDraw->IsTargetAnimeEnd())
		{
			player->m_status.Set(ePlayerStatus::IsMining) = false;

			player->SetState(PlayerIdleState::Instance());
		}
	}

	// ステート終了時の処理
	void PlayerMiningState::Exit(const shared_ptr<Player>& player)
	{
		// 今のところ何もしない
	}

	// Aボタン入力時
	void PlayerMiningState::OnPushA(const shared_ptr<Player>& player)
	{
		// 今のところ何もしない
	}

	// Xボタン入力時
	void PlayerMiningState::OnPushX(const shared_ptr<Player>& player)
	{
		// 今のところ何もしない
	}
}
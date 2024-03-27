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
		return L"Mining";
	}

	// ステート開始時の処理
	void PlayerMiningState::Enter(const shared_ptr<Player>& player)
	{
		// 今のところ何もしない
	}

	// ステート更新時の処理
	void PlayerMiningState::Execute(const shared_ptr<Player>& player)
	{
		// 採掘状態が解除されたので移動ステートに遷移
		if (!player->GetStatus(ePlayerStatus::IsMining)) 
		{
			player->ChangeState(PlayerMovingState::Instance());
		}

		// 採掘状態の待機時間を送る
		player->MiningWaiting();
	}

	// ステート終了時の処理
	void PlayerMiningState::Exit(const shared_ptr<Player>& player)
	{
		// 今のところ何もしない
	}
}
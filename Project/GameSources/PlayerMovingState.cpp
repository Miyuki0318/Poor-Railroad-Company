/*!
@file PlayerMovingState.cpp
@brief プレイヤーの移動状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<PlayerMovingState> PlayerMovingState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<PlayerMovingState> instance(new PlayerMovingState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring PlayerMovingState::GetStateName() const
	{
		return L"Moving";
	}

	// ステート開始時の処理
	void PlayerMovingState::Enter(const shared_ptr<Player>& player)
	{
		// 今のところ何もしない
	}

	// ステート更新時の処理
	void PlayerMovingState::Execute(const shared_ptr<Player>& player)
	{
		// 採掘状態なら採掘ステートに遷移
		if (player->GetStatus(ePlayerStatus::IsMining)) player->ChangeState(PlayerMiningState::Instance());

		// クラフト状態ならクラフトステートに遷移
		if (player->GetStatus(ePlayerStatus::IsCrafting)) player->ChangeState(PlayerCraftingState::Instance());

		// Aボタン入力があれば採掘、またはレールの設置を行う
		if (Input::GetPushA()) player->MovingPushA();

		// Xボタン入力があればクラフトを行う
		if (Input::GetPushX()) player->CraftingPushX();

		// 移動更新を送る
		player->UpdateMove();
	}

	// ステート終了時の処理
	void PlayerMovingState::Exit(const shared_ptr<Player>& player) 
	{
		// 今のところ何もしない
	}
}
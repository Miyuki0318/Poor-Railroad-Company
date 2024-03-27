/*!
@file PlayerCraftingState.cpp
@brief プレイヤーのクラフト状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<PlayerCraftingState> PlayerCraftingState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<PlayerCraftingState> instance(new PlayerCraftingState);

		// 新しく生成されたthisポインタ
		return instance;
	}


	// ステート名取得
	wstring PlayerCraftingState::GetStateName() const
	{
		return L"Crafting";
	}

	// ステート開始時の処理
	void PlayerCraftingState::Enter(const shared_ptr<Player>& player)
	{
		// 今のところ何もしない
	}

	// ステート更新時の処理
	void PlayerCraftingState::Execute(const shared_ptr<Player>& player)
	{
		// クラフト状態が解除されたので移動ステートに遷移
		if (!player->GetStatus(ePlayerStatus::IsCrafting))
		{
			player->ChangeState(PlayerMovingState::Instance());
		}

		// クラフトQTE状態ならQTEが終わったかどうかの確認処理を送る
		if (player->GetStatus(ePlayerStatus::IsCraftQTE))
		{
			player->CheckedCraftQTE();
		}

		// Aボタン入力があればクラフト時のAボタン入力処理を送る
		if (Input::GetPushA()) player->CraftingPushA();

		// Xボタン入力があればクラフト時のXボタン入力処理を送る
		if (Input::GetPushX()) player->CraftingPushX();
	}

	// ステート終了時の処理
	void PlayerCraftingState::Exit(const shared_ptr<Player>& player)
	{
		// 今のところ何もしない
	}
}
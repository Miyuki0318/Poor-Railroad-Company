/*!
@file PlayerIdleState.cpp
@brief プレイヤーの移動状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<PlayerIdleState> PlayerIdleState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<PlayerIdleState> instance(new PlayerIdleState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring PlayerIdleState::GetStateName() const
	{
		return L"待機状態ステート";
	}

	// ステート開始時の処理
	void PlayerIdleState::Enter(const shared_ptr<Player>& player)
	{
		// 移動状態なら移動ステートに遷移
		if (player->GetStatus(ePlayerStatus::IsMove)) player->SetState(PlayerMovingState::Instance());
	}

	// ステート更新時の処理
	void PlayerIdleState::Execute(const shared_ptr<Player>& player)
	{
		// Lスティック入力があれば移動ステートに遷移
		if (Input::IsInputLStick()) player->SetState(PlayerMovingState::Instance());

		// 採掘状態なら採掘ステートに遷移
		if (player->GetStatus(ePlayerStatus::IsMining)) player->SetState(PlayerMiningState::Instance());

		// クラフト状態ならクラフトステートに遷移
		if (player->GetStatus(ePlayerStatus::IsCrafting)) player->SetState(PlayerCraftingState::Instance());
	}

	// ステート終了時の処理
	void PlayerIdleState::Exit(const shared_ptr<Player>& player)
	{
		// 今のところ何もしない
	}

	// Aボタン入力時
	void PlayerIdleState::OnPushA(const shared_ptr<Player>& player)
	{
		// インディケーターに応じた処理を実行
		player->IndicatorOrder();
	}

	// Xボタン入力時
	void PlayerIdleState::OnPushX(const shared_ptr<Player>& player)
	{
		// クラフト画面を切り替える
		player->SwitchCraftWindow();
	}
}
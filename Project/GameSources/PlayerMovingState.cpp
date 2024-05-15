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
		return L"移動状態ステート";
	}

	// ステート開始時の処理
	void PlayerMovingState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// アニメーションの変更
		if (!player->IsAnimation(ePAKey::Walk))
		{
			player->SetAnimationMesh(ePAKey::Walk);
		}
	}

	// ステート更新時の処理
	void PlayerMovingState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// 待機状態なら移動ステートに遷移
		if (player->GetStatus(ePlayerStatus::IsIdle)) player->SetState(PlayerIdleState::Instance());

		// 採掘状態なら採掘ステートに遷移
		if (player->GetStatus(ePlayerStatus::IsMining)) player->SetState(PlayerMiningState::Instance());

		// クラフト状態ならクラフトステートに遷移
		if (player->GetStatus(ePlayerStatus::IsCrafting)) player->SetState(PlayerCraftingState::Instance());

		// アニメーション更新
		player->m_ptrDraw->UpdateAnimation(DELTA_TIME * 0.75f);

		// 移動更新を送る
		player->UpdateMove();
		player->UpdateRotation();
	}

	// ステート終了時の処理
	void PlayerMovingState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Aボタン入力時
	void PlayerMovingState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// インディケーターに応じた処理を実行
		player->IndicatorOrder();
	}

	// Xボタン入力時
	void PlayerMovingState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// クラフト画面を切り替える
		player->SwitchCraftWindow();
	}
}
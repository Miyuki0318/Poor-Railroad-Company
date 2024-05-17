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
	void PlayerIdleState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// 移動状態なら移動ステートに遷移
		if (Input::IsInputLStick()) player->m_status.Set(ePlayerStatus::IsMove);

		// アニメーションの変更
		if (!player->IsAnimation(ePAKey::Waiting))
		{
			player->SetAnimationMesh(ePAKey::Waiting);
		}
	}

	// ステート更新時の処理
	void PlayerIdleState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// Lスティック入力があれば移動ステートに遷移
		if (Input::IsInputLStick() || !player->GetStatus(ePlayerStatus::IsIdle)) player->SetState(PlayerMovingState::Instance());

		// 採掘状態なら採掘ステートに遷移
		if (player->GetStatus(ePlayerStatus::IsMining)) player->SetState(PlayerMiningState::Instance());

		// クラフト状態ならクラフトステートに遷移
		if (player->GetStatus(ePlayerStatus::IsCrafting)) player->SetState(PlayerCraftingState::Instance());

		player->m_ptrDraw->UpdateAnimation(DELTA_TIME * 0.75f);

		Vec3 pos = player->GetPosition();
		player->GridHitResponse(pos);
		player->SetPosition(pos);
	}

	// ステート終了時の処理
	void PlayerIdleState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Aボタン入力時
	void PlayerIdleState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// インディケーターに応じた処理を実行
		player->IndicatorOrder();
	}

	// Xボタン入力時
	void PlayerIdleState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// クラフト画面を切り替える
		player->SwitchCraftWindow();
	}
}
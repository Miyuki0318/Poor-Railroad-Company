/*!
@file PlayerFailedState.cpp
@brief プレイヤーのゲーム失敗状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<PlayerFailedState> PlayerFailedState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<PlayerFailedState> instance(new PlayerFailedState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring PlayerFailedState::GetStateName() const
	{
		return L"失敗状態ステート";
	}

	// ステート開始時の処理
	void PlayerFailedState::Enter(const shared_ptr<GamePlayer>& player)
	{
		if (!player->IsAnimation(ePAKey::GameFailed))
		{
			player->SetAnimationMesh(ePAKey::GameFailed);
		}

		if (player->GetStatus(ePlayerStatus::IsCraftQTE))
		{
			player->m_craft->DestroyCraftQTE();
			player->m_status.Set(ePlayerStatus::IsCraftQTE) = false;
		}

		if (player->GetStatus(ePlayerStatus::IsCrafting))
		{
			player->SwitchCraftWindow();
		}
	}

	// ステート更新時の処理
	void PlayerFailedState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// アニメーションを更新
		player->UpdateAnimation(player->m_animationMap.at(ePAKey::GameFailed).animeSpeed);
	}

	// ステート終了時の処理
	void PlayerFailedState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Aボタン入力時
	void PlayerFailedState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Bボタン入力時
	void PlayerFailedState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Yボタン入力時
	void PlayerFailedState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Xボタン入力時
	void PlayerFailedState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}
}
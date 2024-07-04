/*!
@file GamePlayerFailedState.cpp
@brief プレイヤーのゲーム失敗状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "GamePlayerFailedState.h"
#include "Player.h"

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
		if (!player->IsAnimation(ePAK::GameFailed))
		{
			player->SetAnimationMesh(ePAK::GameFailed);
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
	void GamePlayerFailedState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// アニメーションを更新
		player->UpdateAnimation(player->m_animationMap.at(ePAK::GameFailed).animeSpeed);
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
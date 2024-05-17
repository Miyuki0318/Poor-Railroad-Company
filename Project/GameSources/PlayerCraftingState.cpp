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
		return L"クラフト状態ステート";
	}

	// ステート開始時の処理
	void PlayerCraftingState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
		player->SetAnimationMesh(ePAKey::CraftStart);
	}

	// ステート更新時の処理
	void PlayerCraftingState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// クラフト状態が解除されたので移動ステートに遷移
		if (player->IsAnimation(ePAKey::CraftFinish) && player->m_ptrDraw->IsTargetAnimeEnd())
		{
			player->SetState(PlayerIdleState::Instance());
		}

		// クラフトQTE状態ならQTEが終わったかどうかの確認処理を送る
		if (player->GetStatus(ePlayerStatus::IsCraftQTE))
		{
			player->CheckedCraftQTE();
		}

		// QTEのアニメーションが終了していたら
		if (player->IsAnimeEnd(ePAKey::QTESucces) || player->IsAnimeEnd(ePAKey::QTEFailed))
		{
			player->SetAnimationMesh(ePAKey::CraftStart, player->m_animationMap.at(ePAKey::CraftFinish).flameNum * DELTA_TIME);
		}

		player->UpdateAnimation();

		//// Aボタン入力があればクラフト時のAボタン入力処理を送る
		//if (Input::GetPushA()) player->CraftingPushA();

		//// Xボタン入力があればクラフト時のXボタン入力処理を送る
		//if (Input::GetPushX()) player->SwitchCraftWindow();
	}

	// ステート終了時の処理
	void PlayerCraftingState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Aボタン入力時
	void PlayerCraftingState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// クラフトウィンドウが表示済みで、QTE中じゃなければ
		if (player->m_craft->GetShowCraftWindow() && !player->m_status(ePlayerStatus::IsCraftQTE))
		{
			// クラフト命令を送り、クラフト可能であればtrue
			if (player->m_craft->CraftOrder())
			{
				// QTE状態をオンにし、QTEを開始させる
				player->m_status.Set(ePlayerStatus::IsCraftQTE) = true;
				player->m_craft->StartQTE();
				player->SetAnimationMesh(ePAKey::Crafting);
			}
			return;
		}

		// クラフトQTE
		if (player->m_status(ePlayerStatus::IsCraftQTE))
		{
			// クラフトマネージャにQTEのバーの停止を送る
			bool succes = player->m_craft->StopQTE();
			player->SetAnimationMesh(succes ? ePAKey::QTESucces : ePAKey::QTEFailed);
			player->m_status.Set(ePlayerStatus::IsCraftQTE) = false;
		}
	}

	// Xボタン入力時
	void PlayerCraftingState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// QTE状態じゃなければクラフト画面を切り替える
		if (!player->m_status(ePlayerStatus::IsCraftQTE))
		{
			player->SwitchCraftWindow();
			player->SetAnimationMesh(ePAKey::CraftFinish);
		}
	}
}
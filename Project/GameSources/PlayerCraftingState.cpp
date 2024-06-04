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
			CheckedCraftQTE(player);
		}

		// QTEのアニメーションが終了していたら
		if (player->IsAnimeEnd(ePAKey::QTESucces) || player->IsAnimeEnd(ePAKey::QTEFailed))
		{
			player->SetAnimationMesh(ePAKey::CraftStart, player->m_animationMap.at(ePAKey::CraftFinish).flameNum * DELTA_TIME);
		}

		// アニメーションの更新
		player->UpdateAnimation();

		// クラフト開始できるかの真偽
		m_isStartCraft = player->m_craft->GetShowCraftWindow() && !player->m_status(ePlayerStatus::IsCraftQTE);
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
		if (m_isStartCraft)
		{
			// クラフト命令を送り、クラフト可能であればtrue
			if (player->m_craft->CraftOrder(eCraftItem::WoodBridge))
			{
				// QTE状態をオンにし、QTEを開始させる
				m_pastInput = eCurrentCraftInput::PushStartA;
				player->m_status.Set(ePlayerStatus::IsCraftQTE) = true;
				player->m_craft->StartQTE(eCraftItem::WoodBridge, eItemType::WoodBridge);
				player->SetAnimationMesh(ePAKey::Crafting);
			}
			return;
		}

		// QTE停止処理
		m_currentInput = eCurrentCraftInput::PushStartA;
		PushedQTE(player);
	}

	// Bボタン入力時
	void PlayerCraftingState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// クラフトウィンドウが表示済みで、QTE中じゃなければ
		if (m_isStartCraft)
		{
			// クラフト命令を送り、クラフト可能であればtrue
			if (player->m_craft->CraftOrder(eCraftItem::Rail))
			{
				// QTE状態をオンにし、QTEを開始させる
				m_pastInput = eCurrentCraftInput::PushStartB;
				player->m_status.Set(ePlayerStatus::IsCraftQTE) = true;
				player->m_craft->StartQTE(eCraftItem::Rail, eItemType::Rail);
				player->SetAnimationMesh(ePAKey::Crafting);
			}
			return;
		}

		// QTE停止処理
		m_currentInput = eCurrentCraftInput::PushStartB;
		PushedQTE(player);
	}

	// Yボタン入力時
	void PlayerCraftingState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		// クラフトウィンドウが表示済みで、QTE中じゃなければ
		if (m_isStartCraft)
		{
			// クラフト命令を送り、クラフト可能であればtrue
			if (player->m_craft->CraftOrder(eCraftItem::Crossing))
			{
				// QTE状態をオンにし、QTEを開始させる
				m_pastInput = eCurrentCraftInput::PushStartY;
				player->m_status.Set(ePlayerStatus::IsCraftQTE) = true;
				player->m_craft->StartQTE(eCraftItem::Crossing, eItemType::Crossing);
				player->SetAnimationMesh(ePAKey::Crafting);
			}
			return;
		}

		// QTE停止処理
		m_currentInput = eCurrentCraftInput::PushStartY;
		PushedQTE(player);
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

	// QTEの停止
	void PlayerCraftingState::PushedQTE(const shared_ptr<GamePlayer>& player)
	{
		// 入力が不一致なら無視
		if (m_currentInput != m_pastInput) return;

		// クラフトQTE
		if (player->m_status(ePlayerStatus::IsCraftQTE))
		{
			// QTE停止時の処理を送る
			StoppedCraftQTE(player);
		}
	}

	// クラフトQTEが終わっているかの確認
	void PlayerCraftingState::CheckedCraftQTE(const shared_ptr<GamePlayer>& player)
	{
		// QTEが終わったら
		if (player->m_craft->GetEndedQTE())
		{
			// QTE停止時の処理を送る
			StoppedCraftQTE(player);
		}
	}

	// QTE停止時の処理
	void PlayerCraftingState::StoppedCraftQTE(const shared_ptr<GamePlayer>& player)
	{
		// 入力保持を初期化
		m_pastInput = eCurrentCraftInput::None;
		m_currentInput = eCurrentCraftInput::None;

		// QtE状態を解除
		player->m_status.Set(ePlayerStatus::IsCraftQTE) = false;

		// QTE終了時の処理を送り、結果に応じてアニメーションを変更
		bool isSucces = player->m_craft->StopQTE();
		player->SetAnimationMesh(isSucces ? ePAKey::QTESucces : ePAKey::QTEFailed);
		player->StartSE(isSucces ? L"C_SUCCES_SE" : L"C_FAILURE_SE", 1.0f);
	}
}
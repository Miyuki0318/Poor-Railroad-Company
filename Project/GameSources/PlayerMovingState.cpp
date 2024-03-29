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
		if (player->GetStatus(ePlayerStatus::IsMining)) player->SetState(PlayerMiningState::Instance());

		// クラフト状態ならクラフトステートに遷移
		if (player->GetStatus(ePlayerStatus::IsCrafting)) player->SetState(PlayerCraftingState::Instance());

		// 移動更新を送る
		player->UpdateMove();
	}

	// ステート終了時の処理
	void PlayerMovingState::Exit(const shared_ptr<Player>& player) 
	{
		// 今のところ何もしない
	}

	// Aボタン入力時
	void PlayerMovingState::OnPushA(const shared_ptr<Player>& player)
	{
		// エラーチェック
		if (!player->m_indicator.lock()) return;

		// 採掘可能か、可能なら採掘可能オブジェクトのポインタを返す
		const auto& miningObj = player->m_indicator.lock()->GetMiningPossible();

		// 採掘可能オブジェクトのポインタがあれば
		if (miningObj)
		{
			// 採掘関数を返す
			player->MiningProcces(miningObj);
			return;
		}

		// レール設置用の座標を設定
		Vec3 railPos = player->m_indicator.lock()->GetPosition();
		railPos.y = 1.0f;

		// レールを設置可能かをインディケーターから取得
		if (player->m_indicator.lock()->GetRailedPossible(railPos))
		{
			// レールを所持してたら設置処理を送る
			if (player->GetItemCount(eItemType::Rail))
			{
				player->AddRailed(railPos);
			}
		}
	}

	// Xボタン入力時
	void PlayerMovingState::OnPushX(const shared_ptr<Player>& player)
	{
		// クラフト画面を切り替える
		player->SwitchCraftWindow();
	}
}
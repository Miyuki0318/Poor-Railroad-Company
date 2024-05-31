/*!
@file PlayerSuccesState.cpp
@brief プレイヤーのゲームクリア状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<PlayerSuccesState> PlayerSuccesState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<PlayerSuccesState> instance(new PlayerSuccesState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring PlayerSuccesState::GetStateName() const
	{
		return L"クリア状態ステート";
	}

	// ステート開始時の処理
	void PlayerSuccesState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// アニメーションの変更
		if (!player->IsAnimation(ePAKey::GameSucces))
		{
			player->SetAnimationMesh(ePAKey::GameSucces);
		}
	}

	// ステート更新時の処理
	void PlayerSuccesState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// アニメーションを更新
		player->UpdateAnimation();
	}

	// ステート終了時の処理
	void PlayerSuccesState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Aボタン入力時
	void PlayerSuccesState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Bボタン入力時
	void PlayerSuccesState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Yボタン入力時
	void PlayerSuccesState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Xボタン入力時
	void PlayerSuccesState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}
}
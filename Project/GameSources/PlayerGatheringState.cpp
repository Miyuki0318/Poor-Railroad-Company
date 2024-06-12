/*!
@file PlayerGatheringState.cpp
@brief プレイヤーの採掘状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<PlayerGatheringState> PlayerGatheringState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<PlayerGatheringState> instance(new PlayerGatheringState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring PlayerGatheringState::GetStateName() const
	{
		return L"採掘状態ステート";
	}

	// ステート開始時の処理
	void PlayerGatheringState::Enter(const shared_ptr<GamePlayer>& player)
	{
		// アニメーションの変更
		player->SetAnimationMesh(ePAKey::Harvesting);
	}

	// ステート更新時の処理
	void PlayerGatheringState::Execute(const shared_ptr<GamePlayer>& player)
	{
		// アニメーション更新
		float animeSpeed = player->m_playerData.at(ePST::GatherSpeed).at(player->m_playerLevel);
		player->UpdateAnimation(animeSpeed);
		player->UpdateRotation();

		// 採掘中の待機時間
		if (!player->m_ptrDraw->IsTargetAnimeEnd()) return;

		// 採掘完了してたらステートを待機に遷移させる
		player->m_status.Set(ePlayerStatus::IsGathering) = false;

		// Aボタン入力があるなら
		if (Input::GetButtonB())
		{
			player->IndicatorOrder();
		}

		// 採取ステータスになっていればアニメーションの切り替え、なってなければステートの切り替え
		player->m_status(ePlayerStatus::IsGathering) ? player->SetAnimationMesh(ePAKey::Harvesting) : player->SetState(PlayerIdleState::Instance());
	}

	// ステート終了時の処理
	void PlayerGatheringState::Exit(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Aボタン入力時
	void PlayerGatheringState::OnPushA(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Bボタン入力時
	void PlayerGatheringState::OnPushB(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Xボタン入力時
	void PlayerGatheringState::OnPushX(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Yボタン入力時
	void PlayerGatheringState::OnPushY(const shared_ptr<GamePlayer>& player)
	{
		// 今のところ何もしない
	}
}
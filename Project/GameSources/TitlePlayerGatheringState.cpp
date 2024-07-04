/*!
@file TitlePlayerGatheringState.cpp
@brief プレイヤーの採掘状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "TitlePlayerGatheringState.h"
#include "TitlePlayerIdleState.h"
#include "Player.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<TitlePlayerGatheringState> TitlePlayerGatheringState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<TitlePlayerGatheringState> instance(new TitlePlayerGatheringState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring TitlePlayerGatheringState::GetStateName() const
	{
		return L"採掘状態ステート";
	}

	// ステート開始時の処理
	void TitlePlayerGatheringState::Enter(const shared_ptr<TitlePlayer>& player)
	{
		// アニメーションの変更
		player->SetAnimationMesh(ePAK::Harvesting);
	}

	// ステート更新時の処理
	void TitlePlayerGatheringState::Execute(const shared_ptr<TitlePlayer>& player)
	{
		// アニメーション更新
		float animeSpeed = player->m_playerData.at(ePST::GatherSpeed).at(player->m_statusLevel);
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
		player->m_status(ePlayerStatus::IsGathering) ? player->SetAnimationMesh(ePAK::Harvesting) : player->SetState(TitlePlayerIdleState::Instance());
	}

	// ステート終了時の処理
	void TitlePlayerGatheringState::Exit(const shared_ptr<TitlePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Bボタン入力時
	void TitlePlayerGatheringState::OnPushB(const shared_ptr<TitlePlayer>& player)
	{
		// 今のところ何もしない
	}
}
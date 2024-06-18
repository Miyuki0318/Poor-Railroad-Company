/*!
@file TitlePlayerMovingState.cpp
@brief プレイヤーの移動状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<TitlePlayerMovingState> TitlePlayerMovingState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<TitlePlayerMovingState> instance(new TitlePlayerMovingState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring TitlePlayerMovingState::GetStateName() const
	{
		return L"移動状態ステート";
	}

	// ステート開始時の処理
	void TitlePlayerMovingState::Enter(const shared_ptr<TitlePlayer>& player)
	{
		// アニメーションの変更
		if (!player->IsAnimation(ePAK::Walking))
		{
			player->SetAnimationMesh(ePAK::Walking);
		}
	}

	// ステート更新時の処理
	void TitlePlayerMovingState::Execute(const shared_ptr<TitlePlayer>& player)
	{
		// 待機状態なら移動ステートに遷移
		if (player->GetStatus(ePlayerStatus::IsIdle)) player->SetState(TitlePlayerIdleState::Instance());

		// 採掘状態なら採掘ステートに遷移
		if (player->GetStatus(ePlayerStatus::IsGathering)) player->SetState(TitlePlayerGatheringState::Instance());

		// 移動更新
		UpdateMoving(player);
	}

	// ステート終了時の処理
	void TitlePlayerMovingState::Exit(const shared_ptr<TitlePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Bボタン入力時
	void TitlePlayerMovingState::OnPushB(const shared_ptr<TitlePlayer>& player)
	{
		// インディケーターに応じた処理を実行
		player->IndicatorOrder();
	}

	// 移動更新
	void TitlePlayerMovingState::UpdateMoving(const shared_ptr<TitlePlayer>& player)
	{
		// 移動更新を送る
		player->m_moveValue = 0.0f;
		player->UpdateMove();
		player->UpdateRotation();

		// アニメーションの更新
		player->m_moveValue = min(floor(Utility::RadToDeg(player->m_moveValue), 1), player->m_maxMove);
		player->UpdateAnimation(player->m_moveValue * player->m_animationMap.at(ePAK::Walking).animeSpeed);
	}
}
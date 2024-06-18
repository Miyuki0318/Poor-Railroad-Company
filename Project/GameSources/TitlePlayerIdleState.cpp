/*!
@file TitlePlayerIdleState.cpp
@brief プレイヤーの待機状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<TitlePlayerIdleState> TitlePlayerIdleState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<TitlePlayerIdleState> instance(new TitlePlayerIdleState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring TitlePlayerIdleState::GetStateName() const
	{
		return L"待機状態ステート";
	}

	// ステート開始時の処理
	void TitlePlayerIdleState::Enter(const shared_ptr<TitlePlayer>& player)
	{
		// 移動状態なら移動ステートに遷移
		if (Input::IsInputLStick()) player->m_status.Set(ePlayerStatus::IsMove);

		// アニメーションの変更
		if (!player->IsAnimation(ePAK::Waiting))
		{
			player->SetAnimationMesh(ePAK::Waiting);
		}
	}

	// ステート更新時の処理
	void TitlePlayerIdleState::Execute(const shared_ptr<TitlePlayer>& player)
	{
		// Lスティック入力があれば移動ステートに遷移
		if (Input::IsInputLStick() || !player->GetStatus(ePlayerStatus::IsIdle)) player->SetState(TitlePlayerMovingState::Instance());

		// 採掘状態なら採掘ステートに遷移
		if (player->GetStatus(ePlayerStatus::IsGathering)) player->SetState(TitlePlayerGatheringState::Instance());

		player->UpdateAnimation(player->m_animationMap.at(ePAK::Waiting).animeSpeed);

		Vec3 pos = player->GetPosition();
		player->GridHitResponse(pos);
		player->SetPosition(pos);
	}

	// ステート終了時の処理
	void TitlePlayerIdleState::Exit(const shared_ptr<TitlePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Bボタン入力時
	void TitlePlayerIdleState::OnPushB(const shared_ptr<TitlePlayer>& player)
	{
		// インディケーターに応じた処理を実行
		player->IndicatorOrder();
	}
}
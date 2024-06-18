/*!
@file TitlePlayerPauseState.cpp
@brief プレイヤーの停止状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<TitlePlayerPauseState> TitlePlayerPauseState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<TitlePlayerPauseState> instance(new TitlePlayerPauseState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring TitlePlayerPauseState::GetStateName() const
	{
		return L"停止状態ステート";
	}

	// ステート開始時の処理
	void TitlePlayerPauseState::Enter(const shared_ptr<TitlePlayer>& player)
	{
		// アニメーションの変更
		player->SetAnimationMesh(ePAK::Waiting);
	}

	// ステート更新時の処理
	void TitlePlayerPauseState::Execute(const shared_ptr<TitlePlayer>& player)
	{
		// アニメーション更新
		player->UpdateAnimation();

		const auto& titleStage = player->GetTypeStage<TitleStage>();

		auto& opening = titleStage->GetSharedGameObject<TitleLogo>(L"TitleLogo");
		if (opening->GetLogoState() == eLogoState::exit && Input::GetPushB())
		{
			player->SetState(TitlePlayerIdleState::Instance());
		}
	}

	// ステート終了時の処理
	void TitlePlayerPauseState::Exit(const shared_ptr<TitlePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Bボタン入力時
	void TitlePlayerPauseState::OnPushB(const shared_ptr<TitlePlayer>& player)
	{
		// 今のところ何もしない
	}
}
/*!
@file TitlePlayerPauseState.cpp
@brief プレイヤーの停止状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "TitlePlayerPauseState.h"
#include "TitlePlayerIdleState.h"
#include "TitlePlayerStartState.h"
#include "TitleTrain.h"
#include "TitleStage.h"
#include "Opening.h"
#include "MainCamera.h"
#include "Company.h"

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
		
		// 列車の後ろの位置を保持
		if (auto& train = player->GetStage()->GetSharedGameObject<TitleTrain>(L"TitleTrain", false))
		{
			m_trainBackPosition = train->GetPosition() + BACK_VEC;
		}
	}

	// ステート更新時の処理
	void TitlePlayerPauseState::Execute(const shared_ptr<TitlePlayer>& player)
	{
		// アニメーション更新
		player->UpdateAnimation();

		// タイトルステージとステージプログレスとカメラを取得
		const auto& titleStage = player->GetTypeStage<TitleStage>();
		auto& prog = titleStage->GetTitleProgress();
		auto& camera = dynamic_pointer_cast<MainCamera>(titleStage->GetView()->GetTargetCamera());
		
		// 移動後でカメラが固定ならプレイヤー周りのチェックとカメラリセットを送る
		if (prog == eTitleProgress::move && camera->m_cameraState == MainCamera::ZoomedIn)
		{
			prog = eTitleProgress::start;
			const auto& train = titleStage->GetSharedGameObject<TitleTrain>(L"TitleTrain");
			train->AddTag(App::GetApp()->GetScene<Scene>()->GetTagName());
			return;
		}

		// 選択またはズーム中でBボタン入力したら
		if (prog == eTitleProgress::select && Input::GetPushB())
		{
			// 会社オブジェクトを選択してるならズームを終えて元に戻す
			if (dynamic_pointer_cast<Company>(titleStage->GetSelectObject()))
			{
				player->SetState(TitlePlayerIdleState::Instance());
				auto& zoomCamera = dynamic_pointer_cast<MainCamera>(titleStage->GetView()->GetTargetCamera());
				zoomCamera->ZoomEnd();
				return;
			}

			// 列車に移動ステートに切り替えて、移動プログレスにする
			player->SetState(TitlePlayerStartState::Instance());
			prog = eTitleProgress::move;
			return;
		}

		// タイトルプログレスがノーマルなら待機状態ステートに切り替え
		if (prog == normal)
		{
			player->SetState(TitlePlayerIdleState::Instance());

			// BGMが流れていなければ再生
			if (!titleStage->GetBGMItem().lock()) titleStage->StartBGM();

			return;
		}

		// オープニングスプライト中にボタン入力したら待機状態ステートに切り替え
		if (prog != opening) return;
		auto& opening = titleStage->GetSharedGameObject<TitleLogo>(L"TitleLogo");
		if (opening->GetLogoState() == eLogoState::exit && Input::GetPush())
		{
			prog = eTitleProgress::normal;
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
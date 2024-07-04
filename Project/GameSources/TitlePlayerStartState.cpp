/*!
@file TitlePlayerStartState.cpp
@brief プレイヤーの停止状態ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "TitlePlayerStartState.h"
#include "TitlePlayerPauseState.h"
#include "TitleStage.h"
#include "TitleTrain.h"
#include "MainCamera.h"

namespace basecross
{
	// インスタンス生成
	shared_ptr<TitlePlayerStartState> TitlePlayerStartState::Instance()
	{
		// new演算子で自身を生成
		static shared_ptr<TitlePlayerStartState> instance(new TitlePlayerStartState);

		// 新しく生成されたthisポインタ
		return instance;
	}

	// ステート名取得
	wstring TitlePlayerStartState::GetStateName() const
	{
		return L"開始状態ステート";
	}

	// ステート開始時の処理
	void TitlePlayerStartState::Enter(const shared_ptr<TitlePlayer>& player)
	{
		// アニメーションの変更
		player->SetAnimationMesh(ePAK::Walking);

		// 列車の座標を元に、線形補間に使う座標を保持
		const auto& train = player->GetStage()->GetSharedGameObject<TitleTrain>(L"TitleTrain");
		m_startPosition = player->GetPosition();
		m_endPosition = (train->GetPosition() + BACK_VEC);
		m_endPosition.y = m_startPosition.y;

		// 回転先を設定
		float rotY = atan2f(m_endPosition.z - m_startPosition.z, m_endPosition.x - m_startPosition.x);
		player->m_rotTarget = Vec3(cos(rotY), 0.0f, sin(-rotY));

		m_totalTime = 0.0f;
	}

	// ステート更新時の処理
	void TitlePlayerStartState::Execute(const shared_ptr<TitlePlayer>& player)
	{
		// アニメーションと回転の更新
		player->UpdateAnimation();
		player->UpdateRotation();

		// 線形補間で移動
		Vec3 pos = Utility::Lerp(m_startPosition, m_endPosition, m_totalTime / m_moveTime);
		m_totalTime += DELTA_TIME;
		m_totalTime = min(m_totalTime, m_moveTime);

		// 移動時間に達したなら
		if (m_totalTime >= m_moveTime)
		{
			// 停止状態ステートに切り替え
			player->SetState(TitlePlayerPauseState::Instance());

			// カメラのズームを終了
			const auto& titleStage = player->GetTypeStage<TitleStage>();
			auto& zoomCamera = dynamic_pointer_cast<MainCamera>(titleStage->GetView()->GetTargetCamera());
			const auto& train = titleStage->GetSharedGameObject<TitleTrain>(L"TitleTrain");
			zoomCamera->ZoomStart(m_startPosition, m_endPosition);
		}

		// 座標を更新
		player->SetPosition(pos);
	}

	// ステート終了時の処理
	void TitlePlayerStartState::Exit(const shared_ptr<TitlePlayer>& player)
	{
		// 今のところ何もしない
	}

	// Bボタン入力時
	void TitlePlayerStartState::OnPushB(const shared_ptr<TitlePlayer>& player)
	{
		// 今のところ何もしない
	}
}
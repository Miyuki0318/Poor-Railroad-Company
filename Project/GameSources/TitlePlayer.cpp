/*!
@file TitlePlayer.cpp
@brief タイトル用プレイヤーオブジェクト
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Input;
	using namespace Utility;

	// 生成時の処理
	void TitlePlayer::OnCreate()
	{
		// 継承元の生成時の処理を実行
		Player::OnCreate();

		// ステートマシンの初期化
		m_playerState.reset(new TitlePlayerStateMachine(GetThis<TitlePlayer>()));
		m_playerState->SetState(TitlePlayerPauseState::Instance());
	}

	// 毎フレーム更新処理
	void TitlePlayer::OnUpdate()
	{
		// ステートマシンの更新処理を送る
		m_playerState->Update();

		// ステートマシンにBボタン入力時の処理を送る
		if (GetPushB()) m_playerState->PushB();

		// デバック用文字列
		Debug::Log(L"プレイヤーの座標 : ", GetPosition());
		Debug::Log(L"プレイヤーのステート : ", m_playerState->GetCurrentState()->GetStateName());
	}

	// プレイヤーに付加する機能の生成
	void TitlePlayer::CreatePlayerFeatures()
	{
		// ステージの取得(shared_ptrをconstで取得)
		const auto& stagePtr = GetStage();

		// thisスマートポインタ
		const auto& thisPtr = GetThis<TemplateObject>();

		// セレクトインディケーターの生成
		m_indicator = stagePtr->AddGameObject<SelectIndicator>(thisPtr);

		// アクションガイドの生成
		stagePtr->AddGameObject<ActionGuide>(GetThis<TitlePlayer>(), m_indicator.lock());
	}

	// インディケーターの取得に応じて処理
	void TitlePlayer::IndicatorOrder()
	{
		// エラーチェック
		const auto& indicator = m_indicator.lock();
		if (!indicator) return;

		// インディケーターの方向に向く
		if (GetButtonRB()) SetRotateIndicatorAngle();

		// 採掘命令
		GatheringOrder(indicator);
	}

	// 移動更新
	void TitlePlayer::UpdateMove()
	{
		// LStickの入力があるなら
		if (IsInputLStick())
		{
			// LStick入力量の取得
			Vec3 stickValue = Vec3(GetLStickValue().x, 0.0f, GetLStickValue().y);

			m_acsel += DELTA_TIME;
			m_acsel = min(m_acsel, m_maxAcsel);
			SetRotateTarget(stickValue); // 回転関数
			ControllerMovement(stickValue);	// 移動関数
		}
	}
}
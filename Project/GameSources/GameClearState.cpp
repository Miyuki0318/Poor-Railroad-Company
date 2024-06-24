/*!
@file GameClearState.cpp
@brief ゲームクリア時の処理ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// 生成時の処理
	void GameClearState::CreateState()
	{
		// ステージの取得
		const auto& stagePtr = m_stage.lock();

		// ゲームクリア時に扱うスプライト
		m_nextStageSprite = stagePtr->AddGameObject<Sprite>(L"NEXTSTAGE_TX", m_defScale, m_leftPos - m_moveVal);
		m_clearBackSprite = stagePtr->AddGameObject<Sprite>(L"CLEAR_TITLEBACK_TX", m_defScale, m_rightPos - m_moveVal);
		m_railLineSprite = stagePtr->AddGameObject<Sprite>(L"RAIL_LINE_TX", Vec2(WINDOW_WIDTH, 128.0f), m_railPos);

		// 選択肢マップに追加
		m_selectSprite.emplace(eSelectStage::NextStage, m_nextStageSprite);
		m_selectSprite.emplace(eSelectStage::TitleBack, m_clearBackSprite);
	}

	// 更新処理
	void GameClearState::UpdateState()
	{
		// ステートに応じた関数呼び出し
		m_stateFunc.at(m_clearState)();
	}

	// 初期化処理
	void GameClearState::ResetState()
	{
		m_totalTime = 0.0f;
		m_clearState = eGameClearState::RailFadeIn;
		
		// 生成時の座標に戻す
		m_nextStageSprite.lock()->SetPosition(m_leftPos - m_moveVal);
		m_clearBackSprite.lock()->SetPosition(m_rightPos - m_moveVal);
		m_railLineSprite.lock()->SetPosition(m_railPos);
	}

	// レールのフェードイン
	void GameClearState::RailSpriteFadeIn()
	{
		// 線形補間で移動
		Vec3 pos = Utility::Lerp(m_railPos, m_railPos + m_moveVal, m_totalTime / m_railFadeTime);

		// フェード時間を過ぎたらステートを切り替え
		if (m_totalTime >= m_railFadeTime)
		{
			m_clearState = eGameClearState::SelectFadeIn;
			m_totalTime = 0.0f;
		}

		// 経過時間をデルタタイムで加算し、座標を更新
		m_totalTime += DELTA_TIME;
		m_railLineSprite.lock()->SetPosition(pos);
	}

	// 選択肢スプライトのフェードイン
	void GameClearState::SelectSpriteFadeIn()
	{
		// イージング処理で移動
		Easing<Vec3> moveEasing;
		Vec3 move = moveEasing.EaseOut(EasingType::Cubic, m_moveVal, Vec3(0.0f), m_totalTime, m_selectFadeTime);

		// フェード時間を過ぎたらステートを切り替え
		if (m_totalTime >= m_selectFadeTime)
		{
			m_clearState = eGameClearState::SelectStage;
			m_totalTime = -XM_PIDIV4;
		}
		
		// 経過時間をデルタタイムで加算し、座標を更新
		m_totalTime += DELTA_TIME;
		m_nextStageSprite.lock()->SetPosition(m_leftPos - move);
		m_clearBackSprite.lock()->SetPosition(m_rightPos - move);
	}

	void GameClearState::SelectStageState()
	{
		// LStick入力
		float stickVal = Input::GetLStickValue().x;

		// 経過時間(0.0f～XM_2PI)
		m_totalTime += DELTA_TIME * 2.0f;
		if (m_totalTime >= XM_2PI) m_totalTime = 0.0f;

		// 前回は未入力で、現在で入力があれば
		if (stickVal && !m_pastStick)
		{
			// 経過時間をリセットし、選択肢を切り替える
			m_totalTime = -XM_PIDIV4;
			m_pastSelect = m_selectStage;
			switch (m_selectStage)
			{
			case eSelectStage::TitleBack:
				m_selectStage = eSelectStage::NextStage;
				break;

			case eSelectStage::NextStage:
				m_selectStage = eSelectStage::TitleBack;
				break;

			default:
				break;
			}
		}

		// 入力の保存
		m_pastStick = stickVal;

		// スケールをサインカーブでバウンド処理
		float scale = Utility::SinCurve(m_totalTime, 1.0f, m_boundScale);
		m_selectSprite.at(m_pastSelect).lock()->SetScale(m_defScale);
		m_selectSprite.at(m_selectStage).lock()->SetScale(m_defScale * scale);

		// Bボタン入力があれば
		if (Input::GetPushB())
		{
			// SEを鳴らして初期化し、ステートを切り替える
			m_totalTime = 0.0f;
			m_stage.lock()->CreateSE(L"WHISTLE_SE", 1.0f);
			m_selectSprite.at(m_selectStage).lock()->SetScale(m_defScale);
			m_clearState = eGameClearState::SelectFadeOut;
		}
	}

	// 選択肢スプライトのフェードアウト
	void GameClearState::SelectSpriteFadeOut()
	{
		// イージング処理で移動
		Easing<Vec3> moveEasing;
		Vec3 move = moveEasing.EaseIn(EasingType::Cubic, Vec3(0.0f), m_moveVal, m_totalTime, m_selectFadeTime);
		
		// フェード時間を過ぎたらステートを切り替え
		if (m_totalTime >= m_selectFadeTime)
		{
			m_clearState = eGameClearState::StandBy;
			m_totalTime = 0.0f;
		}
		
		// 経過時間をデルタタイムで加算し、座標を更新
		m_totalTime += DELTA_TIME;
		m_nextStageSprite.lock()->SetPosition(m_leftPos + move);
		m_clearBackSprite.lock()->SetPosition(m_rightPos + move);
	}
}
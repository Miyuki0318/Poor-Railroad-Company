/*!
@file GameClearState.cpp
@brief ゲームクリア時の処理ステート
@author 小澤博貴
*/

#include "stdafx.h"
#include "GamePlayer.h"
#include "GameClearState.h"
#include "Input.h"

namespace basecross
{
	// ネームスペースの省略
	using namespace Utility;

	// 生成時の処理
	void GameClearState::CreateState()
	{
		// ステージの取得
		const auto& stagePtr = m_stage.lock();

		// ゲームクリア時に扱うスプライト
		m_nextStageSprite = stagePtr->AddGameObject<Sprite>(L"NEXTSTAGE_TX", m_defScale, m_leftPos - m_moveVal);
		m_clearBackSprite = stagePtr->AddGameObject<Sprite>(L"CLEAR_TITLEBACK_TX", m_defScale, m_rightPos - m_moveVal);
		m_railLineSprite = stagePtr->AddGameObject<Sprite>(L"RAIL_LINE_TX", Vec2(WINDOW_WIDTH, 128.0f), m_railPos);

		// レイヤー設定
		m_nextStageSprite.lock()->SetDrawLayer(8);
		m_clearBackSprite.lock()->SetDrawLayer(8);
		m_railLineSprite.lock()->SetDrawLayer(8);

		// 煙のエフェクトスプライトを取得
		m_smoke = stagePtr->GetSharedGameObject<SpriteParticle>(L"SmokeEffect");

		// 選択肢マップに追加
		m_selectSprite.emplace(eSelectGameClear::NextStage, m_nextStageSprite);
		m_selectSprite.emplace(eSelectGameClear::TitleBack, m_clearBackSprite);
	}

	// 更新処理
	void GameClearState::UpdateState()
	{
		// ステートに応じた関数呼び出し
		m_stateFunc.at(m_currentState)();
	}

	// 初期化処理
	void GameClearState::ResetState()
	{
		m_totalTime = 0.0f;
		m_currentState = eGameClearState::RailFadeIn;
		
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
			m_currentState = eGameClearState::SelectFadeIn;
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
			m_currentState = eGameClearState::SelectState;
			m_totalTime = -XM_PIDIV4;
		}
		
		// 経過時間をデルタタイムで加算し、座標を更新
		m_totalTime += DELTA_TIME;
		m_nextStageSprite.lock()->SetPosition(m_leftPos - move);
		m_clearBackSprite.lock()->SetPosition(m_rightPos - move);
	}

	// 選択中
	void GameClearState::SelectStageState()
	{
		// 経過時間(0.0f〜XM_2PI)
		m_totalTime += DELTA_TIME * 2.0f;
		if (m_totalTime >= XM_2PI) m_totalTime = 0.0f;

		// コントローラーの接続に応じて選択処理を行う
		Input::GetPadConected() ? ControllerSelect() : MouseSelect();

		// スケールをサインカーブでバウンド処理
		float scale = SinCurve(m_totalTime, 1.0f, m_boundScale);
		m_selectSprite.at(m_pastSelect).lock()->SetScale(m_defScale);
		m_selectSprite.at(m_currentSelect).lock()->SetScale(m_defScale * scale);

		// Bボタン入力があれば
		if (Input::GetPushB())
		{
			// SEを鳴らして初期化し、ステートを切り替える
			m_totalTime = 0.0f;
			m_stage.lock()->CreateSE(L"WHISTLE_SE", 1.0f);
			m_selectSprite.at(m_currentSelect).lock()->SetScale(m_defScale);
			m_currentState = eGameClearState::SelectFadeOut;
		}
	}

	// コントローラーで選択
	void GameClearState::ControllerSelect()
	{
		// LStick入力
		float stickVal = Input::GetLStickValue().x;

		// 前回は未入力で、現在で入力があれば
		if (stickVal && !m_pastStick)
		{
			// 経過時間をリセットし、選択肢を切り替える
			m_totalTime = -XM_PIDIV4;
			m_pastSelect = m_currentSelect;
			m_stage.lock()->CreateSE(L"SELECT_SE", 0.75f);
			switch (m_currentSelect)
			{
			case eSelectGameClear::TitleBack:
				m_currentSelect = eSelectGameClear::NextStage;
				break;

			case eSelectGameClear::NextStage:
				m_currentSelect = eSelectGameClear::TitleBack;
				break;

			default:
				break;
			}
		}

		// 入力の保存
		m_pastStick = stickVal;
	}

	// マウスでの選択
	void GameClearState::MouseSelect()
	{
		Vec2 mousePos = Input::GetMousePosition();	// マウスの座標
		Vec2 helfScale = m_defScale / 2.0f;			// スケールの半分

		// 選択肢の座標
		Vec2 nextPos = Vec2(m_nextStageSprite.lock()->GetPosition());
		Vec2 backPos = Vec2(m_clearBackSprite.lock()->GetPosition());

		// 次のステージへスプライトの範囲内なら
		if (GetBetween(mousePos, nextPos + helfScale, nextPos - helfScale))
		{
			if (m_currentSelect == eSelectGameClear::NextStage) return;

			// 選択肢を変更
			m_totalTime = -XM_PIDIV4;
			m_currentSelect = eSelectGameClear::NextStage;
			m_pastSelect = eSelectGameClear::TitleBack;
			m_stage.lock()->CreateSE(L"SELECT_SE", 0.75f);
		}

		// タイトルへスプライトの範囲内なら
		if (GetBetween(mousePos, backPos + helfScale, backPos - helfScale))
		{
			if (m_currentSelect == eSelectGameClear::TitleBack) return;

			// 選択肢を変更
			m_totalTime = -XM_PIDIV4;
			m_currentSelect = eSelectGameClear::TitleBack;
			m_pastSelect = eSelectGameClear::NextStage;
			m_stage.lock()->CreateSE(L"SELECT_SE", 0.75f);
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
			m_currentState = eGameClearState::StandBy;
			m_totalTime = 0.0f;
		}
		
		// 経過時間をデルタタイムで加算し、座標を更新
		m_totalTime += DELTA_TIME;
		m_nextStageSprite.lock()->SetPosition(m_leftPos + move);
		m_clearBackSprite.lock()->SetPosition(m_rightPos + move);

		// 煙のエフェクトを追加
		m_smoke.lock()->SetEmitterPosition(m_selectSprite.at(m_currentSelect).lock()->GetPosition() + m_smokeDiff);
		m_smoke.lock()->AddParticle(Vec2(RangeRand(50.0f, 5.0f)), m_smokeVelo, RangeRand(XM_PI, -XM_PI), 0.5f);
	}
}
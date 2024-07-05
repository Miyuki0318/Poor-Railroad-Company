/*!
@file PauseMenu.cpp
@brief ポーズメニュー
@prod 矢吹悠葉
*/

#include "stdafx.h"
#include "PauseMenu.h"
#include "MathFunc.h"
#include "Input.h"
#include "GameStage.h"

namespace basecross
{
	using namespace MathF;

	void PauseMenu::OnCreate()
	{
		const shared_ptr<Stage>& stagePtr = GetStage();

		// 各種スプライトの生成
		m_menuSprite = stagePtr->AddGameObject<Sprite>(L"PAUSEMENU_TX", m_CloseMenuScale, m_CloseMenuPos);
		m_menuSprite.lock()->SetDrawLayer(8);
		m_buttonSprites.emplace(eButtons::Retry, stagePtr->AddGameObject<Sprite>(L"CONTINUE_TX", m_DefaultButtonScale, m_DefaultRetryButtonPos));
		m_buttonSprites.emplace(eButtons::BackTitle, stagePtr->AddGameObject<Sprite>(L"CONTINUE_TITLEBACK_TX", m_DefaultButtonScale, m_DefaultTitleButtonPos));
		m_buttonSprites.at(eButtons::Retry).lock()->SetDrawLayer(9);
		m_buttonSprites.at(eButtons::BackTitle).lock()->SetDrawLayer(9);

		SetDrawActiveButtons(false);
	}

	void PauseMenu::OnUpdate()
	{
		StateProcess(m_state);
	}

	bool PauseMenu::OnOpen()
	{
		if (m_state != State::Closed) return false; // 状態がクローズ後でなければ何もしない
		m_state = State::Open; // オープン状態に移行
		GetTypeStage<GameStage>()->SetGameProgress(eGameProgress::Pause); // ゲーム進行状態をポーズ状態にする
		m_lerpRatio = 0.0f; // 補間割合を0に
		return true;
	}

	bool PauseMenu::OnClose()
	{
		if (m_state != State::Opened) return false; // 状態がオープン後でなければ何もしない

		SetDrawActiveButtons(false); // ボタンを非表示
		m_state = State::Close;	// クローズ状態に移行
		GetTypeStage<GameStage>()->SetGameProgress(eGameProgress::Playing); // ゲーム進行状態をプレイ状態にする
		m_lerpRatio = 1.0f; // 補間割合を1に
		return true;
	}

	void PauseMenu::StateProcess(State state)
	{
		m_lerpRatio = Clamp01(m_lerpRatio); // 補間割合を0~1の範囲に制限する

		// メニュースプライトのトランスフォーム設定
		m_menuSprite.lock()->SetPosition(Utility::Lerp(m_CloseMenuPos, m_OpenMenuPos, m_lerpRatio)); 
		m_menuSprite.lock()->SetScale(Utility::Lerp(m_CloseMenuScale, m_OpenMenuScale, m_lerpRatio));

		if (state == State::Open) // オープン状態の時
		{
			m_lerpRatio += DELTA_TIME * m_LerpSpeed;
			m_state = m_lerpRatio >= 1.0f ? State::Opened : State::Open;
		}
		if (state == State::Close) // クローズ状態の時
		{
			m_lerpRatio -= DELTA_TIME * m_LerpSpeed;
			m_state = m_lerpRatio <= 0.0f ? State::Closed : State::Close;
		}
		if (state == State::Opened) // オープン後状態の時
		{
			SetDrawActiveButtons(true); // ボタンを表示
			ButtonSelect(); 
		}
		if (state == State::Selected) // ボタンセレクト後状態の時
		{
			if (m_currentButton == eButtons::BackTitle) // タイトルに戻るボタンが選ばれていたら
			{
				TitleButton(); // タイトルボタン用処理を行う
				return;
			}

			if (GetTypeStage<GameStage>()->GetFadeIn() && m_currentButton == eButtons::Retry) // フェードインが終わったかつリトライボタンが選ばれていたら
			{
				RetryButton(); // リトライボタン用処理を行う
			}
		}
	}

	void PauseMenu::ButtonSelect()
	{
		float stickVal = Input::GetLStickValue().x; // スティック入力を取得

		m_scaleRatio += DELTA_TIME * m_ScaleSpeed; // ボタンのスケール用
		if (m_scaleRatio >= XM_2PI) m_scaleRatio = 0.0f;

		if (stickVal && !m_pastStickVal) // 入力があるかつ直前にスティック入力が無かったら
		{
			// 選択中のボタンを変更する
			m_scaleRatio = 0.0f;
			m_pastButton = m_currentButton;
			switch (m_currentButton)
			{
			case eButtons::Retry:
				m_currentButton = eButtons::BackTitle;
				break;

			case eButtons::BackTitle:
				m_currentButton = eButtons::Retry;
				break;

			default:
				break;
			}
		}

		m_pastStickVal = stickVal;

		// 選択中のボタンのみスケールをサインカーブで変更する
		float scale = Utility::SinCurve(m_scaleRatio, m_MinScaleRatio, m_MaxScaleRatio); 
		m_buttonSprites.at(m_pastButton).lock()->SetScale(m_DefaultButtonScale);
		m_buttonSprites.at(m_currentButton).lock()->SetScale(m_DefaultButtonScale * scale);

		if (Input::GetPushB()) m_state = State::Selected; // Bボタンが押されたらSelected状態に移行
	}

	void PauseMenu::TitleButton()
	{
		GetTypeStage<GameStage>()->SetGameProgress(eGameProgress::ToTitleClear); // ゲーム進行状態をタイトルに戻る状態に移行
	}

	void PauseMenu::RetryButton()
	{
		auto& gameStage = GetTypeStage<GameStage>();
		gameStage->ResetCreateStage();	// ステージをリセット
		gameStage->SetGameProgress(eGameProgress::FadeIn); // ゲーム進行状態をプレイ状態に移行
		SetDrawActiveButtons(false); // ボタンを非表示
		m_state = State::Close; // クローズ状態に移行
	}

	void PauseMenu::SetDrawActiveButtons(bool drawFlag)
	{
		// ボタンの数だけSetDrawActiveを行う
		for (int i = 0; i < eButtons::ButtonNum; i++)
		{
			m_buttonSprites.at((eButtons)i).lock()->SetDrawActive(drawFlag);
		}
	}
}
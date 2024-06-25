#include "stdafx.h"
#include "Project.h"
#include "PauseMenu.h"

namespace basecross
{
	using namespace MathF;

	void PauseMenu::OnCreate()
	{
		const shared_ptr<Stage>& stagePtr = GetStage();

		m_menuSprite = stagePtr->AddGameObject<Sprite>(L"PAUSEMENU_TX", m_CloseMenuScale, m_CloseMenuPos);
		m_retrySprite = stagePtr->AddGameObject<Sprite>(L"CONTINUE_TX", m_DefaultButtonScale, m_DefaultRetryButtonPos);
		m_titleBackSprite = stagePtr->AddGameObject<Sprite>(L"TITLEBACK_TX", m_DefaultButtonScale, m_DefaultTitleButtonPos);
		m_retrySprite.lock()->SetDrawActive(false);
		m_titleBackSprite.lock()->SetDrawActive(false);
		m_buttonSprites.emplace(eButtons::Retry, m_retrySprite);
		m_buttonSprites.emplace(eButtons::BackTitle, m_titleBackSprite);
	}

	void PauseMenu::OnUpdate()
	{
		StateProcess(m_state);
	}

	void PauseMenu::OnOpen()
	{
		if (m_state != State::Closed) return;
		m_state = State::Open;
		m_lerpRatio = 0.0f;
	}

	void PauseMenu::OnClose()
	{
		if (m_state != State::Opened) return;
		m_retrySprite.lock()->SetDrawActive(false);
		m_titleBackSprite.lock()->SetDrawActive(false);

		m_state = State::Close;
		m_lerpRatio = 1.0f;
	}

	void PauseMenu::StateProcess(State state)
	{
		m_lerpRatio = Clamp01(m_lerpRatio);
		m_menuSprite.lock()->SetPosition(Utility::Lerp(m_CloseMenuPos, m_OpenMenuPos, m_lerpRatio));
		m_menuSprite.lock()->SetScale(Utility::Lerp(m_CloseMenuScale, m_OpenMenuScale, m_lerpRatio));

		if (state == State::Open)
		{
			m_lerpRatio += DELTA_TIME * m_LerpSpeed;
			m_state = m_lerpRatio >= 1.0f ? State::Opened : State::Open;
		}
		if (state == State::Close)
		{
			m_lerpRatio -= DELTA_TIME * m_LerpSpeed;
			m_state = m_lerpRatio <= 0.0f ? State::Closed : State::Close;
		}
		if (state == State::Opened)
		{
			m_retrySprite.lock()->SetDrawActive(true);
			m_titleBackSprite.lock()->SetDrawActive(true);
			ButtonSelect();
		}
	}

	void PauseMenu::ButtonSelect()
	{
		float stickVal = Input::GetLStickValue().x;

		m_scaleRatio += DELTA_TIME * m_ScaleSpeed;
		if (m_scaleRatio >= XM_2PI) m_scaleRatio = 0.0f;

		if (stickVal && !m_pastStickVal)
		{
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
		float scale = Utility::SinCurve(m_scaleRatio, m_MinScaleRatio, m_MaxScaleRatio);
		m_buttonSprites.at(m_pastButton).lock()->SetScale(m_DefaultButtonScale);
		m_buttonSprites.at(m_currentButton).lock()->SetScale(m_DefaultButtonScale * scale);

		if (Input::GetPushB()) ButtonAction();
	}

	void PauseMenu::ButtonAction()
	{
		if (m_currentButton == eButtons::BackTitle)
		{
			GetTypeStage<GameStage>()->PostEvent(0.0f, GetTypeStage<GameStage>(), App::GetApp()->GetScene<Scene>(), L"TitleStage");
		}
		if (m_currentButton == eButtons::Retry)
		{
			GetTypeStage<GameStage>()->ResetState();
			m_retrySprite.lock()->SetDrawActive(false);
			m_titleBackSprite.lock()->SetDrawActive(false);
			m_state = Close;
		}
	}
}
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
		//m_retrySprite = stagePtr->AddGameObject<Sprite>(L"CONTINUE_TX", m_DefaultButtonScale, m_DefaultRetryButtonPos);
		//m_titleBackSprite = stagePtr->AddGameObject<Sprite>(L"TITLEBACK_TX", m_DefaultButtonScale, m_DefaultTitleButtonPos);
		//m_retrySprite.lock()->SetDrawActive(false);
		//m_titleBackSprite.lock()->SetDrawActive(false);
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
		//m_retrySprite.lock()->SetDrawActive(false);
		//m_titleBackSprite.lock()->SetDrawActive(false);

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
			//m_retrySprite.lock()->SetDrawActive(true);
			//m_titleBackSprite.lock()->SetDrawActive(true);
		}
	}
}
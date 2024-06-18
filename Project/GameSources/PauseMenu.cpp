#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	using namespace MathF;

	void PauseMenu::OnCreate()
	{
		const shared_ptr<Stage>& stagePtr = GetStage();

		m_menuSprites = stagePtr->AddGameObject<Sprite>(L"PAUSEMENU_TX", m_DefaultScale, m_DefaultPosition);
	}

	void PauseMenu::OnUpdate()
	{
		StateProcess(m_state);
	}

	void PauseMenu::OnOpen()
	{
		m_state = State::Open;
		m_lerpRatio = 0.0f;
	}

	void PauseMenu::OnClose()
	{
		m_state = State::Close;
		m_lerpRatio = 1.0f;
	}

	void PauseMenu::StateProcess(State state)
	{
		m_lerpRatio = Clamp01(m_lerpRatio);
		m_menuSprites.lock()->SetPosition(Utility::Lerp(m_DefaultPosition, m_AfterPosition, m_lerpRatio));
		m_menuSprites.lock()->SetScale(Utility::Lerp(m_DefaultScale, m_AfterScale, m_lerpRatio));

		if (state == State::None)
		{

		}
		if (state == State::Open)
		{
			m_lerpRatio += DELTA_TIME * m_LerpSpeed;
			m_state = m_lerpRatio >= 1.0f ? State::None : State::Open;
		}
		if (state == State::Close)
		{
			m_lerpRatio -= DELTA_TIME * m_LerpSpeed;
			m_state = m_lerpRatio <= 0.0f ? State::None : State::Close;
		}
	}
}
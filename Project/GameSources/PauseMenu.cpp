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
		m_scaleRatio = 0.0f;
	}

	void PauseMenu::OnClose()
	{
		m_state = State::Close;
		m_scaleRatio = 1.0f;
	}

	void PauseMenu::StateProcess(State state)
	{
		m_scaleRatio = Clamp01(m_scaleRatio);
		m_menuSprites.lock()->SetScale(Utility::Lerp(m_DefaultScale, m_AfterScale, m_scaleRatio));

		if (state == State::None)
		{

		}
		if (state == State::Open)
		{
			m_scaleRatio += DELTA_TIME * m_ScaleSpeed;
			m_state = m_scaleRatio >= 1.0f ? State::None : State::Open;
		}
		if (state == State::Close)
		{
			m_scaleRatio -= DELTA_TIME * m_ScaleSpeed;
			m_state = m_scaleRatio <= 0.0f ? State::None : State::Close;
		}
	}
}
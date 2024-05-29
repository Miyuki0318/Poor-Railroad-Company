/*!
@file TitleTrain.cpp
@brief —ñÔ‚ÌÀ‘•
@author –î—I—t
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void TitleTrain::OnCreate()
	{
		m_state = State::None;
		Train::OnCreate();
	}

	void TitleTrain::OnUpdate()
	{
		if (FindTag(tagName))
		{
			m_state = State::GameStart;
		}
		StateProcess(m_state);
	}

	void TitleTrain::StateProcess(State state)
	{
		if (state == State::None) return;

		if (state == State::GameStart)
		{
			GameStartProcess();
		}
	}

	void TitleTrain::GameStartProcess()
	{
		MoveProcess(State::None);

		PostEvent(5.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"GameStage");
	}
}
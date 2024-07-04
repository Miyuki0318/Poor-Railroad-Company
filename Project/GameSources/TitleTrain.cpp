/*!
@file TitleTrain.cpp
@brief —ñÔ‚ÌÀ‘•
@author –î—I—t
*/

#include "stdafx.h"
#include "TitleTrain.h"
#include "TitleStage.h"

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
		const float TIME = 2.0f;

		MoveProcess(State::None);

		float volume = (TIME - GetTime(TIME)) / TIME * 0.5f;
		if (SetTimer(TIME))
		{
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"GameStage");
		}

		auto& bgm = GetTypeStage<TitleStage>()->GetBGMItem();
		if (!bgm.lock()) return;
		bgm.lock()->m_SourceVoice->SetVolume(volume);
	}
}
/*!
@file TitleTrain.cpp
@brief ��Ԃ̎���
@author ��I�t
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	void TitleTrain::OnUpdate()
	{
		auto construction = GetStage()->GetSharedGameObject<Construction>(L"Construction");
		if (construction->MoveTrainFlag())
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
		// ���`��Ԃňړ�
		Vec3 pos = Utility::Lerp(m_movePos.first, m_movePos.second, m_moveRatio);
		m_moveRatio += DELTA_TIME / m_MoveInSeconds;

		// ������1�ȏ�ɂȂ�����0�ŏ�����
		if (m_moveRatio >= 1.0f)
		{
			m_moveRatio = 0.0f;

			// ���̃��[����������A�ݒ�s�Ȃ�ҋ@
			if (!SearchNextRail()) m_state = State::None;

			SetNextRailDirection();
		}

		// ���W�̍X�V
		SetPosition(pos);
	}
}
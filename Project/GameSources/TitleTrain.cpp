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
		// ���`��Ԃňړ�
		Vec3 pos = Utility::Lerp(m_movePos.first, m_movePos.second, m_moveRatio);
		//m_moveRatio += DELTA_TIME * m_MoveSpeed;
		m_moveRatio = MathF::Repeat01(m_moveRatio, m_MoveSpeed, false).value;

		// �͈͊O�ɂȂ�����
		if (MathF::Repeat01(m_moveRatio, m_MoveSpeed, false).outRange)
		{
			// ���̃��[����������A�ݒ�s�Ȃ�ҋ@
			if (!SearchNextRail()) m_state = State::None;

			SetNextRailDirection();
		}

		// ���W�̍X�V
		SetPosition(pos);

		PostEvent(5.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"GameStage");
	}
}
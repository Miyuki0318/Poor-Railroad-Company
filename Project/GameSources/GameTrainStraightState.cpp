/*!
@file GameTrainStraightState.cpp
@brief �Q�[�����̗�Ԃ̒������s���̃X�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	// �C���X�^���X����
	shared_ptr<GameTrainStraightState> GameTrainStraightState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<GameTrainStraightState> instance(new GameTrainStraightState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g�J�n���̏���
	void GameTrainStraightState::Enter(const shared_ptr<GameTrain>& train)
	{
		// ���[���}�b�v�̎擾
		const auto& railMap = train->GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailDataMap();
		if (railMap.empty()) return;

		// ���[����ݒ�
		string line = POS2LINE(railMap.at(train->m_railPos).futurePos);	// 1�惌�[����LINE
		train->m_movePos.first = railMap.at(train->m_railPos).thisPos;	// ���݂̃��[���̍��W
		train->m_movePos.second = railMap.at(line).thisPos;				// 1��̃��[���̍��W
		train->m_railPos = line;										// 1��̃��[����LINE��ێ�

		// �����̏�����
		train->m_moveRatio = 0.0f;
	}

	// �X�e�[�g�X�V���̏���
	void GameTrainStraightState::Execute(const shared_ptr<GameTrain>& train)
	{
		// ���`��Ԃňړ�
		Vec3 pos = Utility::Lerp(train->m_movePos.first, train->m_movePos.second, train->m_moveRatio);
		train->m_moveRatio = MathF::Repeat01(train->m_moveRatio, train->m_MoveSpeed);

		// �͈͊O�ɂȂ�����
		if (MathF::GetOutRange())
		{
			// ���̃��[�����������Ȃ������玟�̃X�e�[�g��
			if (!train->SearchNextRail()) train->m_state = Train::State::Derail;

			train->SetNextRailDirection(); // ���̃��[���̕�����ݒ�
		}

		// ���W�̍X�V
		train->SetPosition(pos);
	}

	// �X�e�[�g�I�����̏���
	void GameTrainStraightState::Exit(const shared_ptr<GameTrain>& train)
	{
		// ���̂Ƃ��뉽�����Ȃ�
	}
}
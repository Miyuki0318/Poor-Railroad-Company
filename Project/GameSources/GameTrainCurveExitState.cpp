/*!
@file GameTrainCurveExitState.cpp
@brief �Q�[�����̗�Ԃ̃J�[�u�I�����̃X�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "GameTrainCurveExitState.h"
#include "MathFunc.h"

namespace basecross
{
	const float HELF_TIME = 0.5f;	// �����̎���
	const float ACSEL = 0.75f;	// �����̎���
	
	// �C���X�^���X����
	shared_ptr<GameTrainCurveExitState> GameTrainCurveExitState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<GameTrainCurveExitState> instance(new GameTrainCurveExitState);
		
		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring GameTrainCurveExitState::GetStateName() const
	{
		return L"GameTrainCurveExitState";
	}

	// �X�e�[�g�J�n���̏���
	void GameTrainCurveExitState::Enter(const shared_ptr<GameTrain>& train)
	{
		// ���[���}�b�v�̎擾
		const auto& railMap = train->GetRailDataMap();
		if (railMap.empty()) return;

		// �J�[�u���̃��[���A�I�����̃��[���̍��W
		Vec3 pointB = railMap.at(POS2LINE(railMap.at(train->m_railPos).futurePos)).thisPos;
		Vec3 pointC = railMap.at(POS2LINE(railMap.at(train->m_railPos).futurePos)).futurePos;
		
		// ���Ԃ̍��W�ƏI�����̃��[���̍��W��ݒ�
		train->m_movePos.first = Utility::Lerp(pointB, pointC, HELF_TIME);
		train->m_movePos.second = pointC;

		// �����̏�����
		train->m_moveRatio = 0.0f;
	}

	// �X�e�[�g�X�V���̏���
	void GameTrainCurveExitState::Execute(const shared_ptr<GameTrain>& train)
	{
		// �����x��0.5����1.0�ɖ߂�
		train->m_acsel = Utility::Lerp(ACSEL, 1.0f, train->m_moveRatio);

		// �J�[�u����`��Ԃŏ���
		Vec3 pos = Utility::Lerp(train->m_movePos.first, train->m_movePos.second, train->m_moveRatio);
		train->m_moveRatio = MathF::Repeat01(train->m_moveRatio, train->m_moveSpeed * train->m_acsel / ACSEL);

		// �͈͊O�ɂȂ�����
		if (MathF::GetOutRange())
		{
			// �I�����̃��[���̍��W��LINE�Ƃ��ĕێ�
			train->m_railPos = POS2LINE(train->m_movePos.second);

			// ���̃��[�����������Ȃ������玟�̃X�e�[�g��
			if (!train->SearchNextRail()) train->m_state = Train::State::Derail;
		}

		// ���W�̍X�V
		train->SetPosition(pos);
	}

	// �X�e�[�g�I�����̏���
	void GameTrainCurveExitState::Exit(const shared_ptr<GameTrain>& train)
	{
		train->m_acsel = 1.0f;
	}
}
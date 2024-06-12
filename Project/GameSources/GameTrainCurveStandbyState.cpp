/*!
@file GameTrainCurveStandbyState.cpp
@brief �Q�[�����̗�Ԃ̃J�[�u�J�n���̃X�e�[�g
@author ���V���M
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	const float HELF_TIME = 0.5f;	// �����̎���
	const float ACSEL = 0.75f;	// �����̎���

	// �C���X�^���X����
	shared_ptr<GameTrainCurveStandbyState> GameTrainCurveStandbyState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<GameTrainCurveStandbyState> instance(new GameTrainCurveStandbyState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g���擾
	wstring GameTrainCurveStandbyState::GetStateName() const
	{
		return L"GameTrainCurveStandbyState";
	}

	// �X�e�[�g�J�n���̏���
	void GameTrainCurveStandbyState::Enter(const shared_ptr<GameTrain>& train)
	{	
		// ���[���}�b�v�̎擾
		const auto& railMap = train->GetRailDataMap();
		if (railMap.empty()) return;

		// ���݂̃��[���A�J�[�u���̃��[���̍��W
		Vec3 pointA = railMap.at(train->m_railPos).thisPos;
		Vec3 pointB = railMap.at(train->m_railPos).futurePos;

		// �J�n���̃��[���̍��W�ƒ��Ԃ̍��W��ݒ�
		train->m_movePos.first = pointA;
		train->m_movePos.second = Utility::Lerp(pointA, pointB, HELF_TIME);

		// �����̏�����
		train->m_moveRatio = 0.0f;
	}

	// �X�e�[�g�X�V���̏���
	void GameTrainCurveStandbyState::Execute(const shared_ptr<GameTrain>& train)
	{
		// �����x��1.0����0.5�ɉ�����
		train->m_acsel = Utility::Lerp(1.0f, ACSEL, train->m_moveRatio);

		// �J�[�u����`��Ԃŏ���
		Vec3 pos = Utility::Lerp(train->m_movePos.first, train->m_movePos.second, train->m_moveRatio);
		train->m_moveRatio = MathF::Repeat01(train->m_moveRatio, train->m_moveSpeed * train->m_acsel / ACSEL);

		// �͈͊O�ɂȂ�����
		if (MathF::GetOutRange())
		{
			// �J�[�u���̃X�e�[�g�ɐ؂�ւ���
			train->m_trainState->ChangeState(GameTrainCurvingState::Instance());
		}

		// ���W�̍X�V
		train->SetPosition(pos);
	}

	// �X�e�[�g�I�����̏���
	void GameTrainCurveStandbyState::Exit(const shared_ptr<GameTrain>& train)
	{
		train->m_acsel = ACSEL;
	}
}
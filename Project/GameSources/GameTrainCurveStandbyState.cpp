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

	// �C���X�^���X����
	shared_ptr<GameTrainCurveStandbyState> GameTrainCurveStandbyState::Instance()
	{
		// new���Z�q�Ŏ��g�𐶐�
		static shared_ptr<GameTrainCurveStandbyState> instance(new GameTrainCurveStandbyState);

		// �V�����������ꂽthis�|�C���^
		return instance;
	}

	// �X�e�[�g�J�n���̏���
	void GameTrainCurveStandbyState::Enter(const shared_ptr<GameTrain>& train)
	{	
		// ���[���}�b�v�̎擾
		const auto& railMap = train->GetStage()->GetSharedGameObject<RailManager>(L"RailManager")->GetRailDataMap();
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
		// �J�[�u����`��Ԃŏ���
		Vec3 pos = Utility::Lerp(train->m_movePos.first, train->m_movePos.second, train->m_moveRatio);
		train->m_moveRatio = MathF::Repeat01(train->m_moveRatio, train->m_MoveSpeed / HELF_TIME);

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
		// ���̂Ƃ��뉽�����Ȃ�
	}
}